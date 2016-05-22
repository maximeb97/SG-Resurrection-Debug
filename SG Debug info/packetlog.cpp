#include <Winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "detours.h"
#include "packetlog.h"
#include "printing.h"

#pragma comment(lib, "detours.lib")
#pragma warning(disable: 4996)

typedef int					(*t_WSA)(SOCKET s, char *buf, int len, int flags);
static t_WSA				oWSARecv;
static t_WSA				oWSASend;
int							MyWSARecv(SOCKET s, char *buf, int len, int flags);
int							MyWSASend(SOCKET s, char *buf, int len, int flags);
int							packetRecvCount = 0;
int							packetSendCount = 0;
FILE						*packetlogs = 0;
char						**packets = 0;

char						**get_packet()
{
	return (packets);
}

void						record_packet(unsigned char *buf, int len, char type)
{
	int						i;
	int						j;
	char					*tmp;
	char					*tmp2;

	if (len <= 4)
		return ;

	tmp = packets[0];
	packets[0] = (char *)malloc(len * 3 + 1);
	sprintf(packets[0], "%c:", type);
	sprintf(packets[0] + 2, "[%02x%02x] ", buf[4], buf[3]);
	i = 5;
	j = 9;
	while (i < len)
	{
		sprintf(packets[0] + j, "%02x ", buf[i]);
		i = i + 1;
		j = j + 3;
	}
	packets[0][j] = 0;
	i = 1;
	while (i != C_HEIGHT - 9)
	{
		tmp2 = packets[i];
		packets[i] = tmp;
		tmp = tmp2;
		i = i + 1;
	}
	if (tmp != 0)
		free(tmp);
}

int							getPacketRecvCount()
{
	return (packetRecvCount);
}

int							MyWSARecv(SOCKET s, char *buf, int len, int flags)
{
	int						i;
	unsigned char			*ubuf;

	ubuf = (unsigned char *)buf;
	_asm pushad;
	packetRecvCount = packetRecvCount + 1;
	record_packet(ubuf, len, 'R');
	fwrite("Recv: ", 1, 6, packetlogs);
	i = 2;
	while (i < len)
	{
		if (i == 2 && len >= 4)
		{
			fprintf(packetlogs, "[0x%02x%02x] ", ubuf[i + 1], ubuf[i]);
			i = i + 1;
		}
		else
			fprintf(packetlogs, "%02x ", ubuf[i]);
		i = i + 1;
	}
	fprintf(packetlogs, "\n");
	_asm popad;
	return (oWSARecv(s, buf, len, flags));
}

int							getPacketSendCount()
{
	return (packetSendCount);
}

int							MyWSASend(SOCKET s, char *buf, int len, int flags)
{
	int						i;
	unsigned char			*ubuf;

	ubuf = (unsigned char *)buf;
	_asm pushad;
	packetSendCount = packetSendCount + 1;
	record_packet(ubuf, len, 'S');
	fwrite("Send: ", 1, 6, packetlogs);
	i = 2;
	while (i < len)
	{
		if (i == 2 && len >= 4)
		{
			fprintf(packetlogs, "[0x%02x%02x] ", ubuf[i + 1], ubuf[i]);
			i = i + 1;
		}
		else
			fprintf(packetlogs, "%02x ", ubuf[i]);
		i = i + 1;
	}
	fprintf(packetlogs, "\n");
	_asm popad;
	return (oWSASend(s, buf, len, flags));
}

void						initPacketLog()
{
	DWORD					dwWSARecv;
	DWORD					dwWSASend;

	if (packetlogs == 0)
		packetlogs = fopen("PacketLog.txt", "w+");
	dwWSARecv = (DWORD)GetProcAddress(LoadLibraryA("Ws2_32.dll"), "recv");
	dwWSASend = (DWORD)GetProcAddress(LoadLibraryA("Ws2_32.dll"), "send");
	oWSARecv = (t_WSA)DetourFunction((PBYTE)dwWSARecv, (PBYTE)MyWSARecv);
	oWSASend = (t_WSA)DetourFunction((PBYTE)dwWSASend, (PBYTE)MyWSASend);
	packets = (char **)malloc(sizeof(unsigned char *) * ((C_HEIGHT) * 2));
	packets[0] = 0;
}