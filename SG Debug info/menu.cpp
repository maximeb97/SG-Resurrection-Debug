#include <stdlib.h>
#include <Windows.h>
#include "curses.h"
#include "printing.h"
#include "coord.h"

char					*get_float(int x, int y)
{
	int					i;
	char				*str;
	char				in;

	str = (char *)malloc(256);
	i = 0;
	mvprintw(x, y, "");
	while ((in = getch()) != '\n')
	{
		if (in != ERR)
		{
			if ((in >= '0' && in <= '9') || in == '.' || in == '-')
			{
				str[i] = in;
				mvprintw(x, y + i, "%c", str[i]);
				i = i + 1;
				Sleep(50);
			}
			else if (in == '\b')
			{
				if (i > 0)
					i = i - 1;
				mvprintw(x, y + i, " ");
			}
		}
	}
	if (i == 0)
		return (0);
	str[i] = 0;
	return (str);
}

void					change_coord()
{
	char				*value;
	t_position			*pos;
	t_position			new_pos;

	pos = getPosition();
	if (pos != 0)
	{
		new_pos.x = pos->x;
		new_pos.y = pos->y;
		new_pos.z = pos->z;
		clear();
		mvprintw(0, 0, "Enter new X pos (previous = %.2f). Press enter for no changes", pos->x);
		if ((value = get_float(1, 0)) != 0)
			new_pos.x = atof(value);
		clear();
		mvprintw(0, 0, "Enter new Y pos (previous = %.2f). Press enter for no changes", pos->y);
		if ((value = get_float(1, 0)) != 0)
			new_pos.y = atof(value);
		clear();
		mvprintw(0, 0, "Enter new Z pos (previous = %.2f). Press enter for no changes", pos->z);
		if ((value = get_float(1, 0)) != 0)
			new_pos.z = atof(value);
		clear();
		free(value);
		pos->x = new_pos.x;
		pos->y = new_pos.y;
		pos->z = new_pos.z;
	}
}

void					help_menu()
{
	clear();
	mvprintw(0, 0, "StreetGears Resurrection Debug");

	attron(COLOR_PAIR(4));
	mvprintw(2, 0, "[Packets]");
	attroff(COLOR_PAIR(4));
	mvprintw(3, 0, "This menu allow you to see recv/send packets. They are also saved in the file \"PacketLog.txt\"");
	mvprintw(4, 0, "To see the packets, use \"LEFT ARROW\"");
	mvprintw(5, 0, "You can Hide/Shown recv/send packets with \"S\" and \"R\"");
	
	attron(COLOR_PAIR(4));
	mvprintw(7, 0, "[Coordinates]");
	attroff(COLOR_PAIR(4));
	mvprintw(8, 0, "This menu show you the player coordinates and the memory address where you can get them.");
	mvprintw(9, 0, "You can change them using \"t\"");

	attron(COLOR_PAIR(4));
	mvprintw(11, 0, "[Debug Info]");
	attroff(COLOR_PAIR(4));
	mvprintw(12, 0, "This menu show the debug information provided by the game.");
	mvprintw(13, 0, "To see it, use \"RIGHT ARROW\" (enabled by default)");
	
	mvprintw(16, 0, "Press any key to continue");

	mvprintw(C_HEIGHT - 1, 0, "Sources: ");
	attron(COLOR_PAIR(2));
	mvprintw(C_HEIGHT - 1, 9, "https://github.com/maximeb97/SG-Resurrection-Debug");
	attroff(COLOR_PAIR(2));

	while (getch() == ERR);
}