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
	// sanity checks
	if (NULL == pelListings->pelFirst)
	{
		// collection is empty
		return pelListings->pbListings;
	}

	SIZE_T cbRemaining = EmployeeListingCollectionGetCapacity(pelListings);
	if (cbRemaining < cbRequiredSize)
	{
		EmployeeListingCollectionIncreaseSize(pelListings);
	}

	// iterare over listings, looking for an opening or end of listing
	// NOTE: cRemaining may be fragmented, so we might not have a slot big enough

	SIZE_T cbSlotSize = 0;
	PEMPLOYEE_LISTING pCursor = pelListings->pelFirst;
	while (pCursor && pCursor->pelNext)
	{
		// slot size is pelNext - current
		cbSlotSize = ELC_CALC_SLOT_SIZE(pCursor, pCursor->pelNext);
		if (cbSlotSize >= cbRequiredSize)
		{
			return (PBYTE)pCursor + pCursor->cbListingSize;
		}

		pCursor = pCursor->pelNext;
	}

	// if we're here, we're at the last listing
	// the slot size is end of listing to the end of the memory area
	if (pCursor)
	{
		PBYTE pLastListing = (PBYTE)pCursor + pCursor->cbListingSize;
		PBYTE pEnd = &pelListings->pbListings[pelListings->cbListingsSize - 1];
		cbSlotSize = pEnd - pLastListing;

		// see if remaining memory is sufficient
		// could be insufficient due to fragmenting
		if (cbSlotSize < cbRequiredSize)
		{
			EmployeeListingCollectionIncreaseSize(pelListings);
		}

		// either memory was too fragmented or the last slot was sufficient
		// either way pLastListing is sufficient
		return pLastListing;
	}

	// should not get here
	__fastfail(244);
	return NULL;
}

PEMPLOYEE_LISTING
EmployeeListingCollectionGetPredecessor(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PBYTE pListingSlot)
{
	PEMPLOYEE_LISTING pCursor = pelcListings->pelFirst;
	while (NULL != pCursor &&
		NULL != pCursor->pelNext)
	{
		PBYTE stStart = (PBYTE)pCursor + pCursor->cbListingSize;
		PBYTE stEnd = (PBYTE)pCursor->pelNext;
		if (pListingSlot > stStart && 
			(NULL == stEnd || pListingSlot < stEnd))
		{
			break;
		}

		pCursor = pCursor->pelNext;
	}

	return pCursor;
}

PEMPLOYEE_LISTING
EmployeeListingCollectionGetSuccessor(
	PEMPLOYEE_LISTING_COLLECTION pelcListings,
	PBYTE pListingSlot)
{
	PEMPLOYEE_LISTING pCursor = pelcListings->pelFirst;
	while (NULL != pCursor &&
		NULL != pCursor->pelNext)
	{
		PBYTE stStart = (PBYTE)pCursor + pCursor->cbListingSize;
		PBYTE stEnd = (PBYTE)pCursor->pelNext;
		if (pListingSlot > stStart &&
			(NULL == stEnd || pListingSlot < stEnd))
		{
			break;
		}

		pCursor = pCursor->pelNext;
	}

	if (NULL != pCursor)
	{
		return pCursor->pelNext;
	}

	return NULL;
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