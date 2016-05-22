#include "printing.h"

#pragma once

#define COORD_H_POS (C_WIDTH - 13)
#define COORD_X 3
#define COORD_Y 4
#define COORD_Z 5

typedef struct			s_position
{
	float				x;
	float				y;
	float				z;
}						t_position;

void					hkCoord();
t_position				*getPosition();