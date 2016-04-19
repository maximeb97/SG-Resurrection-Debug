#include <Windows.h>
#include "coord.h"

t_position				*pos = 0;
void					*jmpBack = (void *)0x0046A3EF;

_declspec(naked) void	hkCoord()
{
	_asm
	{
		cmp eax, 0x00400000
		jb ori
		mov pos, eax
		ori:
		fstp dword ptr[eax]
		fld dword ptr[ecx + 04]
		jmp jmpBack
	}
}

t_position				*getPosition()
{
	if (pos == 0)
		return (0);
	return (pos);
}