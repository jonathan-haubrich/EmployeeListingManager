#ifndef _EMPLOYEELISTINGFORMATTERS_H
#define _EMPLOYEELISTINGFORMATTERS_H

#include <WinSock2.h>
#include <strsafe.h>

#include "EmployeeListing.h"

#define FORMATTER_NAME_MAX_LEN		0x10

#define ELF_NUM_FORMATTERS			3

typedef struct _FORMATTER_ENTRY
{
	CHAR acszName[FORMATTER_NAME_MAX_LEN];
	FN_EMPLOYEEFORMATTER fnFormatter;
} FORMATTER_ENTRY, *PFORMATTER_ENTRY;

extern FORMATTER_ENTRY g_afeFormattersMap[ELF_NUM_FORMATTERS];

VOID BasicFormatter(HANDLE hFile, PEMPLOYEE_LISTING pEmployeeListing);
VOID FancyFormatter(HANDLE hFile, PEMPLOYEE_LISTING pEmployeeListing);
VOID HexFormatter(HANDLE hFile, PEMPLOYEE_LISTING pEmployeeListing);

FN_EMPLOYEEFORMATTER
GetFormatterByName(
	PSTR pszName);

#endif // _EMPLOYEELISTINGFORMATTERS_H