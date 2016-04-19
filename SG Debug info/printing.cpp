#include "curses.h"
#include "coord.h"
#include "packetlog.h"

void				print_PlayerPos()
{
	t_position		*pos;

	mvprintw(0, 63, "    Player Pos ");
	mvprintw(1, 63, "  _______________");
	mvprintw(2, 63, "X |");
	mvprintw(3, 63, "Y |");
	mvprintw(4, 63, "Z |");
	mvprintw(5, 63, "  |______________");
	if ((pos = getPosition()) != 0)
	{
		mvprintw(2, 67, "%.5f  ", pos->x);
		mvprintw(3, 67, "%.5f  ", pos->y);
		mvprintw(4, 67, "%.5f  ", pos->z);
	}
}

void		print_PacketInfo()
{
	mvprintw(22, 0, "Packet Received: %d", getPacketRecvCount());
	mvprintw(23, 0, "Packet Send: %d", getPacketSendCount());
	mvprintw(24, 0, "Saved in \"PacketLog.txt\"");
}