#ifndef _EMPLOYEELISTINGMANAGER_H
#define _EMPLOYEELISTINGMANAGER_H

#include <Windows.h>

#include "EmployeeListing.h"
#include "EmployeeListingFormatters.h"

typedef struct _LISTING_COLLECTION
{
	BYTE cNumListings;
	PBYTE pbListings;
} LISTING_COLLECTION, *PLISTING_COLLECTION;

#endif // _EMPLOYEELISTINGMANAGER_H