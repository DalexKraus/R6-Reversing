# R6-Reversing

Disclaimer: **THIS IS NOT READY TO PASTE - YOU WILL BE BANNED!**


Last updated for: **Operation Shadow Legacy 25.11.2020**


## How it Works
Important information (ESP, health, tracers) are shown inside an overlay which hovers over the actual game window.

The overlay uses the LynxEngine as framework for rendering which uses OpenGL as rendering backend.
Furthermore, the LynxEngine does use GLFW as library for window creation, input handling, etc.,
which means that we have to modify GLFW to create a transparent window for us.
This is done by applying the styles *WS_EX_LAYERED* and *WS_EX_TRANSPARENT* to the window.

	If GLFW needs to be rebuilt:
	Download the GLFW sources and navigate to the file win32_window.c.
	Search for 'CreateWindowExW' and set 'WS_EX_LAYERED' as style.

The style *WS_EX_TRANSPARENT* is added later on at runtime by using the function *SetWindowLong*.

To keep the window on top everyting else, the window position is set manually each frame.


## Features
- ESP (Extra Sensory Perception)
- Tracers (not finished in this overlay version)

## Detection Vectors

You have to find a way to read memory from the game without BattlEye noticing it.
There are a couple ways to do this:
* Operating at ring0 using a vulnerable driver for kernel-level code execution (next lightning talk)
* Hijacking other applications in ring3
* Using a hypervisor

BattlEye itself has some detection mechanisms for Hypervisors,
those have to be bypassed. These include **at least** the following:
* VM-Exit timing checks
* CPU flags
* Drive names
* Drive serial numbers
* Disk space
* Hardware serial numbers

I will not give any information about how to bypass these, but it is possible.


The overlay itself **could** also be a detection vector, but I highly doubt it.
BattlEye does perform scans for windows using *WS_EX_LAYERED* and *WS_EX_TRANSPARENT*,
but as the overlay process itself does not have **any** handles to the game or any BattlEye
processes, it is unsure if the player is actually doing something suspicious.

**The cheat itself does NOT modify any game memory.**
