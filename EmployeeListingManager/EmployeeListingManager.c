#include "EmployeeListingManager.h"

BOOL
StartServer(
	PWSTR pwszIpAddress,
	PWSTR pwszPort,
	SOCKET* psServer)
{
	WSADATA wsaData = { 0 };
	ADDRINFOW aiHints = { 0 };
	aiHints.ai_family = AF_INET;
	aiHints.ai_socktype = SOCK_STREAM;
	aiHints.ai_protocol = IPPROTO_TCP;
	aiHints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;

	PADDRINFOW pResults = NULL;

	SOCKET sServer = INVALID_SOCKET;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		fwprintf(stderr, L"WSAStartup failed: %d", WSAGetLastError());
		return FALSE;
	}

	wprintf(L"Starting server on %s:%s\r\n", pwszIpAddress, pwszPort);
	if (0 != GetAddrInfoW(pwszIpAddress, pwszPort, (const ADDRINFOW*)&aiHints, &pResults))
	{
		fwprintf(stderr, L"GetAddrInfo failed: %d", WSAGetLastError());
		return FALSE;
	}

	// take first result
	// we assume user will enter specific ip address or 0.0.0.0
	sServer = WSASocketW(pResults->ai_family, pResults->ai_socktype, pResults->ai_protocol, NULL, 0, 0);
	if (INVALID_SOCKET == sServer)
	{
		fwprintf(stderr, L"WSASocketW failed: %d", WSAGetLastError());
		return FALSE;
	}

	if (0 != bind(sServer, (const struct sockaddr*)(pResults->ai_addr), (INT)pResults->ai_addrlen))
	{
		fwprintf(stderr, L"bind failed: %d", WSAGetLastError());
		return FALSE;
	}

	if (0 != listen(sServer, SOMAXCONN))
	{
		fwprintf(stderr, L"listen failed: %d", WSAGetLastError());
		return FALSE;
	}

	*psServer = sServer;
	return TRUE;
}

VOID
StopServer(
	SOCKET sServer)
{
	shutdown(sServer, SD_BOTH);
	closesocket(sServer);
	WSACleanup();
}

BOOL
HandleRequestAddUser(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings)
{
	PREQUEST_ADD_USER pRequestAddUser = NULL;
	RESPONSE_HEADER ResponseHeader = { ELM_RESPONSE_SUCCESS };
	PEMPLOYEE_LISTING pEmployeeListing = NULL;
	FN_EMPLOYEEFORMATTER fnFormatter = NULL;

	if (FALSE == ReceiveRequestAddUser(sRemote, &pRequestAddUser))
	{
		return FALSE;
	}

	if (NULL != EmployeeListingCollectionGetListingById(pelcListings, pRequestAddUser->MetaData.bId))
	{
		ResponseHeader.bResponseCode = ELM_RESPONSE_USER_ALREADY_EXISTS;
		goto end;
	}

	fnFormatter = GetFormatterByName(pRequestAddUser->Data.sFormatter);
	if (NULL == fnFormatter)
	{
		ResponseHeader.bResponseCode = ELM_RESPONSE_INVALID_FORMATTER;
		goto end;
	}

	pEmployeeListing = EmployeeListingNew(pRequestAddUser->MetaData.bId,
		pRequestAddUser->Data.sFirstName,
		pRequestAddUser->Data.sLastName,
		pRequestAddUser->MetaData.cbDescription,
		pRequestAddUser->Data.sDescription,
		fnFormatter);
	if (NULL == pEmployeeListing)
	{
		ResponseHeader.bResponseCode = ELM_RESPONSE_FAILURE;
	}

	EmployeeListingCollectionAddListing(pelcListings, pEmployeeListing);

end:
	if (NULL != pEmployeeListing)
	{
		HeapFree(GetProcessHeap(), 0, pEmployeeListing);
	}
	if (NULL != pRequestAddUser)
	{
		HeapFree(GetProcessHeap(), 0, pRequestAddUser);
	}
	return SendAll(sRemote, (PBYTE)&ResponseHeader, sizeof(ResponseHeader));
}

