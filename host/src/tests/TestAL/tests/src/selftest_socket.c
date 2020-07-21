#include <stdint.h>
#include <string.h>
#include "test_pal_socket.h"
#include "test_pal_log.h"
#include "test_pal_time.h"
#include "test_pal_mem.h"
#include "selftest_socket.h"
#include "test_pal_thread.h"

/*******************************************************************************
 *
 * Global Variables
 *
 ******************************************************************************/

/* 0 for success, 1 for failure */
int serverCreated = 0;

/*******************************************************************************
 *
 * Structs
 *
 ******************************************************************************/

struct SocketStr {
	tp_socket sockDesc;	/* Descriptor TCP-IP Socket	*/
};

/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/

typedef void *comHdl;

#define MAX_SOCKET_CONNECTIONS		10
#define MAX_SIZE_OF_RECEIVED_DATA	7340032 /* 7MB */
#define MAX_IP_ADDR_LEN			16

/* max number of retries for sending data to socket */
#define COMM_SEND_MAX_RETRIES		5
/* max number of retries for connecting to target */
#define COMM_CONNECT_MAX_RETRIES	10

/* delay between retries in usec */
#define COMM_DELAY_BETWEEN_RETRIES_USEC	2000000

#define RC_SUCCESS				0
#define RC_FAIL					1

/* Port Number declaration */
#define PORT_NUM_OF_CTRL_CHANNEL	5670

/*******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/

/*
 * @brief Create and open communication for client.
 *        This function initialize the communication channel
 *
 * @param ipAddr   - (input)  pointer to server IP address
 * @param port     - (input)  connection server port
 *
 * @return pointer to connection handler, in error - NULL
 */
comHdl *comm_clientInitConnection(const uint8_t *ipAddr, uint32_t port)
{
	uint32_t rc = RC_SUCCESS;
	struct SocketStr *pStr;
	uint32_t retry = COMM_CONNECT_MAX_RETRIES;

	/* allocate the socket str */
	pStr = (struct SocketStr *)Test_PalMalloc(sizeof(struct SocketStr));
	if (pStr == NULL) {
		TEST_PRINTF_ERROR("failed to allocate socket");
		goto alloc_err1;
	}
	TEST_PRINTF("init connection... port %u , sock 0x%x"
			" ip: %s \n",
						port, &(pStr->sockDesc), ipAddr);

	/* Create socket */
	rc = Test_PalSocket(&(pStr->sockDesc), TP_AF_INET,
			TP_SOCK_STREAM, TP_IPPROTO_TCP, MAX_DELAY);
	if (rc == RC_FAIL) {
		TEST_PRINTF_ERROR("failed to create socket (client)");
		goto sock_err;
	}

	TEST_PRINTF("after socket creation - client \n");

	/* connect to remote address */
	do {
		rc = Test_PalConnect(pStr->sockDesc, ipAddr, port);
		if (rc == RC_FAIL) {
			TEST_PRINTF("Connect Retry: port %u", port, *ipAddr);
			Test_PalDelay(COMM_DELAY_BETWEEN_RETRIES_USEC);
		}
		retry--;
	} while ((retry) && (rc == RC_FAIL));

	if ((rc == RC_FAIL) && (retry == 0)) {
		TEST_PRINTF_ERROR("failed to connect (client)");
		goto conn_err;
	}

	TEST_PRINTF("after connect (client) \n");

	return (comHdl *)pStr;

conn_err:
	Test_PalCloseSocket(pStr->sockDesc);
sock_err:
	Test_PalFree(pStr);
alloc_err1:
	return NULL;
}

/*
 * @brief comm_serverInitConnection() API function -
 *        Create and open server communication channel.
 *
 * \param port     - (input)  server port number
 *
 * \return pointer to connection handler, in error - NULL
 */
