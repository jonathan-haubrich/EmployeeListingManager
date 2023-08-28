
#include "EmployeeListing.h"
#include "EmployeeListingFormatters.h"
#include "EmployeeListingManager.h"

#include <stdio.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi")

#define DEFAULT_LISTEN_ADDR		L"0.0.0.0"
#define DEFAULT_LISTEN_PORT		L"4444"

VOID
StartCommandListener(
	WSAEVENT wsaeStop);

INT wmain(INT argc, PWSTR argv[])
{
	PWSTR pwszListenAddress = DEFAULT_LISTEN_ADDR;
	PWSTR pwszListenPort = DEFAULT_LISTEN_PORT;
	SOCKET sServer = INVALID_SOCKET;
	BOOL fStop = FALSE;
	HANDLE hServerLoopThread = NULL;
	PEMPLOYEE_LISTING_COLLECTION pelcListings = NULL;

	if (argc > 1)
	{
		if (0 == StrCmpIW(argv[1], L"-h") ||
			0 == StrCmpIW(argv[1], L"-help") ||
			0 == StrCmpIW(argv[1], L"--help"))
		{
			fwprintf(stderr, L"Usage: %s <listen port (default: %s)> <listen addr (default: %s)>\r\n",
				argv[0],
				DEFAULT_LISTEN_PORT,
				DEFAULT_LISTEN_ADDR);
			return EXIT_FAILURE;
		}

		pwszListenPort = argv[1];
	}
	if (argc > 2)
	{
		pwszListenAddress = argv[2];
	}

	WSAEVENT wsaeStop = WSACreateEvent();
	if (NULL != wsaeStop)
	{
		if (FALSE != StartServer(pwszListenAddress, pwszListenPort, &sServer))
		{
			pelcListings = EmployeeListingCollectionNew();
			DWORD_PTR adwpServerLoopArguments[] = {
				(DWORD_PTR)sServer,
				(DWORD_PTR)wsaeStop,
				(DWORD_PTR)pelcListings
			};
			hServerLoopThread = CreateThread(NULL, 0, ServerLoopThreadProc, adwpServerLoopArguments, 0, NULL);
			StartCommandListener(wsaeStop);

			WaitForSingleObject(hServerLoopThread, INFINITE);

			StopServer(sServer);
			EmployeeListingCollectionDestroy(pelcListings);
		}
	}
}

VOID
StartCommandListener(
	WSAEVENT wsaeStop)
{
	CHAR acCommandBuffer[0x100] = { 0 };
	DWORD dwBytesRead = 0;

	printf("Type \"stop\" or \"quit\" to stop server\r\n");
	while (0 != StrCmpIA(acCommandBuffer, "stop") &&
		0 != StrCmpIA(acCommandBuffer, "quit"))
	{
		dwBytesRead = 0;
		SecureZeroMemory(acCommandBuffer, sizeof(acCommandBuffer));

		if (FALSE == ReadFile(GetStdHandle(STD_INPUT_HANDLE),
			acCommandBuffer,
			sizeof(acCommandBuffer) - 1,
			&dwBytesRead,
			NULL))
		{
			fwprintf(stderr, L"StartCommandListener:ReadFile failed: %d\r\n", GetLastError());
			break;
		}

		acCommandBuffer[dwBytesRead - 2] = '\0';
		acCommandBuffer[dwBytesRead - 1] = '\0';
	}

	WSASetEvent(wsaeStop);
}