#ifndef _EMPLOYEELISTINGCOLLECTION_H
#define _EMPLOYEELISTINGCOLLECTION_H

#include <Windows.h>

#include "EmployeeListing.h"

#define ELC_DEFAULT_SIZE	0x1000

#define ELC_CALC_SLOT_SIZE(current, next)	((PBYTE)next - ((PBYTE)current + current->cbListingSize))

typedef struct _EMPLOYEE_LISTING_COLLECTION
{
	DWORD cEntries;
	SIZE_T cbListingsSize;
	PBYTE pbListings;
	PEMPLOYEE_LISTING pelFirst;
} EMPLOYEE_LISTING_COLLECTION, *PEMPLOYEE_LISTING_COLLECTION;

PEMPLOYEE_LISTING_COLLECTION
EmployeeListingCollectionNew(
	VOID);

SIZE_T
EmployeeListingCollectionGetCapacity(
	PEMPLOYEE_LISTING_COLLECTION pelListings);

VOID
EmployeeListingCollectionIncreaseSize(
	PEMPLOYEE_LISTING_COLLECTION pelListing);

PBYTE
EmployeeListingCollectionFindSlot(
	PEMPLOYEE_LISTING_COLLECTION pelListings,
	SIZE_T cbRequiredSize);

PEMPLOYEE_LISTING
EmployeeListingCollectionGetPredecessor(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PBYTE pListingSlot);

PEMPLOYEE_LISTING
EmployeeListingCollectionGetSuccessor(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PBYTE pListingSlot);

VOID
EmployeeListingCollectionAddListing(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PEMPLOYEE_LISTING pelListing);

VOID
EmployeeListingCollectionRemoveListing(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PEMPLOYEE_LISTING pelListing);

PEMPLOYEE_LISTING
EmployeeListingCollectionGetListingById(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	BYTE bId);

#endif // _EMPLOYEELISTINGCOLLECTION_H