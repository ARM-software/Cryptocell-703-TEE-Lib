#ifndef TESTS_INCLUDES_SELFTEST_SOCKET_H_
#define TESTS_INCLUDES_SELFTEST_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
* @brief This function tests the APIs of test_pal_socket.h in TestAL
* create 1 thread as the server, and main thread as the client.
* after establishing the connection between the client and the server,
* messages are sent from client to server and the other way around,
* each message is verified that it was received correctly,
* by checking its length and its content.
*
* @param[in]
*
* @param[out]
*
* @return - 0 for success, 1 for failure.
*/
int Test_Socket(void);

#endif /* TESTS_INCLUDES_SELFTEST_SOCKET_H_ */
