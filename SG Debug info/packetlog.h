#include "printing.h"

#pragma once

#define PACKET_H_RECV (C_HEIGHT - 3)
#define PACKET_H_SEND (C_HEIGHT - 2)
#define PACKET_H_SAVE (C_HEIGHT - 1)
#define PACKET_W 0

void			initPacketLog();
char			**get_packet();
int				getPacketRecvCount();
int				getPacketSendCount();