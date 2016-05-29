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
	- 't' -> Change the player position
	- 'h' -> Display help menu

---

## Hooks

	- send (Ws2_32.dll) for packets log
	- recv (Ws2_32.dll) for packets log
	- IsDebuggerPresent (Kernel32.dll) for debug messages
	- OutputDebugStringA (Kernel32.dll) for debug messages
	- 0x0046A3EF (StreetGear.exe) for player position

---

## Screen

![](http://image.prntscr.com/image/f250424eac974a30a0ce5d2f0f44aed3.png)
![](http://image.prntscr.com/image/746a8f69d404472fa465a69332e7755f.png)

---

## Changelog
```
[29 / 05 / 2016]
- Added Changelog (README)
- Added help menu
- It's now possible to change the coordinates

[22 / 05 / 2016]
- Added packet view
- Added colors in debug info and packets view

[13 / 05 / 2016]
- Added log of debug informations
- Added memory address of player position

[19 / 04 / 2016]
- Added README.MD
- Some small modifications

[19 / 03 / 2016]
- Packet log
- Show coordinates
```

### Made by

maximeb97

Astropilot -> Debug Infos