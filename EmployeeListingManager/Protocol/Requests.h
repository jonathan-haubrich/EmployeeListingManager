#ifndef _REQUESTS_H
#define _REQUESTS_H

#include <WinSock2.h>

#include "../EmployeeListing.h"
#include "../EmployeeListingFormatters.h"

enum ELM_REQUEST_TYPES
{
	ELM_REQUEST_ADD_USER = 0,
	ELM_REQUEST_REMOVE_USER,
	ELM_REQUEST_LIST_USERS,
	ELM_REQUEST_GET_USER,
	ELM_REQUEST_DISPLAY_USER
};

#pragma pack(push, 1)

typedef struct _REQUEST_HEADER
{
	BYTE	bCommandType;
} REQUEST_HEADER, *PREQUEST_HEADER;

typedef struct _REQUEST_ADD_USER
{
	BYTE bId;
	SIZE_T cbListingSize;
	BYTE cbFormatter;
	CHAR sFormatter[FORMATTER_NAME_MAX_LEN];
	BYTE cbFirstName;
	CHAR sFirstName[EL_FIRSTNAME_MAX_LEN];
	BYTE cbLastName;
	CHAR sLastName[EL_LASTNAME_MAX_LEN];
	BYTE cbDescription;
	CHAR sDescription[1];	// Descriptions can be long so they are variable length
} REQUEST_ADD_USER, *PREQUEST_ADD_USER;

typedef struct _REQUEST_REMOVE_USER
{
	BYTE bId;
} REQUEST_REMOVE_USER, *PREQUEST_REMOVE_USER;

typedef struct _REQUEST_GET_USER
{
	BYTE bId;
} REQUEST_GET_USER, *PREQUEST_GET_USER;

typedef struct _REQUEST_DISPLAY_USER
{
	BYTE bId;
} REQUEST_DISPLAY_USER, *PREQUEST_DISPLAY_USER;

#pragma pack(pop)

#endif // _REQUESTS_H