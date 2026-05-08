#include "key.h"

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

static inline unsigned char kbd_status() {
    return inb(0x64);
}

static const char scancode_azerty[128] = {
    0,   27,  '&', '\x82', '"', '\'', '(', '-', '\x88', '_', '\x87', '\x85',
    ')', '=', '\b', '\t',
   'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n', 0,   'q', 's',
   'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '\xF9', '*', 0,   '<', 'w', 'x', 'c', 'v',
   'b', 'n', ',', ';', ':', '!', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,  0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
   '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
static const char scancode_azerty_shift[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '\xB0', '+', '\b', '\t',
   'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '\xA8', '\xA3', '\n', 0,   'Q', 'S',
   'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '%', '\xB5', 0,   '>', 'W', 'X', 'C', 'V',
   'B', 'N', '?', '.', '/', '\xA7', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,  0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
   '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

/*
I 
*/
static const char scancode_qwerty[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', '\t',
   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,   'a', 's',
   'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\\', 'z', 'x', 'c', 'v', 'b',
   'n', 'm', ',', '.', '/', 0,   0,   '*', 0,   ' ', 0,   0,   0,   0,   0,
    0,  0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
   '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
static const char scancode_qwerty_shift[128] = {
    0,   27,  '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', '\t',
   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,   'A', 'S',
   'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '|', 'Z', 'X', 'C', 'V', 'B',
   'N', 'M', '<', '>', '?', 0,   0,   '*', 0,   ' ', 0,   0,   0,   0,   0,
    0,  0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
   '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};


// european qwertz layout, used in germany and some other countries
//
// idk why i did this
// shit nobody ask for it but i decided it useful after finding the screen is empty
//
// REMOVE // IF YOU WANT IT BACK!
// static const char scancode_qwertz[128] = {
//     0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
//     '\'', '+', '\b', '\t',
//    'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '\x81', '+', '\n', 0,   'a', 's',
//    'd', 'f', 'g', 'h', 'j', 'k', 'l', '\x94', '\x84', '^', '#', 'y', 'x', 'c', 'v', 'b',
//    'n', 'm', ',', '.', '-', 0,   0,   '*', 0,   ' ', 0,   0,   0,   0,   0,
//     0,  0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
//    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
// };
// static const char scancode_qwertz_shift[128] = {
//     0,   27,  '!', '"', ';', '%', '&', '/', '(', ')', '=', '?',
//     '\'', '*', '\b', '\t',
//    'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', '\x9A', '*', '\n', 0,   'A', 'S',
//    'D', 'F', 'G', 'H', 'J', 'K', 'L', '\x99', '\x8E', '\xF8', '\'', 'Y', 'X', 'C', 'V', 'B',
//    'N', 'M', ';', ':', '_', 0,   0,   '*', 0,   ' ', 0,   0,   0,   0,   0,
//     0,  0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
//    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
// };
// 
static const char* scancode_map = scancode_azerty;
static const char* scancode_shift_map = scancode_azerty_shift;

#define KBD_SCANCODE_COUNT 128


/*

TODAY - 01/05/2026

what modified: i added higher cases, the thing yall had been waiting for months.. yay
be happy now and shut up.

and fuck off now off my emails begging me to give you shift!

- Jad 17:07 gmt+1
*/
static int shift_pressed = 0;


void keyboard_init() {
    keyboard_set_layout(1);
}

void keyboard_set_layout(unsigned char layout) {
    if (layout == 2) {
        scancode_map = scancode_qwerty;
        scancode_shift_map = scancode_qwerty_shift;
    } //else if (layout == 3) {
       // scancode_map = scancode_qwertz; // middle europe stuff
       // scancode_shift_map = scancode_qwertz_shift;
   /// } 
   else {
        scancode_map = scancode_azerty;
        scancode_shift_map = scancode_azerty_shift;
    }
}

unsigned char get_key() {
    if ((kbd_status() & 0x01) == 0) {
        return 0;
    }

    unsigned char scancode = inb(0x60);

    /*
    handle left and right shift button 
    */
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return 0;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        return 0;
    }

    if (scancode & 0x80) {
        return 0;
    }

    /*
    if someone touched these 3 FUCKING LINES OF CODE I SWEAR TO GOD I WILL FUCKING END YOU
    IF YOU BREAK I WILL FUCKING BAN YOU OR SMT
    /joke 
    but seriously don't touch this part as it took me 9 hours to REALISE I DONT FUCKING NEED TO CHECK FOR ARROWS BECAUSE
    IT DONT KNOW WHAT A FUCKING ARROW IS
    */
    if (scancode == 0x48 || scancode == 0x50) {
        return scancode;
    }

    if (scancode >= KBD_SCANCODE_COUNT) {
        return 0;
    }

    char ch = shift_pressed ? scancode_shift_map[scancode] : scancode_map[scancode];

    if (ch == 0) {
        return 0;
    }

    if (ch == '\n' || ch == '\b' || ch == '\t' || ch == 27) {
        return ch;
    }

    {
        unsigned char u = (unsigned char)ch;
        if (u >= 32 && u <= 126) return ch;
        if (u >= 128) return ch;
    }
    return 0;
}

void keyboardtest(void){
    printv("Keyboard test driver operation had been successful.\nCopyright (c) jad 2026 Driver Tester (DriTest)\n");
}