BOOL
HandleRequestRemoveUser(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings)
{
	REQUEST_REMOVE_USER RequestRemoveUser = { 0 };
	RESPONSE_HEADER ResponseHeader = { ELM_RESPONSE_SUCCESS };
	PEMPLOYEE_LISTING pEmployeeListing = NULL;

	if (FALSE == ReceiveRequestRemoveUser(sRemote, &RequestRemoveUser))
	{
		return FALSE;
	}

	pEmployeeListing = EmployeeListingCollectionGetListingById(pelcListings, RequestRemoveUser.bId);
	if (NULL == pEmployeeListing)
	{
		ResponseHeader.bResponseCode = ELM_RESPONSE_UNKNOWN_USER;
		goto end;
	}

	EmployeeListingCollectionRemoveListing(pelcListings, pEmployeeListing);

end:
	return SendAll(sRemote, (PBYTE)&ResponseHeader, sizeof(ResponseHeader));
}

BOOL
HandleRequestListUsers(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings)
{
	BOOL fSuccess = TRUE;
	PRESPONSE_HEADERPAYLOAD pResponseHeaderPayload = NULL;
	PEMPLOYEE_LISTING pEmployeeListing = NULL;

	BYTE cEntries = EmployeeListingCollectionGetEntries(pelcListings);
	SIZE_T cbEntriesSize = sizeof(pEmployeeListing->MetaData.bId) * cEntries;
	SIZE_T cbTotalSize = ((sizeof(*pResponseHeaderPayload) - 1) + cbEntriesSize);

	pResponseHeaderPayload = HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,
		cbTotalSize);

	pResponseHeaderPayload->rhHeader.bResponseCode = ELM_RESPONSE_SUCCESS;
	pResponseHeaderPayload->rdPayload.cPayloadSize = (BYTE)cbEntriesSize;
	pResponseHeaderPayload->rdPayload.dwNumItems = cEntries;

	pEmployeeListing = pelcListings->pelFirst;
	BYTE cEntry = 0;

	while (NULL != pEmployeeListing)
	{
		pResponseHeaderPayload->rdPayload.abPayload[cEntry] = pEmployeeListing->MetaData.bId;

		++cEntry;
		pEmployeeListing = pEmployeeListing->MetaData.pelNext;
	}

	fSuccess = SendAll(sRemote, (PBYTE)pResponseHeaderPayload, cbTotalSize);
	HeapFree(GetProcessHeap(), 0, pResponseHeaderPayload);
	return fSuccess;
}

BOOL
HandleRequestDisplayUser(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings)
{
	REQUEST_DISPLAY_USER RequestDisplayUser = { 0 };
	RESPONSE_HEADER ResponseHeader = { ELM_RESPONSE_SUCCESS };
	PEMPLOYEE_LISTING pEmployeeListing = NULL;

	if (FALSE == ReceiveRequestDisplayUser(sRemote, &RequestDisplayUser))
	{
		return FALSE;
	}

	pEmployeeListing = EmployeeListingCollectionGetListingById(pelcListings, RequestDisplayUser.bId);
	if (NULL == pEmployeeListing)
	{
		ResponseHeader.bResponseCode = ELM_RESPONSE_UNKNOWN_USER;
		goto end;
	}

	SendAll(sRemote, (PBYTE)&ResponseHeader, sizeof(ResponseHeader));
	pEmployeeListing->MetaData.fnFormatter((HANDLE)sRemote, pEmployeeListing);
	return TRUE;

end:
	return SendAll(sRemote, (PBYTE)&ResponseHeader, sizeof(ResponseHeader));
}

BOOL
HandleRequestInvalidCommand(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings)
{
	RESPONSE_HEADER ResponseHeader = { ELM_RESPONSE_INVALID_COMMAND };
	return SendAll(sRemote, (PBYTE)&ResponseHeader, sizeof(ResponseHeader));
}

