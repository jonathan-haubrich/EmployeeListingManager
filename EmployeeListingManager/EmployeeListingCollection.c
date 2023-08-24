#include "EmployeeListingCollection.h"

PEMPLOYEE_LISTING_COLLECTION
EmployeeListingCollectionNew(
	VOID)
{
	PEMPLOYEE_LISTING_COLLECTION pelcListings = NULL;

	pelcListings = (PEMPLOYEE_LISTING_COLLECTION)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,
		sizeof(*pelcListings));

	pelcListings->cEntries = 0;
	pelcListings->cbListingsSize = ELC_DEFAULT_SIZE;
	pelcListings->pbListings = (PBYTE)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,
		ELC_DEFAULT_SIZE);

	pelcListings->pelFirst = NULL;

	return pelcListings;
}

SIZE_T
EmployeeListingCollectionGetCapacity(
	PEMPLOYEE_LISTING_COLLECTION pelListings)
{
	SIZE_T cRemaining = pelListings->cbListingsSize;
	PEMPLOYEE_LISTING pCursor = pelListings->pelFirst;

	while (NULL != pCursor)
	{
		cRemaining -= pCursor->cbListingSize;
		pCursor = pCursor->pelNext;
	}

	return cRemaining;
}

VOID
EmployeeListingCollectionIncreaseSize(
	PEMPLOYEE_LISTING_COLLECTION pelListing)
{
	// this should always be sufficient for any listing
	// max size of listing is < 512 bytes
	// we start listing at 0x1000 so any increase exceeds max size of a listing
	pelListing->cbListingsSize *= 2;

	pelListing->pbListings = (PBYTE)HeapReAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,
		pelListing->pbListings,
		pelListing->cbListingsSize);
}

PBYTE
EmployeeListingCollectionFindSlot(
	PEMPLOYEE_LISTING_COLLECTION pelListings,
	SIZE_T cbRequiredSize)
{
	PBYTE pCursor = pelListings->pbListings;
	PBYTE pNext = (PBYTE)pelListings->pelFirst;
	PBYTE pEnd = &pelListings->pbListings[pelListings->cbListingsSize - 1];
	SIZE_T cbSlotSize = 0;

	while (NULL != pCursor &&
		NULL != pNext &&
		pEnd > pCursor)
	{
		cbSlotSize = pNext - pCursor;
		if (cbSlotSize >= cbRequiredSize)
		{
			return pCursor;
		}

		pCursor = pNext + ((PEMPLOYEE_LISTING)pNext)->cbListingSize;
		pNext = (PBYTE)((PEMPLOYEE_LISTING)pNext)->pelNext;
	}

	// if we got here, pNext is NULL
	// the slot is the block of memory following last listing
	// i.e. the remaining memory in pbListings
	if (NULL == pNext)
	{
		pNext = pEnd;
		cbSlotSize = pNext - pCursor;
		if (cbSlotSize < cbRequiredSize)
		{
			EmployeeListingCollectionIncreaseSize(pelListings);
		}
	}

	return pCursor;
}

PEMPLOYEE_LISTING
EmployeeListingCollectionGetNeighbor(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PBYTE pListingSlot,
	INT iNeighbor)
{
	PEMPLOYEE_LISTING pCursor = pelcListings->pelFirst;
	PEMPLOYEE_LISTING pPrev = NULL;

	while (NULL != pCursor &&
		pListingSlot > (PBYTE)pCursor)
	{
		pPrev = pCursor;
		pCursor = pCursor->pelNext;
	}

	if (ELC_NEIGHBOR_LEFT == iNeighbor)
	{
		return pPrev;
	}
	else if (ELC_NEIGHBOR_RIGHT == iNeighbor)
	{
		return pCursor;
	}

	// should never get here
	__fastfail(ELC_NEIGHBOR_INVALID);
	// return to satisfy warning
	return NULL;
}

PEMPLOYEE_LISTING
EmployeeListingCollectionGetPredecessor(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PBYTE pListingSlot)
{
	return EmployeeListingCollectionGetNeighbor(pelcListings,
		pListingSlot,
		ELC_NEIGHBOR_LEFT);
}

PEMPLOYEE_LISTING
EmployeeListingCollectionGetSuccessor(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PBYTE pListingSlot)
{
	return EmployeeListingCollectionGetNeighbor(pelcListings,
		pListingSlot,
		ELC_NEIGHBOR_RIGHT);
}

VOID
EmployeeListingCollectionAddListing(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PEMPLOYEE_LISTING pelListing)
{
	PBYTE pListingSlot = EmployeeListingCollectionFindSlot(pelcListings,
		pelListing->cbListingSize);

	if (NULL == pelcListings->pelFirst)
	{
		pelcListings->pelFirst = (PEMPLOYEE_LISTING)pListingSlot;
	}
	else
	{
		pelListing->pelPrev = EmployeeListingCollectionGetPredecessor(pelcListings, pListingSlot);
		pelListing->pelNext = EmployeeListingCollectionGetSuccessor(pelcListings, pListingSlot);

		if (NULL != pelListing->pelPrev)
		{
			pelListing->pelPrev->pelNext = (PEMPLOYEE_LISTING)pListingSlot;
		}

		if (NULL != pelListing->pelNext)
		{
			pelListing->pelNext->pelPrev = (PEMPLOYEE_LISTING)pListingSlot;
		}
	}

	RtlCopyMemory(pListingSlot, pelListing, pelListing->cbListingSize);
}

VOID
EmployeeListingCollectionRemoveListing(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PEMPLOYEE_LISTING pelListing)
{
	if (NULL == pelListing->pelPrev)
	{
		// listing is the first
		pelcListings->pelFirst = pelListing->pelNext;
		if (NULL != pelcListings->pelFirst->pelNext)
		{
			pelcListings->pelFirst->pelPrev = pelcListings->pelFirst;
		}
		return;
	}

	pelListing->pelPrev->pelNext = pelListing->pelNext;

	// only need to set next's prev if listing isn't the last
	if (NULL != pelListing->pelNext)
	{
		pelListing->pelNext->pelPrev = pelListing->pelPrev;
	}
}

PEMPLOYEE_LISTING
EmployeeListingCollectionGetListingById(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	BYTE bId)
{
	PEMPLOYEE_LISTING pCursor = pelcListings->pelFirst;

	while (NULL != pCursor)
	{
		if (bId == pCursor->bId)
		{
			return pCursor;
		}

		pCursor = pCursor->pelNext;
	}

	// id not found
	return NULL;
}