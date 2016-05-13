#include <Windows.h>
#include "curses.h"
#include "coord.h"
#include "packetlog.h"
#include "DebugStr.h"

void		*DetourFunc(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len + 5);
	DWORD dwback;
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);
	memcpy(jmp, src, len);	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	VirtualProtect(src, len, dwback, &dwback);
	return (jmp - len);
}

void		initDebug()
{
	initPacketLog();
	initDebugStr();
	DetourFunc((BYTE *)0x0046A3EA, (BYTE *)hkCoord, 5);
}