BOOL
HandleConnection(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings)
{
	REQUEST_HEADER RequestHeader = { 0 };
	BOOL fSuccess = TRUE;

	if (FALSE == ReceiveAll(sRemote, (PBYTE)&RequestHeader, sizeof(RequestHeader)))
	{
		return FALSE;
	}

	switch (RequestHeader.bCommandType)
	{
	case ELM_REQUEST_ADD_USER:
		fSuccess = HandleRequestAddUser(sRemote, pelcListings);
		break;
	case ELM_REQUEST_REMOVE_USER:
		fSuccess = HandleRequestRemoveUser(sRemote, pelcListings);
		break;
	case ELM_REQUEST_LIST_USERS:
		fSuccess = HandleRequestListUsers(sRemote, pelcListings);
		break;
	case ELM_REQUEST_DISPLAY_USER:
		fSuccess = HandleRequestDisplayUser(sRemote, pelcListings);
		break;
	default:
		fSuccess = HandleRequestInvalidCommand(sRemote, pelcListings);
		break;
	}

	closesocket(sRemote);

	return fSuccess;
}

BOOL
HandleNetworkEvent(
	SOCKET sServer,
	WSAEVENT wsaEvent,
	PEMPLOYEE_LISTING_COLLECTION pelcListings)
{
	WSANETWORKEVENTS wsaNetworkEvents = { 0 };
	SOCKET sRemote = INVALID_SOCKET;
	SOCKADDR_STORAGE sasRemote = { 0 };
	INT iAddrLen = 0;

	if (0 != WSAEnumNetworkEvents(sServer, wsaEvent, &wsaNetworkEvents))
	{
		return FALSE;
	}

	if (FD_CLOSE == (wsaNetworkEvents.lNetworkEvents & FD_CLOSE))
	{
		return FALSE;
	}
	if (FD_ACCEPT == (wsaNetworkEvents.lNetworkEvents & FD_ACCEPT))
	{
		if (wsaNetworkEvents.iErrorCode[FD_ACCEPT_BIT] == 1)
		{
			fwprintf(stderr, L"FD_ACCEPT encountered error: %d\r\n", WSAGetLastError());
			return FALSE;
		}

		iAddrLen = sizeof(sasRemote);
		SecureZeroMemory(&sasRemote, iAddrLen);

		sRemote = WSAAccept(sServer, (struct sockaddr*)&sasRemote, &iAddrLen, NULL, 0);
		if (INVALID_SOCKET == sRemote)
		{
			fwprintf(stderr, L"WSAAccept failed: %d", WSAGetLastError());
			return FALSE;
		}

		return HandleConnection(sRemote, pelcListings);
	}

	return TRUE;
}

BOOL
ServerLoop(
	SOCKET sServer,
	WSAEVENT wsaeStop,
	PEMPLOYEE_LISTING_COLLECTION pelcListings)
{
	DWORD dwHandle = 0;
	BOOL fStop = FALSE;

	WSAEVENT wsaeServer = INVALID_HANDLE_VALUE;
	WSAEVENT wsaEvents[] = {
		wsaeServer,
		wsaeStop
	};
	
	wsaeServer = WSACreateEvent();
	if (NULL == wsaeServer ||
		0 != WSAEventSelect(sServer, wsaeServer, FD_ACCEPT))
	{
		fwprintf(stderr, L"WSAEventSelect failed: %d\r\n", WSAGetLastError());
		return FALSE;
	}
	wsaEvents[0] = wsaeServer;

	while (FALSE == fStop)
	{
		dwHandle = WSAWaitForMultipleEvents(_countof(wsaEvents),
			wsaEvents,
			FALSE,
			INFINITE,
			TRUE);
		dwHandle -= WSA_WAIT_EVENT_0;

		switch (dwHandle)
		{
		case 0:
			if (FALSE == HandleNetworkEvent(sServer, wsaeServer, pelcListings))
			{
				fStop = TRUE;
			}
			break;
		case 1:
			fStop = TRUE;
			break;
		}
	}

	return TRUE;
}

BOOL
ServerLoopThreadProc(
	DWORD_PTR *dwpArguments)
{
	SOCKET sServer = (SOCKET)dwpArguments[0];
	PBOOL pfStop = (PBOOL)dwpArguments[1];
	PEMPLOYEE_LISTING_COLLECTION pelcListings = (PEMPLOYEE_LISTING_COLLECTION)dwpArguments[2];

	return ServerLoop(sServer, pfStop, pelcListings);
}