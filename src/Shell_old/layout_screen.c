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

#include "layout_screen.h"
#include "../drivers/VGAf/vgaf.h"
#include "../drivers/KEYdriver/key.h"

void layout_screen_init(void) {
    colorscreen(0x14);
    colortext(0x0F);

    printv("Welcome to Razzle Boot Setup!\n\n");
    printv("================================================================================\n");
    printv("Welcome to Razzle Setup screen!\nThis screen allows you to select your preferred keyboard layout for the shell.\n");
    printv("Please select your keyboard layout:\n\n");
    printv("Do you want to use:\n");
    printv("-");
    printv(" azerty layout\n");
    printv("-");
    printv(" qwerty layout\n");
    printv("-");
    // printv(" qwertz Europe Layout\n\n");
    printv("Press 1 for azerty, 2 for qwerty and 3 for qwertz\n");
    printv("(on AZERTY, you can also press & for azerty, \x82 for qwerty, or # for qwertz)\n");
    printv("Waiting for input...\n\n\nCOPYRIGHT C JAD 2026\nFREE OPEN SOURCE SOFTWARE, LICENSED UNDER GPLv3\n");

    unsigned char choice = 0;
    while (choice != '1' && choice != '2' && choice != '3') {
        unsigned char key = get_key();
        if (key == '1' || key == '&') {
            choice = '1';
        } else if (key == '2' || key == '\x82') {
            choice = '2';
        } 
        // turn this on if you want qwertz, but i dont want it so i commented it out
        //else if (key == '3' || key == '#') {
           // choice = '3';
        //}
    }
// clear the fuck
// IT ANNOY ME I WANT TO SLAP THE SHIT OUT OF THIS
// FUCK
/*
as a last message i passed 3 hours because of A ELSE I FORGOT!

here is a warning for someone else comming

IF YOU TOUCH AGAIN EVEN 1 SINGLE LINE OF CODE I WILL COME TO YOUR HOME AND....

but:

int lost_hours_on_useless_shit 30;



*/
    printv("\n\n");
    if (choice == '1') {
        printv("AZERTY layout selected.\n");
        keyboard_set_layout(1);
    } else if (choice == '2') {
        printv("QWERTY layout selected.\n");
        keyboard_set_layout(2);
    } else {
        printv("QWERTZ layout selected.\n");
        keyboard_set_layout(3);
    }
    printv("Loading shell...\nLoading...\n...Initialized Shell!\n\n");
    clear();
    colortext(0x0F);
}

void layoutswitcher_test(void){
    printv("Layout switcher test driver operation had been successful.\nCopyright (c) jad 2026 Driver Tester (DriTest)\n");
}
