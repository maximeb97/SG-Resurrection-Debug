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
	- Saved in file "PacketLog.txt", format "[0xID] %02x ..."
	- Shown in the "Packets" menu:
	 	- 'S' -> sended packets
	 	- 'R' -> received packets

### Game debug message
	- Shown in the "Debug Info" menu
	- Saved in file "DebugLog.txt"

### Keys
	- Left arrow -> Show packets
	- Right arrow -> Show debug messages
	- Up arrow -> Show / Hide sended
	- Down arrow -> Show / Hide received packets

---

## Hooks

	- send (Ws2_32.dll) for packets log
	- recv (Ws2_32.dll) for packets log
	- IsDebuggerPresent (Kernel32.dll) for debug messages
	- OutputDebugStringA (Kernel32.dll) for debug messages
	- 0x0046A3EF (StreetGear.exe) for player position

---

## Screen

![](http://i.imgur.com/IcYdzj8.png)
![](http://i.imgur.com/jU56Lbn.png)

---

### Made by

Maxime Betemps (betemp_a)

Astropilot -> Debug Infos