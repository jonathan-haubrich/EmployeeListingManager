#ifndef _RESPONSES_H
#define _RESPONSES_H

#include <WinSock2.h>

#include "../EmployeeListing.h"
#include "../EmployeeListingFormatters.h"

enum ELM_RESPONSE_CODES
{
	ELM_RESPONSE_SUCCESS = 0,
	ELM_RESPONSE_UNKNOWN_USER,
	ELM_RESPONSE_USER_ALREADY_EXISTS,
	ELM_RESPONSE_INVALID_FORMATTER,
	ELM_RESPONSE_INVALID_COMMAND = 0xFE,
	ELM_RESPONSE_FAILURE = 0xFF
};

#pragma pack(push, 1)

typedef struct _RESPONSE_HEADER
{
	BYTE	bResponseCode;
} RESPONSE_HEADER, * PRESPONSE_HEADER;

typedef struct _RESPONSE_DATA
{
	BYTE cPayloadSize;
	BYTE dwNumItems;
	BYTE abPayload[1];
} RESPONSE_DATA, *PRESPONSE_DATA;

typedef struct _RESPONSE_HEADERPAYLOAD
{
	RESPONSE_HEADER rhHeader;
	RESPONSE_DATA rdPayload;
} RESPONSE_HEADERPAYLOAD, * PRESPONSE_HEADERPAYLOAD;

// ELM_REQUEST_ADD_USER
// returns SUCCESS or FAILURE

// ELM_REQUEST_REMOVE_USER
// returns SUCCESS or FAILURE

// ELM_REQUEST_LIST_USERS
// returns an array of BYTES, one BYTE for each listing id

// ELM_REQUEST_GET_USER
// returns an array of RESPONSE_GET_USER
typedef struct _RESPONSE_GET_USER
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
} RESPONSE_GET_USER, *PRESPONSE_GET_USER;

// ELM_REQUEST_DISPLAY_USER
// returns the output of the formatter specified for the listing
// due to the nature of the formatters, this is stream buffered and unstructured
// output end is indicated by the closing of the network socket

#pragma pack(pop)

#endif // _RESPONSES_H