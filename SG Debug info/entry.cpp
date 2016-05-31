#include <Windows.h>
#include "curses.h"
#include "debug.h"
#include "printing.h"


void			initCurses()
{
	WINDOW		*w;

	if (!AllocConsole())
	{
		FreeConsole();
		Sleep(100);
		AllocConsole();
	}
	system("mode 120,45");
	SetConsoleTitleA("Street Gears Resurrection Debug");
	w = initscr();
	start_color();
	noecho();
	keypad(w, true);
	nodelay(w, true);
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);
}

/*
** TODO:
** Use 'while' instead of 'if'
*/
void			get_hotkey(int *tab, int *send, int *recv)
{
	int			key;

	if ((key = getch()) == KEY_RIGHT)
		*tab = 1;
	else if (key == KEY_LEFT)
		*tab = 0;
	else if (key == KEY_UP)
		*send = (*send == 0);
	else if (key == KEY_DOWN)
		*recv = (*recv == 0);
	else if (key == 't')
		change_coord();
	else if (key == 'h')
		help_menu();
}

int				MainThread()
{
	LPSTR		cmdLine;
	int			tab;
	int			send = 1;
	int			recv = 1;

	tab = 0;
	cmdLine = GetCommandLineA();
	if (strstr(cmdLine, "/debug") > 0)
	{
		initCurses();
		initDebug();
		initHighlight();
		while (1)
		{
			get_hotkey(&tab, &send, &recv);
			clear();
			mvprintw(0, 0, "Press 'h' for help");
			print_menu(tab, send, recv);
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