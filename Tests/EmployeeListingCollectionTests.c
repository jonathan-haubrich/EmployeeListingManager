
#include "../EmployeeListingManager/EmployeeListing.h"
#include "../EmployeeListingManager/EmployeeListingCollection.h"
#include "../EmployeeListingManager/EmployeeListingFormatters.h"

#include <stdio.h>

int main()
{
	CHAR acDescription1[] = "Test1Description";
	PEMPLOYEE_LISTING pListing1 = EmployeeListingNew(0x00,
		"Test1First",
		"Test1Last",
		_countof(acDescription1),
		acDescription1,
		BasicFormatter);
	CHAR acDescription2[] = "Test2Description";
	PEMPLOYEE_LISTING pListing2 = EmployeeListingNew(0x01,
		"Test2First",
		"Test2Last",
		_countof(acDescription2),
		acDescription2,
		FancyFormatter);
	CHAR acDescription3[] = "Test3Description";
	PEMPLOYEE_LISTING pListing3 = EmployeeListingNew(0x02,
		"Test3First",
		"Test3Last",
		_countof(acDescription3),
		acDescription3,
		HexFormatter);

	BasicFormatter(pListing1);
	FancyFormatter(pListing2);
	HexFormatter(pListing3);

	PEMPLOYEE_LISTING_COLLECTION pelcListings = EmployeeListingCollectionNew();

	EmployeeListingCollectionAddListing(pelcListings, pListing1);
	EmployeeListingCollectionAddListing(pelcListings, pListing2);
	EmployeeListingCollectionAddListing(pelcListings, pListing3);

	if (pelcListings->pelFirst->bId != 0x00)
	{
		fprintf(stderr, "EmployeeListingCollectionAddListing failed: pListing1");
		return 1;
	}
	if (pelcListings->pelFirst->pelNext->bId != 0x01)
	{
		fprintf(stderr, "EmployeeListingCollectionAddListing failed: pListing2");
		return 1;
	}
	if (pelcListings->pelFirst->pelNext->pelNext->bId != 0x02)
	{
		fprintf(stderr, "EmployeeListingCollectionAddListing failed: pListing3");
		return 1;
	}

	HeapFree(GetProcessHeap(), 0, pListing1);
	HeapFree(GetProcessHeap(), 0, pListing2);
	HeapFree(GetProcessHeap(), 0, pListing3);

	pListing1 = EmployeeListingCollectionGetListingById(pelcListings, 0x01);
	HexFormatter(pListing1);

	EmployeeListingCollectionRemoveListing(pelcListings, pListing1);
	if (pelcListings->pelFirst->bId != 0x00 ||
		pelcListings->pelFirst->pelNext->bId != 0x02)
	{
		fprintf(stderr, "EmployeeListingCollectionRemoveListing failed");
		return 1;
	}

	CHAR acDescription4[] = "Test4";
	PEMPLOYEE_LISTING pListing4 = EmployeeListingNew(0xFF,
		"Test4First",
		"Test4Last",
		_countof(acDescription4),
		acDescription4,
		HexFormatter);
	EmployeeListingCollectionAddListing(pelcListings, pListing4);
	if (pelcListings->pelFirst->bId != 0x00 ||
		pelcListings->pelFirst->pelNext->bId != 0xFF ||
		pelcListings->pelFirst->pelNext->pelNext->bId != 0x02)
	{
		fprintf(stderr, "EmployeeListingCollectionRemoveListing failed");
		return 1;
	}

	HeapFree(GetProcessHeap(), 0, pListing4);
	pListing4 = EmployeeListingCollectionGetListingById(pelcListings, 0xFF);

	printf("pListing4: %08x pListing4->plePrev: %08x pListing4->pleNext: %08x",
		pListing4,
		pListing4->pelPrev,
		pListing4->pelNext);

	return 0;
}