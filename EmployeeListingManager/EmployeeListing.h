#ifndef _EMPLOYEELISTING_H
#define _EMPLOYEELISTING_H

#include <WinSock2.h>

#define EL_FIRSTNAME_MAX_LEN	0x10
#define EL_LASTNAME_MAX_LEN		0x10
#define EL_DESCRIPTION_MAX_LEN	0xFF	// BYTE_MAX

struct _EMPLOYEE_LISTING;

typedef void(*FN_EMPLOYEEFORMATTER)(HANDLE, struct _EMPLOYEE_LISTING*);

#pragma pack(push, 1)

typedef struct _EMPLOYEE_LISTING_METADATA
{
	BYTE bId;
	DWORD cbListingSize;
	BYTE cbFirstName;
	BYTE cbLastName;
	BYTE cbDescription;
	struct _EMPLOYEE_LISTING* pelPrev;
	struct _EMPLOYEE_LISTING* pelNext;
	FN_EMPLOYEEFORMATTER fnFormatter;
} EMPLOYEE_LISTING_METADATA, *PEMPLOYEE_LISTING_METADATA;

typedef struct _EMPLOYEE_LISTING_DATA
{
	CHAR sFirstName[EL_FIRSTNAME_MAX_LEN];
	CHAR sLastName[EL_LASTNAME_MAX_LEN];
	CHAR sDescription[1];	// Descriptions can be long so they are variable length
} EMPLOYEE_LISTING_DATA, * PEMPLOYEE_LISTING_DATA;

#define EMPLOYEE_LISTING_DATA_SIZE	(sizeof(EMPLOYEE_LISTING_DATA) - 1)

typedef struct _EMPLOYEE_LISTING {
	EMPLOYEE_LISTING_METADATA MetaData;
	EMPLOYEE_LISTING_DATA Data;
} EMPLOYEE_LISTING, *PEMPLOYEE_LISTING;

#pragma pack(pop)

PEMPLOYEE_LISTING
EmployeeListingNew(
	BYTE bId,
	PSTR pszFirstName,
	PSTR pszLastName,
	BYTE cbDescription,
	PSTR pszDescription,
	FN_EMPLOYEEFORMATTER fnFormatter);

#endif // _EMPLOYEELISTING_H