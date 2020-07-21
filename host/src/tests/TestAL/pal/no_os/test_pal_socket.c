/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#include <stdint.h>

#include "test_pal_socket.h"
#include "test_pal_mem.h"

/* the time to wait for sent/recive replay */
#define TIMEOUT_MS	100

/******************************************************************************/
uint32_t Test_PalSocket(tp_socket *s, enum tp_sock_domain domain,
		enum tp_sock_type type, enum tp_sock_protocol protocol,
		const uint32_t recvTimeout_ms)
{
	(void)s;
	(void)domain;
	(void)type;
	(void)protocol;
	(void)recvTimeout_ms;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalCloseSocket(tp_socket s)
{
	(void)s;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalConnect(tp_socket s, const uint8_t *addr, uint32_t port)
{
	(void)s;
	(void)addr;
	(void)port;
	return 0;

}

/******************************************************************************/
uint32_t Test_PalBind(tp_socket s, uint32_t port)
{
	(void)s;
	(void)port;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalListen(tp_socket s, uint32_t backlog)
{
	(void)s;
	(void)backlog;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalAccept(tp_socket s, tp_socket *acptS, uint8_t *addr,
			 uint32_t *port)
{
	(void)s;
	(void)acptS;
	(void)addr;
	(void)port;
	return 0;
}

/******************************************************************************/
tp_socket Test_PalShutdown(tp_socket s)
{
	(void)s;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalSend(tp_socket s, const uint8_t *buf,
		      size_t len)
{
	(void)s;
	(void)buf;
	(void)len;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalSendTo(tp_socket s, const uint8_t *buf,
			size_t len, const uint8_t *addr, uint32_t port)
{
	(void)s;
	(void)buf;
	(void)len;
	(void)addr;
	(void)port;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalRecvFrom(tp_socket s, const uint8_t *buf,
			size_t len, uint8_t *addr, uint32_t *port)
{
	(void)s;
	(void)buf;
	(void)len;
	(void)addr;
	(void)port;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalRecv(tp_socket s, const uint8_t *buf,
		      size_t len)
{
	(void)s;
	(void)buf;
	(void)len;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalHtonl(uint32_t val)
{
	(void)val;
	return 0;
}

/******************************************************************************/
uint16_t Test_PalHtons(uint16_t val)
{
	(void)val;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalNtohl(uint32_t val)
{
	(void)val;
	return 0;
}

/******************************************************************************/
uint16_t Test_PalNtohs(uint16_t val)
{
	(void)val;
	return 0;
}

