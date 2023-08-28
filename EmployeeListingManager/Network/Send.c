#include "Send.h"

BOOL
SendAll(
	SOCKET sRemote,
	PBYTE pBuffer,
	SIZE_T cbSendLen)
{
	DWORD dwBytesSent = 0;
	DWORD dwTotalBytes = 0;
	WSABUF wsaBuf = { 0 };

	while (dwTotalBytes < cbSendLen)
	{
		wsaBuf.buf = (CHAR*)&pBuffer[dwTotalBytes];
		wsaBuf.len = (ULONG)(cbSendLen - dwTotalBytes);

		if (0 != WSASend(sRemote,
			&wsaBuf,
			1,
			&dwBytesSent,
			0,
			NULL,
			NULL))
		{
			return FALSE;
		}

		dwTotalBytes += dwBytesSent;
	}

	return TRUE;
}