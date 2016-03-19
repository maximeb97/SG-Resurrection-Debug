#include "curses.h"
#include "coord.h"
#include "packetlog.h"

void		print_PlayerPos()
{
	t_position		*pos;

	mvprintw(0, 65, "    Player Pos ");
	mvprintw(1, 65, "  _____________");
	mvprintw(2, 65, "X |");
	mvprintw(3, 65, "Y |");
	mvprintw(4, 65, "Z |");
	mvprintw(5, 65, "  |____________");
	if ((pos = getPosition()) != 0)
	{
		mvprintw(2, 69, "%.2f  ", pos->x);
		mvprintw(3, 69, "%.2f  ", pos->y);
		mvprintw(4, 69, "%.2f  ", pos->z);
	}
}

void		print_PacketInfo()
{
	mvprintw(22, 0, "Packet Received: %d", getPacketRecvCount());
	mvprintw(23, 0, "Packet Send: %d", getPacketSendCount());
	mvprintw(24, 0, "Saved in \"PacketLog.txt\"");
}