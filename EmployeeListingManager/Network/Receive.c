
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
ReceiveRequestAddUserDataElement(
	SOCKET sRemote,
	BYTE cbElementRecvLen,
	BYTE pbElementBufferMaxSize,
	PBYTE pbElementBuffer,
	PBYTE pbElementBufferLen)
{
	PBYTE pBuffer = NULL;
	BOOL fSuccess = TRUE;

	pBuffer = HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,
		cbElementRecvLen);

	if (FALSE == ReceiveAll(sRemote, pBuffer, cbElementRecvLen))
	{
		fSuccess = FALSE;
		goto end;
	}
	*pbElementBufferLen = min(pbElementBufferMaxSize, cbElementRecvLen);
	RtlCopyMemory(pbElementBuffer,
		pBuffer,
		*pbElementBufferLen);

end:
	if (NULL != pBuffer)
	{
		HeapFree(GetProcessHeap(), 0, pBuffer);
	}
	return fSuccess;
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

	REQUEST_ADD_USER_METADATA MetaData = { 0 };
	SIZE_T cbRequired = 0;
	PREQUEST_ADD_USER pRequestAddUser = NULL;

	if (FALSE == ReceiveAll(sRemote, (PBYTE)&MetaData, sizeof(MetaData)))
	{
		goto fail;
	}

	cbRequired = sizeof(MetaData) +
		MetaData.cbFormatter +
		MetaData.cbFirstName +
		MetaData.cbLastName +
		MetaData.cbDescription;

	pRequestAddUser = (PREQUEST_ADD_USER)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,
		cbRequired);

	RtlCopyMemory(&pRequestAddUser->MetaData, &MetaData, sizeof(pRequestAddUser->MetaData));
	pRequestAddUser->MetaData.cbListingSize = htonl(pRequestAddUser->MetaData.cbListingSize);

	if (FALSE == ReceiveRequestAddUserDataElement(sRemote,
		pRequestAddUser->MetaData.cbFormatter,
		FORMATTER_NAME_MAX_LEN,
		pRequestAddUser->Data.sFormatter,
		&pRequestAddUser->MetaData.cbFormatter))
	{
		goto fail;
	}

	if (FALSE == ReceiveRequestAddUserDataElement(sRemote,
		pRequestAddUser->MetaData.cbFirstName,
		EL_FIRSTNAME_MAX_LEN,
		pRequestAddUser->Data.sFirstName,
		&pRequestAddUser->MetaData.cbFirstName))
	{
		goto fail;
	}

	if (FALSE == ReceiveRequestAddUserDataElement(sRemote,
		pRequestAddUser->MetaData.cbLastName,
		EL_LASTNAME_MAX_LEN,
		pRequestAddUser->Data.sLastName,
		&pRequestAddUser->MetaData.cbLastName))
	{
		goto fail;
	}

	if (FALSE == ReceiveRequestAddUserDataElement(sRemote,
		pRequestAddUser->MetaData.cbDescription,
		EL_DESCRIPTION_MAX_LEN,
		pRequestAddUser->Data.sDescription,
		&pRequestAddUser->MetaData.cbDescription))
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