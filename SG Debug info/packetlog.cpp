#include <Winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "detours.h"
#include "packetlog.h"
#include "printing.h"

#pragma comment(lib, "detours.lib")
#pragma warning(disable: 4996)

typedef int					(WINAPI *t_WSA)(SOCKET s, char *buf, int len, int flags);
static t_WSA				oWSARecv;
static t_WSA				oWSASend;
int	WINAPI					MyWSARecv(SOCKET s, char *buf, int len, int flags);
int	WINAPI					MyWSASend(SOCKET s, char *buf, int len, int flags);
int							packetRecvCount = 0;
int							packetSendCount = 0;
FILE						*packetlogs = 0;
char						**packets = 0;

extern char					**blacklist; // in DebugStr.cpp

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
	char					*packet_byte;

	if (len <= 4)
		return ;
	packet_byte = (char *)malloc(len * 3 + 1);
	sprintf(packet_byte, "%c:", type);
	sprintf(packet_byte + 2, "[%02x%02x] ", buf[3], buf[2]);
	i = 4;
	j = 9;
	while (i < len)
	{
		sprintf(packet_byte + j, "%02x ", buf[i]);
		i = i + 1;
		j = j + 3;
	}
	packet_byte[j] = 0;
	j = 0;
	while (blacklist[j] != 0)
	{
		if (strstr(packet_byte, blacklist[j]) != 0 || strncmp(packet_byte, blacklist[j], strlen(blacklist[j])) == 0)
			return ;
		j = j + 1;
	}
	tmp = packets[0];
	packets[0] = packet_byte;
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

int	WINAPI						MyWSARecv(SOCKET s, char *buf, int len, int flags)
{
	int						i;
	unsigned char			*ubuf;
	int						ret;

	ubuf = (unsigned char *)buf;
	ret = oWSARecv(s, buf, len, flags);
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
	return (ret);
}

int							getPacketSendCount()
{
	return (packetSendCount);
}

int	WINAPI					MyWSASend(SOCKET s, char *buf, int len, int flags)
{
	int						i;
	unsigned char			*ubuf;
	int						ret;

	ubuf = (unsigned char *)buf;
	ret = oWSASend(s, buf, len, flags);
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
	return (ret);
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