comHdl *comm_serverInitConnection(uint32_t port)
{
	uint32_t rc = RC_SUCCESS;
	struct SocketStr *pStr;

	/* allocate the socket str */
	pStr = (struct SocketStr *)Test_PalMalloc(sizeof(struct SocketStr));
	if (pStr == NULL) {
		TEST_PRINTF_ERROR("failed to allocate - out of memory");
		goto alloc_err;
	}
	TEST_PRINTF("init connection... port %u , sock 0x%x \n",
						port, &(pStr->sockDesc));

	/* Create socket */
	rc = Test_PalSocket(&(pStr->sockDesc), TP_AF_INET,
			TP_SOCK_STREAM, TP_IPPROTO_TCP, MAX_DELAY);
	if (rc == 1) {
		TEST_PRINTF_ERROR("failed to create socket (server)");
		goto sock_err;
	}

	/* bind the server listening port to the created socket */
	rc = Test_PalBind(pStr->sockDesc, port);
	if (rc == 1) {
		TEST_PRINTF_ERROR("failed to bind socket (server)");
		goto bind_err;
	}

	/* wait for connections */
	rc = Test_PalListen(pStr->sockDesc, MAX_SOCKET_CONNECTIONS);
	if (rc == 1) {
		TEST_PRINTF_ERROR("failed to listen socket (server)");
		goto listen_err;
	}

	return (comHdl *)pStr;

listen_err:
bind_err:
	Test_PalCloseSocket(pStr->sockDesc);
sock_err:
	Test_PalFree(pStr);
alloc_err:
	return NULL;
}

/*
 * @brief comm_serverAcceptConnection() API function -
 *     Wait for client to connect (blocking) and returns the connection handle.
 *
 * @param hdl      - (input)  connection handler structure
 *
 * @return pointer to connection handler, in error - NULL
 */
comHdl *comm_serverAcceptConnection(comHdl *hdl)
{
	uint32_t rc;
	uint8_t addr[MAX_IP_ADDR_LEN];
	uint32_t port;
	struct SocketStr *pAcceptSockStr;
	struct SocketStr *pStr = (struct SocketStr *)hdl;

	/* Check validity */
	if (hdl == NULL) {
		/* Input is wrong */
		TEST_PRINTF_ERROR("failed to accept connection (server)"
				" - commHdl is NULL");
		return NULL;
	}

	/* allocate the accept socket str */
	pAcceptSockStr = (struct SocketStr *)Test_PalMalloc(
						sizeof(struct SocketStr));
	if (pAcceptSockStr == NULL) {
		TEST_PRINTF_ERROR("failed to allocate - out of memory");
		return NULL;
	}

	/* accept connection from an incoming client */
	rc = Test_PalAccept(pStr->sockDesc,
				&(pAcceptSockStr->sockDesc),
				addr, &port);
	if (rc == 1) {
		TEST_PRINTF_ERROR("failed to accept connection");
		Test_PalFree(pAcceptSockStr);
		return NULL;
	}

	return (comHdl *)pAcceptSockStr;
}

/*
 * @brief comm_recvData() API function - receive data from communication
 *        This function get buffer pointer and also the relevant buffer size
 *
 * @param hdl           - (input)   connection handler structure
 * @param size          - (output)  size of message
 *
 * @return uint32_t return code - pointer to received data, NULL in case error
 */
uint8_t *comm_recvData(comHdl *hdl, uint32_t *size)
{
	uint32_t sizeMsg                = 0;
	uint32_t byteRead               = 0;
	uint8_t *buf;
	const uint8_t *sizeMsgAddr = (const uint8_t *)(&sizeMsg);
	struct SocketStr *sHdl = (struct SocketStr *)hdl;

	/* Check validity */
	if (sHdl == NULL) {
		/* Input is wrong */
		TEST_PRINTF_ERROR("failed to receive data - "
				"comHdl is NULL");
		return NULL;
	}
	/* reset the buffer size */
	*size = 0;
	/* Read the first word in order to know a message size */
	do {
		byteRead += Test_PalRecv(sHdl->sockDesc,
					sizeMsgAddr + byteRead,
					sizeof(sizeMsg) - byteRead);
	} while (byteRead != sizeof(sizeMsg));
	/* check that received size valid */
	if (sizeMsg >= MAX_SIZE_OF_RECEIVED_DATA)
		return NULL;
	/* set the buffer size to be received with the message */
	*size = sizeMsg;
	/* allocate buffer according to expected size */
	buf = (uint8_t *)Test_PalMalloc(sizeMsg);
	if (buf == NULL)
		return NULL;

	/* start looping to receive data */
	byteRead = 0;
	do {
		/* get the data */
		byteRead += Test_PalRecv(sHdl->sockDesc, buf + byteRead,
						sizeMsg - byteRead);
	} while (byteRead != sizeMsg);

	/* return allocated data to the user */
	return buf;
}

