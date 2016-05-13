# __SG-Resurrection-Debug__
A debug console for Street Gears

### __Usage__

Start the game with `/debug` parameter, the debug console will be loaded automatically.

---

## Features

### Player position
	- X Position
	- Y Position
	- Z Position
	- Memory address of the position
	
### Packets log
	- Output (send)
	- Input (recv)
	- Saved in file "PacketLog.txt", format "%02x ..."

### Game debug message
	- Game debug message
	- Saved in file "DebugLog.txt"

---

## Hooks

	- send (Ws2_32.dll) for packets log
	- recv (Ws2_32.dll) for packets log
	- IsDebuggerPresent (Kernel32.dll) for debug messages
	- OutputDebugStringA (Kernel32.dll) for debug messages
	- 0x0046A3EF (StreetGear.exe) for player position

---

## Screen

![](http://i.imgur.com/FhV8SpV.png)

---

### Made by

Maxime Betemps (betemp_a)

