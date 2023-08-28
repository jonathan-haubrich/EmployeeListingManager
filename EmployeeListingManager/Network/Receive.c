
#include "Receive.h"

BOOL
ReceiveAll(
	SOCKET sRemote,
	PBYTE pBuffer,
	SIZE_T cbRecvLen)
{
	DWORD dwBytesReceived = 0;
	DWORD dwTotalBytes = 0;
	DWORD dwFlags = 0;
	WSABUF wsaBuf = { 0 };

	while (dwTotalBytes < cbRecvLen)
	{
		wsaBuf.buf = &pBuffer[dwTotalBytes];
		wsaBuf.len = (ULONG)(cbRecvLen - dwTotalBytes);

		if (SOCKET_ERROR == WSARecv(sRemote,
			&wsaBuf,
			1,
			&dwBytesReceived,
			&dwFlags,
			NULL,
			NULL))
		{
			return FALSE;
		}

		dwTotalBytes += dwBytesReceived;
	}

	return TRUE;
}

BOOL
ReceiveRequestAddUser(
	SOCKET sRemote,
	PREQUEST_ADD_USER *ppRequestAddUser)
{
	if (NULL == ppRequestAddUser)
	{
		return FALSE;
	}

	BYTE bId = 0;
	SIZE_T cbListingSize = 0;
	SIZE_T cbRemaining = 0;
	PREQUEST_ADD_USER pRequestAddUser = NULL;
	PBYTE pBuffer = NULL;

	if (FALSE == ReceiveAll(sRemote, &bId, sizeof(bId)))
	{
		goto fail;
	}

	if (FALSE == ReceiveAll(sRemote, (PBYTE)&cbListingSize, sizeof(cbListingSize)))
	{
		goto fail;
	}
	cbListingSize = htonll(cbListingSize);

	pRequestAddUser = (PREQUEST_ADD_USER)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,
		cbListingSize);

	pRequestAddUser->bId = bId;
	pRequestAddUser->cbListingSize = cbListingSize;

	cbRemaining = cbListingSize - sizeof(bId) - sizeof(cbListingSize);
	pBuffer = (PBYTE)pRequestAddUser + (cbListingSize - cbRemaining);

	if (FALSE == ReceiveAll(sRemote, pBuffer, cbRemaining))
	{
		goto fail;
	}

	*ppRequestAddUser = pRequestAddUser;
	return TRUE;

fail:
	if (NULL != pRequestAddUser)
	{
		HeapFree(GetProcessHeap(), 0, pRequestAddUser);
	}

	return FALSE;
}

BOOL
ReceiveRequestRemoveUser(
	SOCKET sRemote,
	PREQUEST_REMOVE_USER pRequestRemoveUser)
{
	return ReceiveAll(sRemote, (PBYTE)pRequestRemoveUser, sizeof(*pRequestRemoveUser));
}

BOOL
ReceiveRequestGetUser(
	SOCKET sRemote,
	PREQUEST_GET_USER pRequestGetUser)
{
	return ReceiveAll(sRemote, (PBYTE)pRequestGetUser, sizeof(*pRequestGetUser));
}

BOOL
ReceiveRequestDisplayUser(
	SOCKET sRemote,
	PREQUEST_DISPLAY_USER pRequestDisplayUser)
{
	return ReceiveAll(sRemote, (PBYTE)pRequestDisplayUser, sizeof(*pRequestDisplayUser));
}