uint32_t comm_sendDataBuf(tp_socket sock,
				const uint8_t *buf, uint32_t size)
{
	uint32_t byteSent = 0;
	uint32_t byteSentSum = 0;
	uint32_t retries = 0;
	uint8_t *msg = (uint8_t *)buf;

	do {
		/* loop until all data successfully send
		 *  (limited by max retries) */
		if (retries == COMM_SEND_MAX_RETRIES) {
			/* despite retries, failed to complete data sending */
			TEST_PRINTF_ERROR("failed to send data");
			return RC_FAIL;
		}
		byteSent = Test_PalSend(sock, msg, size);
		msg += byteSent;
		byteSentSum += byteSent;
		size -= byteSent;
		retries++;
	} while (size > 0);

	return byteSentSum;
}

/*
 * @brief comm_sendData() API function - send data via communication
 *        This function send buffer according to the buffer size by using
 *        communication module
 *
 * @param hdl          - (input)  connection handler structure
 * @param buf          - (input)  pointer to buffer to send
 * @param size         - (input)  length of buffer to send
 *
 * @return - number of bytes sent for success, 0 for failure.
 */
uint32_t comm_sendData(comHdl *hdl, const uint8_t *buf, uint32_t size)
{
	uint32_t rc = RC_SUCCESS;
	uint8_t *msg;
	struct SocketStr *sHdl = (struct SocketStr *)hdl;

	/* Check validity */
	if (sHdl == NULL) {
		/* Input is wrong */
		TEST_PRINTF_ERROR("failed to send data -"
				" connFd (socket handler) is NULL");
		return RC_FAIL;
	}

	/* first send size of the data to be sent */
	msg = (uint8_t *)&size;
	rc = comm_sendDataBuf(sHdl->sockDesc, msg, sizeof(size));

	/* send the actual data */
	rc += comm_sendDataBuf(sHdl->sockDesc, buf, size);

	return rc;
}

/*!
 * @brief comm_closeConnection() API function - finish communication
 *        This function close the communication channel
 *        communication module
 *
 * @param hdl   - (input)  connection handler structure
 *
 * @return uint32_t return code - PASS(0) / FAIL(1)
 */
uint32_t comm_closeConnection(comHdl *hdl)
{
	uint32_t rc = RC_SUCCESS;
	struct SocketStr *pStr = (struct SocketStr *)hdl;

	/* Check validity */
	if (pStr == NULL) {
		/* Input is wrong */
		TEST_PRINTF_ERROR("failed to close connection - "
				"invalid socket input");
		return RC_FAIL;
	}

	/* Close Connection */
	if (pStr->sockDesc) {
		/* close socket/file descriptor */
		rc = Test_PalCloseSocket(pStr->sockDesc);
	}

	/* free SocketStr that was allocated during InitConnection */
	Test_PalFree(pStr);

	return rc;
}

static void *serverFunc(void *args) {

	comHdl *comHdls;
	comHdl *serverHandler;
	uint32_t rc;
	uint32_t writeSize = 53;
	uint32_t readSize = 0;
	uint8_t *testStrRcv = NULL;
	uint32_t *ret = Test_PalDMAContigBufferAlloc
			(sizeof(uint32_t));
	char *testStrSnd = Test_PalDMAContigBufferAlloc(sizeof(char)*writeSize);
	(void)args;

	strcpy(testStrSnd, "SERVER: This is a test -"
			" pal_socket works correctly!");

	TEST_PRINTF("server start \n");

	/* Init Connection */
	comHdls = comm_serverInitConnection(PORT_NUM_OF_CTRL_CHANNEL);
	if (comHdls == NULL){
		TEST_PRINTF_ERROR("failed to initialise Connection (server)");
		*ret = 1;
		return ret;
	}

	serverCreated = 1;

	/* Wait for connection from host */
	serverHandler = comm_serverAcceptConnection(comHdls);
	if (serverHandler == NULL){
		TEST_PRINTF_ERROR("failed to accept connection from host");
		comm_closeConnection(comHdls);
		*ret = 1;
		return ret;
	}

	TEST_PRINTF("server accepted connection \n");

	/* Receive buffer from the client */

	/* wait for data to be written by client */
	testStrRcv = comm_recvData(serverHandler, &readSize);
	TEST_PRINTF("server received data \n");
	TEST_PRINTF("Received message: %s \n", testStrRcv);
	if (testStrRcv == NULL){
		TEST_PRINTF_ERROR("failed to receive data buffer"
				" (server) - pointer to buffer is NULL");
		*ret = 1;
		return ret;
	}

	if (readSize != 53){
		TEST_PRINTF_ERROR("failed to receive data buffer"
				" (server) - buffer size received is incorrect");
		*ret = 1;
		return ret;
	}

	if (strcmp((const char*) testStrRcv, "CLIENT: This is a test -"
			" pal_socket works correctly!") != 0) {
		TEST_PRINTF_ERROR("failed to receive data buffer"
				" (server) - buffer content is incorrect");
		*ret = 1;
		return ret;
	}

	TEST_PRINTF("server received data correctly \n");

	rc = comm_sendData(serverHandler,(const uint8_t*) testStrSnd, writeSize);
	if (rc == 0) {
		TEST_PRINTF_ERROR("failed to send data (client)");
		*ret = 1;
		return ret;
	}

	TEST_PRINTF("server sent data \n");

	*ret = 0;
	return ret;

}

