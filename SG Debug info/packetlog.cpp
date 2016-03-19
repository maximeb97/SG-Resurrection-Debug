#include <Winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include "detours.h"

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

typedef int					(*t_WSA)(SOCKET s, char *buf, int len, int flags);
static t_WSA				oWSARecv;
static t_WSA				oWSASend;
int							MyWSARecv(SOCKET s, char *buf, int len, int flags);
int							MyWSASend(SOCKET s, char *buf, int len, int flags);
int							packetRecvCount = 0;
int							packetSendCount = 0;
FILE						*packetlogs = 0;

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
	fwrite("Recv: ", 1, 6, packetlogs);
	i = 0;
	while (i != len)
	{
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
	fwrite("Send: ", 1, 6, packetlogs);
	i = 0;
	while (i != len)
	{
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
}