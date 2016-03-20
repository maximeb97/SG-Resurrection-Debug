#include <Windows.h>
#include "curses.h"
#include "debug.h"

void			initCurses()
{
	if (!AllocConsole())
	{
		FreeConsole();
		Sleep(100);
		AllocConsole();
	}
	SetConsoleTitleA("Street Gears Resurrection Debug");
	initscr();
	noecho();
}

int				MainThread()
{
	LPSTR		cmdLine;

	cmdLine = GetCommandLineA();
	if (strstr(cmdLine, "/debug") > 0)
	{
		initCurses();
		initDebug();
		while (1)
		{
			print_PlayerPos();
			print_PacketInfo();
			refresh();
			Sleep(100);
		}
		endwin();
	}
	return (0);
}

BOOL WINAPI		DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0);
	return true;
}