#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "detours.h"
#include "DebugStr.h"
#include "printing.h"

#pragma comment(lib, "detours.lib")
#pragma warning(disable: 4996)

FILE						*debuglogs = 0;
char						**debugstr = 0;

BOOL WINAPI					MyIsDebuggerPresent(void)
{
	return (TRUE);
}

void WINAPI					MyOutputDebugString(LPCTSTR lpOutputString)
{
	int						i;
	char					*tmp;
	char					*tmp2;

	_asm pushad;
	if (debuglogs != 0)
	{
		fprintf(debuglogs, lpOutputString);
		fprintf(debuglogs, "\n");
	}
	tmp = debugstr[0];
	debugstr[0] = strdup(lpOutputString);
	i = 1;
	while (i != C_HEIGHT - 9)
	{
		tmp2 = debugstr[i];
		debugstr[i] = tmp;
		tmp = tmp2;
		i = i + 1;
	}
	if (tmp != 0)
		free(tmp);
	_asm popad;
}

char						**get_debug()
{
	return (debugstr);
}

void						initDebugStr()
{
	DWORD					dwIsDebuggerPresent;
	DWORD					dwOutputDebugString;

	if (debuglogs == 0)
		debuglogs = fopen("DebugLog.txt", "w+");
	dwIsDebuggerPresent = (DWORD)GetProcAddress(LoadLibraryA("Kernel32.dll"), "IsDebuggerPresent");
	dwOutputDebugString = (DWORD)GetProcAddress(LoadLibraryA("Kernel32.dll"), "OutputDebugStringA");
	DetourFunction((PBYTE)dwIsDebuggerPresent, (PBYTE)MyIsDebuggerPresent);
	DetourFunction((PBYTE)dwOutputDebugString, (PBYTE)MyOutputDebugString);
	debugstr = (char **)malloc(sizeof(char *) * (C_HEIGHT));
	debugstr[0] = 0;
}
