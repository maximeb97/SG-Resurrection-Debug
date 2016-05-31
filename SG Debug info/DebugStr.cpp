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
char						**blacklist = 0;

BOOL WINAPI					MyIsDebuggerPresent(void)
{
	return (TRUE);
}

void WINAPI					MyOutputDebugString(LPCTSTR lpOutputString)
{
	int						i;
	int						j;
	char					*tmp;
	char					*tmp2;

	_asm pushad;
	if (debuglogs != 0)
	{
		fprintf(debuglogs, lpOutputString);
		fprintf(debuglogs, "\n");
	}
	j = 0;
	while (blacklist[j] != 0)
	{
		if (strstr(lpOutputString, blacklist[j]) != 0 || strncmp(lpOutputString, blacklist[j], strlen(blacklist[j])) == 0)
		{
			_asm popad;
			return;
		}
		j = j + 1;
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

void						initBlacklist()
{
	FILE					*fd;
	int						i;
	char					buf[256];
	int						len;

	i = 0;
	if ((fd = fopen("dbg_blacklist.txt", "r")) != 0)
	{
		while (fgets(buf, sizeof(buf), fd) != 0)
			i = i + 1;
		fclose(fd);
		blacklist = (char **)malloc(sizeof(char *) * i + 1);
		blacklist[i] = 0;
		fd = fopen("dbg_blacklist.txt", "r");
		i = 0;
		while (fgets(buf, sizeof(buf), fd) != 0)
		{
			buf[strlen(buf) - 1] = 0;
			blacklist[i] = strdup(buf);
			i = i + 1;
		}
		fclose(fd);
	}
}

void						initDebugStr()
{
	DWORD					dwIsDebuggerPresent;
	DWORD					dwOutputDebugString;

	if (debuglogs == 0)
		debuglogs = fopen("DebugLog.txt", "w+");
	initBlacklist();
	dwIsDebuggerPresent = (DWORD)GetProcAddress(LoadLibraryA("Kernel32.dll"), "IsDebuggerPresent");
	dwOutputDebugString = (DWORD)GetProcAddress(LoadLibraryA("Kernel32.dll"), "OutputDebugStringA");
	DetourFunction((PBYTE)dwIsDebuggerPresent, (PBYTE)MyIsDebuggerPresent);
	DetourFunction((PBYTE)dwOutputDebugString, (PBYTE)MyOutputDebugString);
	debugstr = (char **)malloc(sizeof(char *) * (C_HEIGHT));
	debugstr[0] = 0;
}
