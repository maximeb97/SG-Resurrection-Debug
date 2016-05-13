#include <string.h>
#include "curses.h"
#include "coord.h"
#include "packetlog.h"
#include "printing.h"
#include "DebugStr.h"

void				print_PlayerPos()
{
	t_position		*pos;

	pos = getPosition();
	mvprintw(1, C_WIDTH - 19, "Player Pos %08x", pos);
	mvprintw(2, C_WIDTH - 17, "  _______________");
	mvprintw(3, C_WIDTH - 17, "X |");
	mvprintw(4, C_WIDTH - 17, "Y |");
	mvprintw(5, C_WIDTH - 17, "Z |");
	mvprintw(6, C_WIDTH - 17, "  |______________");
	if (pos != 0)
	{
		mvprintw(3, C_WIDTH - 13, "%.3f  ", pos->x);
		mvprintw(4, C_WIDTH - 13, "%.3f  ", pos->y);
		mvprintw(5, C_WIDTH - 13, "%.3f  ", pos->z);
	}
}

void		print_PacketInfo()
{

	mvprintw(C_HEIGHT - 3, 0, "Packet Received: %d", getPacketRecvCount());
	mvprintw(C_HEIGHT - 2, 0, "Packet Send: %d", getPacketSendCount());
	mvprintw(C_HEIGHT - 1, 0, "Saved in \"PacketLog.txt\"");
}

void		print_border()
{
	int		i;

	i = 0;
	while (i != C_WIDTH - 22)
	{
		mvprintw(3, i + 3, "-");
		mvprintw(C_HEIGHT - 5, i + 3, "-");
		i = i + 1;
	}
	i = 0;
	while (i != C_HEIGHT - 7)
	{
		if (i == 0)
		{
			mvprintw(i + 3, 2, "/");
			mvprintw(i + 3, C_WIDTH - 20, "\\");
		}
		else if (i == C_HEIGHT - 8)
		{
			mvprintw(i + 3, 2, "\\");
			mvprintw(i + 3, C_WIDTH - 20, "/");
		}
		else
		{
			mvprintw(i + 3, 2, "|");
			mvprintw(i + 3, C_WIDTH - 20, "|");
		}
		i = i + 1;
	}
}

void		print_debuginfo()
{
	int		i;
	char	*tmp;
	char	**debugstr;

	if ((debugstr = get_debug()) == 0)
		return ;
	mvprintw(2, 3, "Debug Info | Saved in \"DebugLog.txt\"");
	i = 0;
	while (debugstr[i] != 0)
	{
		if (strlen(debugstr[i]) > C_WIDTH - 23)
			debugstr[i][C_WIDTH - 23] = 0;
		mvprintw(i + 4, 3, "%s", debugstr[i]);
		i = i + 1;
	}
	print_border();
}