
#include "../EmployeeListingManager/EmployeeListing.h"
#include "../EmployeeListingManager/EmployeeListingFormatters.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	CHAR acDescription[] = "TestDescription";
	PEMPLOYEE_LISTING pListing1 = EmployeeListingNew(0x00,
		"TestFirst",
		"TestLast",
		_countof(acDescription),
		acDescription,
		BasicFormatter);

	return 0;
}