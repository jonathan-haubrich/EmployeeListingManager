#ifndef _EMPLOYEELISTINGMANAGER_H
#define _EMPLOYEELISTINGMANAGER_H

#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")

#include "EmployeeListing.h"
#include "EmployeeListingCollection.h"
#include "EmployeeListingFormatters.h"

#include "Protocol/Requests.h"
#include "Protocol/Responses.h"
#include "Network/Receive.h"
#include "Network/Send.h"

BOOL
StartServer(
	PWSTR pwszIpAddress,
	PWSTR pwszPort,
	SOCKET* psServer);

VOID
StopServer(
	SOCKET sServer);

BOOL
HandleRequestAddUser(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings);

BOOL
HandleRequestRemoveUser(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings);

BOOL
HandleRequestListUsers(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings);

BOOL
HandleRequestDisplayUser(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings);

BOOL
HandleConnection(
	SOCKET sRemote,
	PEMPLOYEE_LISTING_COLLECTION pelcListings);

BOOL
ServerLoop(
	SOCKET sServer,
	PBOOL pfStop,
	PEMPLOYEE_LISTING_COLLECTION pelcListings);

BOOL
ServerLoopThreadProc(
	DWORD_PTR* dwpArguments);

#endif // _EMPLOYEELISTINGMANAGER_H