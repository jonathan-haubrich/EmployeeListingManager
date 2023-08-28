#ifndef _RECEIVE_H
#define _RECEIVE_H

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

#include "../Protocol/Requests.h"

BOOL
ReceiveAll(
	SOCKET sRemote,
	PBYTE pBuffer,
	SIZE_T cbRecvLen);

BOOL
ReceiveRequestAddUser(
	SOCKET sRemote,
	PREQUEST_ADD_USER* ppRequestAddUser);

BOOL
ReceiveRequestRemoveUser(
	SOCKET sRemote,
	PREQUEST_REMOVE_USER pRequestRemoveUser);

BOOL
ReceiveRequestGetUser(
	SOCKET sRemote,
	PREQUEST_GET_USER pRequestGetUser);

BOOL
ReceiveRequestDisplayUser(
	SOCKET sRemote,
	PREQUEST_DISPLAY_USER pRequestDisplayUser);

#endif // _RECEIVE_H