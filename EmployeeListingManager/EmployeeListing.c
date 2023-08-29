#include "EmployeeListing.h"

PEMPLOYEE_LISTING
EmployeeListingNew(
	BYTE bId,
	PSTR pszFirstName,
	PSTR pszLastName,
	BYTE cbDescription,
	PSTR pszDescription,
	FN_EMPLOYEEFORMATTER fnFormatter)
{
	BYTE cbFirstName = (BYTE)min(strlen(pszFirstName), EL_FIRSTNAME_MAX_LEN);
	BYTE cbLastName = (BYTE)min(strlen(pszLastName), EL_LASTNAME_MAX_LEN);

	PEMPLOYEE_LISTING pEmployeeListing = NULL;

	// Size of struct is dynamic
	// calculate required size based on length of strings
	DWORD cbListingSize = sizeof(EMPLOYEE_LISTING_METADATA) + 
		cbFirstName +
		cbLastName +
		cbDescription;

	pEmployeeListing = (PEMPLOYEE_LISTING)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,
		cbListingSize * 2); // allocate twice as much, just to be safe...

	pEmployeeListing->MetaData.bId = bId;
	pEmployeeListing->MetaData.pelPrev = NULL;
	pEmployeeListing->MetaData.pelNext = NULL;

	pEmployeeListing->MetaData.cbListingSize = cbListingSize;
	pEmployeeListing->MetaData.fnFormatter = fnFormatter;
	pEmployeeListing->MetaData.cbFirstName = cbFirstName;
	pEmployeeListing->MetaData.cbLastName = cbLastName;
	pEmployeeListing->MetaData.cbDescription = cbDescription;

	RtlCopyMemory(pEmployeeListing->Data.sFirstName, pszFirstName, cbFirstName);
	RtlCopyMemory(pEmployeeListing->Data.sLastName, pszLastName, cbLastName);
	RtlCopyMemory(pEmployeeListing->Data.sDescription, pszDescription, cbDescription);

	return pEmployeeListing;
}