#ifndef _SEND_H
#define _SEND_H

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

BOOL
SendAll(
	SOCKET sRemote,
	PBYTE pBuffer,
	SIZE_T cbSendLen);

#endif // _SEND_H