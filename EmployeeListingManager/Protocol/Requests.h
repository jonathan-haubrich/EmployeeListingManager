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

typedef struct _REQUEST_ADD_USER_METADATA
{
	BYTE bId;
	DWORD cbListingSize;
	BYTE cbFormatter;
	BYTE cbFirstName;
	BYTE cbLastName;
	BYTE cbDescription;
} REQUEST_ADD_USER_METADATA, *PREQUEST_ADD_USER_METADATA;

typedef struct _REQUEST_ADD_USER_DATA
{
	CHAR sFormatter[FORMATTER_NAME_MAX_LEN];
	CHAR sFirstName[EL_FIRSTNAME_MAX_LEN];
	CHAR sLastName[EL_LASTNAME_MAX_LEN];
	CHAR sDescription[1];	// Descriptions can be long so they are variable length
} REQUEST_ADD_USER_DATA, * PREQUEST_ADD_USER_DATA;

#define REQUEST_ADD_USER_DATA_SIZE	(sizeof(REQUEST_ADD_USER_DATA) - 1)

typedef struct _REQUEST_ADD_USER
{
	REQUEST_ADD_USER_METADATA MetaData;
	REQUEST_ADD_USER_DATA Data;
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