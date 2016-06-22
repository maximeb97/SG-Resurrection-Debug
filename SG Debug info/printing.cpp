#include <string.h>
#include <stdlib.h>
#include "curses.h"
#include "coord.h"
#include "packetlog.h"
#include "printing.h"
#include "DebugStr.h"

#pragma warning(disable: 4996)

t_syntax	syntax[3] =
{
	{"S:", 1},
	{"R:", 2},
	{0, 3}
};

t_syntax	*usr_syntax;

void				initHighlight()
{
	FILE					*fd;
	int						i;
	char					buf[256];
	int						len;

	i = 0;
	if ((fd = fopen("dbg_highlight.txt", "r")) != 0)
	{
		while (fgets(buf, sizeof(buf), fd) != 0)
			i = i + 1;
		fclose(fd);
		usr_syntax = (t_syntax *)malloc(sizeof(t_syntax) * i + 1);
		usr_syntax[i].color = 0;
		usr_syntax[i].syntax = 0;
		fd = fopen("dbg_highlight.txt", "r");
		i = 0;
		while (fgets(buf, sizeof(buf), fd) != 0)
		{
			buf[strlen(buf) - 1] = 0;
			if (strlen(buf) > 2)
				buf[1] = 0;
			usr_syntax[i].color = atoi(buf);
			usr_syntax[i].syntax = strdup(buf + 3);
			i = i + 1;
		}
		fclose(fd);
	}
}

void				print_PlayerPos()
{
	t_position		*pos;

	pos = getPosition();
	mvprintw(COORD_X - 2, COORD_H_POS - 6, "Player Pos %08x", pos);
	mvprintw(COORD_X - 1, COORD_H_POS - 4, "  _______________");
	mvprintw(COORD_X, COORD_H_POS - 4, "X |");
	mvprintw(COORD_Y, COORD_H_POS - 4, "Y |");
	mvprintw(COORD_Z, COORD_H_POS - 4, "Z |");
	mvprintw(COORD_Z + 1, COORD_H_POS - 4, "  |______________");
	if (pos != 0)
	{
		mvprintw(COORD_X, COORD_H_POS, "%.3f  ", pos->x);
		mvprintw(COORD_Y, COORD_H_POS, "%.3f  ", pos->y);
		mvprintw(COORD_Z, COORD_H_POS, "%.3f  ", pos->z);
	}
}

void		print_PacketInfo()
{

	mvprintw(PACKET_H_RECV, PACKET_W, "Packet Received: %d", getPacketRecvCount());
	mvprintw(PACKET_H_SEND, PACKET_W, "Packet Send: %d", getPacketSendCount());
	mvprintw(PACKET_H_SAVE, PACKET_W, "Saved in \"PacketLog.txt\"");
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

void		color_info(char *info, int on)
{
	int		i;
	int		col;

	i = 0;
	while (syntax[i].syntax != 0)
	{
		if (strncmp(info, syntax[i].syntax, strlen(syntax[i].syntax)) == 0)
			break;
		i = i + 1;
	}
	col = syntax[i].color;
	i = 0;
	while (usr_syntax[i].syntax != 0)
	{
		if (strstr(info, usr_syntax[i].syntax) != 0 || strncmp(info, usr_syntax[i].syntax, strlen(usr_syntax[i].syntax)) == 0)
			col = usr_syntax[i].color;
		i = i + 1;
	}
	if (on == 1)
		attron(COLOR_PAIR(col));
	else
		attroff(COLOR_PAIR(col));
}

/*
** Print information in info (debug info / packet log)
*/
void		print_info(char **info, int send, int recv)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (info[j] != 0 && i < C_HEIGHT - 9)
	{
		if (strlen(info[j]) > C_WIDTH - 23)
			info[j][C_WIDTH - 23] = 0;
		color_info(info[j], 1);
		if (info[j][1] == ':' && ((info[j][0] == 'S' && send == 0) || (info[j][0] == 'R' && recv == 0)))
			i = i - 1;
		else
			mvprintw(i + 4, 3, "%s", info[j]);
		color_info(info[j], 0);
		i = i + 1;
		j = j + 1;
	}
}

void		print_menu(int tab, int send, int recv)
{
	int		i;
	char	**info = 0;

	if (tab == 0)
		info = get_debug();
	else if (tab == 1)
		info = get_packet();
	if (info == 0)
		return ;
	if (tab == 0)
		attron(A_REVERSE);
	mvprintw(2, 3, "Debug Info | Saved in \"DebugLog.txt\"");
	attroff(A_REVERSE);
	mvprintw(2, 40, "- ");
	if (tab == 1)
		attron(A_REVERSE);
	mvprintw(2, 42, "Packets");
	attroff(A_REVERSE);
	if (send == 1)
		attron(A_REVERSE);
	mvprintw(2, 50, "S");
	attroff(A_REVERSE);
	if (recv == 1)
		attron(A_REVERSE);
	mvprintw(2, 52, "R");
	attroff(A_REVERSE);
	print_info(info, send, recv);
	print_border();
}