#include <Windows.h>
#include <stdio.h>

#include "EmployeeListing.h"
#include "EmployeeListingFormatters.h"

int main()
{
	PFORMATTER_ENTRY pfeFormatter = NULL;
	for (int i = 0; i < _countof(g_afeFormattersMap); ++i)
	{
		pfeFormatter = &g_afeFormattersMap[i];
		printf("%s\n", pfeFormatter->acszName);
	}
}