/*******************************************************************************
 *
 * Tests
 *
 ******************************************************************************/

int Test_Socket(void)
{

	ThreadHandle targetThread;
	uint32_t *argVal = Test_PalDMAContigBufferAlloc
			(sizeof(uint32_t));
	uint32_t **arg = &argVal;
	int args = 1;
	uint32_t rc;
	uint32_t writeSize = 53;
	uint32_t readSize = 0;
	comHdl clientHandler;
	uint8_t *testStrRcv = NULL;
	uint8_t *testStrSnd = Test_PalDMAContigBufferAlloc
			(sizeof(uint8_t)*writeSize);
	uint8_t *ipaddr = Test_PalDMAContigBufferAlloc
			(sizeof(uint8_t)*16);

	/* lookback address */
	strcpy((char *)ipaddr, "127.0.0.1");
	strcpy((char *)testStrSnd, "CLIENT: This is a test -"
			" pal_socket works correctly!");

	targetThread = Test_PalThreadCreate(
					Test_PalGetMinimalStackSize(),
					&serverFunc,
					7, &args,
					"tt",
					3, 0);

	TEST_PRINTF("thread created successfully \n");

	while(serverCreated == 0){
		Test_PalDelay(1000);
	}

	/* Init Connection client */

	Test_PalDelay(10000);

	clientHandler = comm_clientInitConnection(ipaddr,
			PORT_NUM_OF_CTRL_CHANNEL);
	if (clientHandler == NULL) {
		TEST_PRINTF_ERROR("failed to initialise Connection (client) \n");
		goto error;
	}

	TEST_PRINTF("after client init \n");

	rc = comm_sendData(clientHandler, testStrSnd, writeSize);
	if (rc == 0) {
		TEST_PRINTF_ERROR("failed to send data (client)");
		goto error;
	}

	TEST_PRINTF("after client send \n");

	/* Receive buffer from the client */

	/* wait for data to be written by client */
	testStrRcv = comm_recvData(clientHandler, &readSize);
	TEST_PRINTF("after client receive \n");
	if (testStrRcv == NULL){
		TEST_PRINTF_ERROR("failed to receive data buffer"
				" (client) - pointer to buffer is NULL");
		goto error;
	}

	if (readSize != 53){
		TEST_PRINTF_ERROR("failed to receive data buffer"
				" (client) - buffer size received is incorrect");
		goto error;
	}

	if (strcmp((const char *)testStrRcv, "SERVER: This is a test -"
			" pal_socket works correctly!") != 0) {
		TEST_PRINTF_ERROR("failed to receive data buffer"
				" (client) - buffer content is incorrect");
		goto error;
	}

	TEST_PRINTF("client received data correctly \n");

	if (Test_PalThreadJoin(targetThread, (void **)arg) != 0) {
		TEST_PRINTF_ERROR("Failed to Test_PalThreadJoin()\r\n");
		goto error;
	}

	if(*arg == NULL){
		TEST_PRINTF_ERROR("Return value from server thread is NULL \n");
				goto error;
	}

	TEST_PRINTF("After JOIN \n");
	TEST_PRINTF("Return value of thread: %d \n", **arg);

	/*
	*server thread terminated with some error
	*server thread prints the specific error before returning
	*/
	if(**arg == 1) {
		goto error;
	}

	if (Test_PalThreadDestroy(targetThread) != 0) {
		TEST_PRINTF_ERROR("Failed to destroy thread\r\n");
		goto error;
	}

	Test_PalDMAContigBufferFree(testStrSnd);
	Test_PalDMAContigBufferFree(ipaddr);
	Test_PalDMAContigBufferFree(argVal);

	TEST_PRINTF_MESSAGE("Socket test result: SUCCESS \r\n");
	return 0;
error:
	TEST_PRINTF_MESSAGE("Socket test result: FAILED \r\n");
	return 1;
}
