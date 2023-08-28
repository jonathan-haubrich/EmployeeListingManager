#ifndef _EMPLOYEELISTINGCOLLECTION_H
#define _EMPLOYEELISTINGCOLLECTION_H

#include <WinSock2.h>

#include "EmployeeListing.h"

#define ELC_DEFAULT_SIZE	0x1000

#define ELC_NEIGHBOR_LEFT		0x01
#define ELC_NEIGHBOR_RIGHT		0x02
#define ELC_NEIGHBOR_INVALID	0x03

#define ELC_CALC_SLOT_SIZE(current, next)	((PBYTE)next - ((PBYTE)current + current->cbListingSize))

typedef struct _EMPLOYEE_LISTING_COLLECTION
{
	SIZE_T cbListingsSize;
	PBYTE pbListings;
	PEMPLOYEE_LISTING pelFirst;
} EMPLOYEE_LISTING_COLLECTION, *PEMPLOYEE_LISTING_COLLECTION;

PEMPLOYEE_LISTING_COLLECTION
EmployeeListingCollectionNew(
	VOID);

VOID
EmployeeListingCollectionDestroy(
	PEMPLOYEE_LISTING_COLLECTION pelcListings);

SIZE_T
EmployeeListingCollectionGetCapacity(
	PEMPLOYEE_LISTING_COLLECTION pelcListings);

BYTE
EmployeeListingCollectionGetEntries(
	PEMPLOYEE_LISTING_COLLECTION pelcListings);

VOID
EmployeeListingCollectionIncreaseSize(
	PEMPLOYEE_LISTING_COLLECTION pelListing);

PBYTE
EmployeeListingCollectionFindSlot(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	SIZE_T cbRequiredSize);

PEMPLOYEE_LISTING
EmployeeListingCollectionGetNeighbor(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PBYTE pListingSlot,
	INT iNeighbor);

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