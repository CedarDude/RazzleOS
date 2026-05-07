/*
COPYRIGHT C JAD 2026 MAY 2 - 20:00 GMT+1

This file is part of Razzle OS.
required for the shell, it displays the layout selection screen and sets the keyboard layout accordingly.


- Jad

what added? 
- added a layout selection screen that appears on first boot,
allowing users to choose between azerty and qwerty layouts.
the keyboard driver is then configured based on the user's choice.

why?
- to fix problem of my os being only azerty and qwerty users cannot use a azerty layout, and to give users the choice of their preferred layout right from the start.

- Jad 20:00 gmt+1

-how to use?

- simply boot the os, and follow the on-screen instructions to select your keyboard layout.

then press:
- 1 or & for azerty
- 2 or é for qwerty

i added letters as there problems here with char codes
yep i hate them

==========================================================================================================================================================================
COPYRIGHT C JAD 2026 MAY 2 - 20:00 GMT+1
LICENSED UNDER GNU GENERAL PUBLIC LICENSE V3.0
==========================================================================================================================================================================

1. Anyone can copy, modify and distribute this software.
2. You have to include the license and copyright notice with each and every distribution.
3. You can use this software privately.
4. You can use this software for commercial purposes.
5. If you dare build your business solely from this code, you risk open-sourcing the whole code base.
6. If you modify it, you have to indicate changes made to the code.
7. Any modifications of this code base MUST be distributed with the same license, GPLv3.
8. This software is provided without warranty.
9. The software author or license can not be held liable for any damages inflicted by the software.

==========================================================================================================================================================================

Thanks. and goodbye.
*/

#ifndef LAYOUT_SCREEN_H
#define LAYOUT_SCREEN_H

void layout_screen_init(void);

#endif
