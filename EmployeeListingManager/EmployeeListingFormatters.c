#include "EmployeeListingFormatters.h"

VOID OutputStrWithNewline(PSTR pStr, DWORD cbStrLen)
{
	DWORD dwBytesWritten = 0;
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),
		pStr,
		cbStrLen,
		&dwBytesWritten,
		NULL);
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),
		"\n",
		1,
		&dwBytesWritten,
		NULL);
}

VOID BasicFormatter(PEMPLOYEE_LISTING pEmployeeListing)
{
	OutputStrWithNewline(pEmployeeListing->sFirstName, pEmployeeListing->cbFirstName);
	OutputStrWithNewline(pEmployeeListing->sLastName, pEmployeeListing->cbLastName);
	OutputStrWithNewline(pEmployeeListing->sDescription, pEmployeeListing->cbDescription);
}

VOID FancyFormatter(PEMPLOYEE_LISTING pEmployeeListing)
{
	DWORD dwBytesWritten = 0;
	CHAR acszHeaderFooter[] = "=========================";

	OutputStrWithNewline(acszHeaderFooter, _countof(acszHeaderFooter));
	OutputStrWithNewline(pEmployeeListing->sFirstName, pEmployeeListing->cbFirstName);
	OutputStrWithNewline(pEmployeeListing->sLastName, pEmployeeListing->cbLastName);
	OutputStrWithNewline(pEmployeeListing->sDescription, pEmployeeListing->cbDescription);
	OutputStrWithNewline(acszHeaderFooter, _countof(acszHeaderFooter));
}

VOID OutputHexByte(BYTE b)
{
	PSTR pOutput = NULL;
	DWORD dwAllocated = 0;
	DWORD_PTR dwpArguments[] = { b };
	DWORD dwBytesWritten = 0;

	dwAllocated = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_STRING,
		"%1!02X!",
		0,
		0,
		(LPSTR)&pOutput,
		2,
		(va_list *)dwpArguments);
		
	if(0 == dwAllocated)
	{
		CHAR acErrorMessage[] = __FILE__ ":" __FUNCTION__ ": FormatMessage failed";
		WriteFile(GetStdHandle(STD_ERROR_HANDLE),
			acErrorMessage,
			_countof(acErrorMessage),
			&dwBytesWritten,
			NULL);
		return;
	}

	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),
		pOutput,
		dwAllocated,
		&dwBytesWritten,
		NULL);

	LocalFree(pOutput);
}

OutputStrAsHex(PSTR pStr, DWORD cbStrLen)
{
	for (DWORD i = 0; i < cbStrLen; ++i)
	{
		OutputHexByte(pStr[i]);
	}
}

OutputStrAsHexWithNewline(PSTR pStr, DWORD cbStrLen)
{
	DWORD dwBytesWritten = 0;

	OutputStrAsHex(pStr, cbStrLen);

	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),
		"\n",
		1,
		&dwBytesWritten,
		NULL);
}

VOID HexFormatter(PEMPLOYEE_LISTING pEmployeeListing)
{
	OutputStrAsHexWithNewline(pEmployeeListing->sFirstName, pEmployeeListing->cbFirstName);
	OutputStrAsHexWithNewline(pEmployeeListing->sLastName, pEmployeeListing->cbLastName);
	OutputStrAsHexWithNewline(pEmployeeListing->sDescription, pEmployeeListing->cbDescription);
}

extern FORMATTER_ENTRY g_afeFormattersMap[ELF_NUM_FORMATTERS] = {
	{ "basic", BasicFormatter },
	{ "fancy", FancyFormatter },
	{ "hex", HexFormatter}
};