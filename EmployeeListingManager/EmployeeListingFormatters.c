#include "EmployeeListingFormatters.h"

VOID OutputStrWithNewline(HANDLE hFile, PSTR pStr, DWORD cbStrLen)
{
	DWORD dwBytesWritten = 0;
	WriteFile(hFile,
		pStr,
		cbStrLen,
		&dwBytesWritten,
		NULL);
	WriteFile(hFile,
		"\n",
		1,
		&dwBytesWritten,
		NULL);
}

VOID BasicFormatter(HANDLE hFile, PEMPLOYEE_LISTING pEmployeeListing)
{
	OutputStrWithNewline(hFile, pEmployeeListing->Data.sFirstName, pEmployeeListing->MetaData.cbFirstName);
	OutputStrWithNewline(hFile, pEmployeeListing->Data.sLastName, pEmployeeListing->MetaData.cbLastName);
	OutputStrWithNewline(hFile, pEmployeeListing->Data.sDescription, pEmployeeListing->MetaData.cbDescription);
}

VOID FancyFormatter(HANDLE hFile, PEMPLOYEE_LISTING pEmployeeListing)
{
	DWORD dwBytesWritten = 0;
	CHAR acszHeaderFooter[] = "=========================";

	OutputStrWithNewline(hFile, acszHeaderFooter, _countof(acszHeaderFooter)-1);
	OutputStrWithNewline(hFile, pEmployeeListing->Data.sFirstName, pEmployeeListing->MetaData.cbFirstName);
	OutputStrWithNewline(hFile, pEmployeeListing->Data.sLastName, pEmployeeListing->MetaData.cbLastName);
	OutputStrWithNewline(hFile, pEmployeeListing->Data.sDescription, pEmployeeListing->MetaData.cbDescription);
	OutputStrWithNewline(hFile, acszHeaderFooter, _countof(acszHeaderFooter)-1);
}

VOID OutputHexByte(HANDLE hFile, BYTE b)
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

	WriteFile(hFile,
		pOutput,
		dwAllocated,
		&dwBytesWritten,
		NULL);

	LocalFree(pOutput);
}

OutputStrAsHex(HANDLE hFile, PSTR pStr, DWORD cbStrLen)
{
	for (DWORD i = 0; i < cbStrLen; ++i)
	{
		OutputHexByte(hFile, pStr[i]);
	}
}

OutputStrAsHexWithNewline(HANDLE hFile, PSTR pStr, DWORD cbStrLen)
{
	DWORD dwBytesWritten = 0;

	OutputStrAsHex(hFile, pStr, cbStrLen);

	WriteFile(hFile,
		"\n",
		1,
		&dwBytesWritten,
		NULL);
}

VOID HexFormatter(HANDLE hFile, PEMPLOYEE_LISTING pEmployeeListing)
{
	OutputStrAsHexWithNewline(hFile, pEmployeeListing->Data.sFirstName, pEmployeeListing->MetaData.cbFirstName);
	OutputStrAsHexWithNewline(hFile, pEmployeeListing->Data.sLastName, pEmployeeListing->MetaData.cbLastName);
	OutputStrAsHexWithNewline(hFile, pEmployeeListing->Data.sDescription, pEmployeeListing->MetaData.cbDescription);
}

extern FORMATTER_ENTRY g_afeFormattersMap[ELF_NUM_FORMATTERS] = {
	{ "basic", BasicFormatter },
	{ "fancy", FancyFormatter },
	{ "hex", HexFormatter }
};

FN_EMPLOYEEFORMATTER
GetFormatterByName(
	PSTR pszName)
{
	SIZE_T cchNameLen = 0;
	SIZE_T cchEntryLen = 0;
	if (S_OK != StringCchLengthA(pszName, STRSAFE_MAX_LENGTH, &cchNameLen))
	{
		return NULL;
	}

	for (DWORD dwFormatter = 0;
		dwFormatter < _countof(g_afeFormattersMap);
		++dwFormatter)
	{
		if (S_OK != StringCchLengthA(g_afeFormattersMap[dwFormatter].acszName,
			STRSAFE_MAX_LENGTH,
			&cchEntryLen))
		{
			return NULL;
		}

		if (CSTR_EQUAL == CompareStringEx(LOCALE_NAME_SYSTEM_DEFAULT,
			NORM_IGNORECASE,
			(LPCWCH)pszName,
			(INT)cchNameLen,
			(LPCWCH)g_afeFormattersMap[dwFormatter].acszName,
			(INT)cchEntryLen,
			NULL,
			NULL,
			0))
		{
			return g_afeFormattersMap[dwFormatter].fnFormatter;
		}
	}

	return NULL;
}