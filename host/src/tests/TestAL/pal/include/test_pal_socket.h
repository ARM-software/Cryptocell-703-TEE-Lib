/*******************************************************************************
* The confidential and proprietary information contained in this file may      *
* only be used by a person authorised under and to the extent permitted        *
* by a subsisting licensing agreement from ARM Limited or its affiliates.      *
*   (C) COPYRIGHT [2001-2017] ARM Limited or its affiliates.                   *
*       ALL RIGHTS RESERVED                                                    *
* This entire notice must be reproduced on all copies of this file             *
* and copies of this file may only be made by a person if such person is       *
* permitted to do so under the terms of a subsisting license agreement         *
* from ARM Limited or its affiliates.                                          *
*******************************************************************************/

#ifndef TP_SOCKET_H_
#define TP_SOCKET_H_

/*!
 @file
 @brief This file contains socket APIs used by tests.
 */

/*!
 @addtogroup pal_socket_test
 @{
 */

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DELAY 0xFFFFFFFF

typedef uint32_t tp_socket;

/** tp_sock_domain - communication domain
 *
 * @TP_AF_INET - Internet IP protocol
 */
enum tp_sock_domain {
    TP_AF_INET    = 1
};


/** tp_sock_type - Socket types
 *
 * @TP_SOCK_DGRAM - datagram (conn.less) socket
 * @TP_SOCK_STREAM - stream (connection) socket
 */
enum tp_sock_type {
    TP_SOCK_DGRAM    = 1,
    TP_SOCK_STREAM    = 2
};

/** test_pal_sock_protocol - protocol types
 *
 * @TP_IPPROTO_TCP - TCP socket
 * @TP_IPPROTO_UDP - UDP socket
 */
enum tp_sock_protocol {
    TP_IPPROTO_TCP    = 1,
    TP_IPPROTO_UDP    = 2
};

/******************************************************************************/
/*!
 * @brief This function creates an endpoint for communication.
 *
 *
 * @return 0 on success.
 * @return 1 on failure.
 */
uint32_t Test_PalSocket(tp_socket *s, /*<! Socket structure pointer.*/
 enum tp_sock_domain domain, /*<! Specifies a communication domain.*/
 enum tp_sock_type type,
 /*<! Indicated type, which specifies the communication semantics.*/
 enum tp_sock_protocol protocol,
 /*<! Specifies a particular protocol to be used with the socket.*/
 const uint32_t recvTimeout_ms /*<! Specifies receive timeout in milliseconds.*/
);

/******************************************************************************/
/*!
 * @brief This function closes an endpoint for communication.
 *
 *
 * @return 0 on success.
 * @return 1 on failure.
 */
uint32_t Test_PalCloseSocket(tp_socket s /*<! Socket handler to be closed.*/
);

/******************************************************************************/
/*!
 * @brief This function initiates a connection on a socket.
 *
 *
 * @return 0 on success.
 * @return 1 on failure.
 */
uint32_t Test_PalConnect(tp_socket s, /*<! Socket handler.*/
 const uint8_t *addr, /*<! Destination IP address.*/
 uint32_t port /*<! Destination port number.*/
);

/******************************************************************************/
/*!
 * @brief This function assigns the local address to the socket.
 *
 *
 * @return 0 on success.
 * @return 1 on failure.
 */
uint32_t Test_PalBind(tp_socket s, /*<! Socket handler.*/
 uint32_t port /*<! Socket local port number.*/
);

/******************************************************************************/
/*!
 * @brief This function listens for incoming connection on socket.
 *
 *
 * @return 0 on success.
 * @return 1 on failure.
 */
uint32_t Test_PalListen(tp_socket s, /*<! Socket handler.*/
 uint32_t backlog /*<! Puts a limit on the number of simultaneously
 connected clients.*/
);

/******************************************************************************/
/*!
 * @brief This function accept connection on a socket.
 *
 *
 * @return 0 on success.
 * @return 1 on failure.
 */
uint32_t Test_PalAccept(
 tp_socket s, /*<! The listening socket on which new connections are to be accepted.*/
 tp_socket *acptS, /*<! Handle of the accepted socket created.*/
 uint8_t *addr, /*<! IP Addr of the socket from which a connection was accepted.*/
 uint32_t *port /*<! Port number of the socket from which a connection was accepted.*/
);

/******************************************************************************/
/*!
 * @brief This function disable reads and writes on a connected TCP socket.
 *
 *
 * @return 0 on success.
 * @return 1 on failure.
 */
uint32_t Test_PalShutdown(tp_socket s /*<! Socket handler.*/
);

/******************************************************************************/
/*!
 * @brief This function sends messages via a specified socket.
 *
 *
 * @return Number of bytes sent on success.
 * @return 0 on failure.
 */
uint32_t Test_PalSend(tp_socket s, /*<! Active connected socket-descriptor.*/
 const uint8_t *buf, /*<! Pointer to data buffer prepared by user.*/
 size_t len /*<! Buffer size.*/
);

/******************************************************************************/
/*!
 * @brief This function sends messages via a specified socket.
 *
 *
 * @return Number of bytes sent on success.
 * @return 0 on failure.
 */
uint32_t Test_PalSendTo(tp_socket s, /*!< Active connected socket-descriptor.*/
 const uint8_t *buf, /*!< Pointer to data buffer prepared by user.*/
 size_t len, /*!< Buffer size.*/
 const uint8_t *addr, /*!< IP address.*/
 uint32_t port /*!< Port number.*/
);

/******************************************************************************/
/*!
 * @brief This function sends messages via a specified socket.
 *
 *
 * @return Number of bytes recv on success.
 * @return 0 on failure.
 */
uint32_t Test_PalRecvFrom(tp_socket s, /*!< Active socket-descriptor.*/
 const uint8_t *buf, /*!< Pointer to data buffer prepared by user.*/
 size_t len, /*!< Buffer size.*/
 uint8_t *addr, /*!< Received IP address.*/
 uint32_t *port /*!< Received port number.*/
);

/******************************************************************************/
/*!
 * @brief This function sends messages via a specified socket.
 *
 *
 * @return Number of bytes recv for success, 0 for failure.
 */
uint32_t Test_PalRecv(tp_socket s, /*!< Active socket-descriptor.*/
 const uint8_t *buf, /*!< Pointer to data buffer prepared by user.*/
 size_t len /*!< Buffer size.*/
);

/******************************************************************************/
/*!
 * @brief This function set the Byte Order and Endian of the host
 * to network long.
 *
 *
 * @return The converted value.
 */
uint32_t Test_PalHtonl(uint32_t val /*!< Value to convert.*/
);

/******************************************************************************/
/*!
 * @brief This function set the Byte Order and Endian of the host
 * to network short.
 *
 *
 * @return The converted value.
 */
uint16_t Test_PalHtons(uint16_t val /*!< Value to convert.*/
);

/******************************************************************************/
/*!
 * @brief This function set the Byte Order and Endian of the network
 * to host long.
 *
 *
 * @return The converted value.
 */
uint32_t Test_PalNtohl(uint32_t val /*!< Value to convert.*/
);

/******************************************************************************/
/*!
 * @brief This function set the Byte Order and Endian of the network
 * to host short.
 *
 *
 * @return The converted value.
 */
uint16_t Test_PalNtohs(uint16_t val /*!< Value to convert.*/
);

#ifdef __cplusplus
}
#endif
/*!
 @}
 */
#endif /* TP_SOCKET_H_ */
