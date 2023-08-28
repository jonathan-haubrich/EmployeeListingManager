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
	BYTE cbLastName = (BYTE)min(strlen(pszFirstName), EL_LASTNAME_MAX_LEN);

	PEMPLOYEE_LISTING pEmployeeListing = NULL;

	// Size of struct is dynamic
	// calculate required size based on length of strings
	SIZE_T cbListingSize = (sizeof(*pEmployeeListing) - 1) + cbFirstName + cbLastName + cbDescription;
	pEmployeeListing = (PEMPLOYEE_LISTING)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,
		cbListingSize);

	pEmployeeListing->bId = bId;
	pEmployeeListing->pelPrev = NULL;
	pEmployeeListing->pelNext = NULL;

	pEmployeeListing->cbListingSize = cbListingSize;

	pEmployeeListing->cbFirstName = cbFirstName;
	RtlCopyMemory(pEmployeeListing->sFirstName, pszFirstName, cbFirstName);
	pEmployeeListing->cbLastName = cbLastName;
	RtlCopyMemory(pEmployeeListing->sLastName, pszLastName, cbLastName);
	pEmployeeListing->cbDescription = cbDescription;
	RtlCopyMemory(pEmployeeListing->sDescription, pszDescription, cbDescription);
	pEmployeeListing->fnFormatter = fnFormatter;

	return pEmployeeListing;
}