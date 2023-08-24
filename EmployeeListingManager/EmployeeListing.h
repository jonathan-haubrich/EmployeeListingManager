#ifndef _EMPLOYEELISTING_H
#define _EMPLOYEELISTING_H

#include <Windows.h>

#define EL_FIRSTNAME_MAX_LEN	0x10
#define EL_LASTNAME_MAX_LEN		0x10

typedef struct _EMPLOYEE_LISTING {
	BYTE bId;
	struct _EMPLOYEE_LISTING *pelPrev;
	struct _EMPLOYEE_LISTING* pelNext;
	void(*fnFormatter)(struct _EMPLOYEE_LISTING*);
	SIZE_T cbListingSize;
	BYTE cbFirstName;
	CHAR sFirstName[EL_FIRSTNAME_MAX_LEN];
	BYTE cbLastName;
	CHAR sLastName[EL_LASTNAME_MAX_LEN];
	BYTE cbDescription;
	CHAR sDescription[1];	// Descriptions can be long so they are variable length
} EMPLOYEE_LISTING, *PEMPLOYEE_LISTING;

typedef void(*FN_EMPLOYEEFORMATTER)(PEMPLOYEE_LISTING);

PEMPLOYEE_LISTING
EmployeeListingNew(
	BYTE bId,
	PSTR pszFirstName,
	PSTR pszLastName,
	BYTE cbDescription,
	PSTR pszDescription,
	FN_EMPLOYEEFORMATTER fnFormatter);

#endif // _EMPLOYEELISTING_H