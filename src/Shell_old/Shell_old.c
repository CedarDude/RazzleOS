#include "Shell_old.h"
#include "../drivers/VGAf/vgaf.h"
#include "../drivers/KEYdriver/key.h"
#include "layout_screen.h"
#include "../branding/branding.h"
#include "../include/bugcheck.h"

#define BUFFER_SIZE 256
#define HISTORY_SIZE 10 // Number of commands to store in history

static char history[HISTORY_SIZE][BUFFER_SIZE]; // Circular buffer for command history
static int history_count = 0;      // Number of commands currently stored in history
static int history_write_ptr = 0;  // Index in 'history' where the next command will be written

/* Performs a case-insensitive string comparison */
static int string_equal_ci(const char* a, const char* b) {
    while (*a && *b) {
        char ca = *a;
        char cb = *b;
        if (ca >= 'A' && ca <= 'Z') ca += 'a' - 'A';
        if (cb >= 'A' && cb <= 'Z') cb += 'a' - 'A';
        if (ca != cb) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

/* Converts a decimal string to a signed integer */
static int string_to_int(const char* str) {
    int res = 0;
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res * sign;
}

/* converts a hexadecimal string like some ff (mpeg OK STOP YOU ARE GOING ANNOYING
OK SHUT UP


uhh like "FF") */
static unsigned int hex_to_uint(const char* str) {
    unsigned int res = 0;
    char c;
    while ((c = *str) != '\0') {
        if (c >= '0' && c <= '9') res = res * 16 + (c - '0');
        else if (c >= 'a' && c <= 'f') res = res * 16 + (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') res = res * 16 + (c - 'A' + 10);
        else break; // Invalid character, stop parsing
        str++;
    }
    return res;
}

/* Reads a byte from an I/O port */
static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/* Writes a byte to an I/O port */
static inline void outb(unsigned short port, unsigned char value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

/* Reads from the CMOS/RTC registers (used here for RAM detection) */
static unsigned int cmos_read(unsigned char addr) {
    outb(0x70, addr | 0x80);
    return (unsigned int)inb(0x71);
}

static void print_uint(unsigned int n) {
    char buf[12];
    int i = 0;
    if (n == 0) {
        vga_putc('0');
        return;
    }
    while (n) {
        buf[i++] = (char)('0' + (n % 10));
        n /= 10;
    }
    while (i > 0) vga_putc(buf[--i]);
}

/* Prints a signed integer to the screen */
static void print_int(int n) {
    if (n < 0) {
        vga_putc('-');
        print_uint((unsigned int)(-n));
    } else {
        print_uint((unsigned int)n);
    }
}

static int int_to_str(int n, char* buf) {
    unsigned int value;
    int i = 0;
    if (n < 0) {
        buf[i++] = '-';
        value = (unsigned int)(-n);
    } else {
        value = (unsigned int)n;
    }

    char tmp[12];
    int j = 0;
    if (value == 0) {
        tmp[j++] = '0';
    }
    while (value) {
        tmp[j++] = (char)('0' + (value % 10));
        value /= 10;
    }
    while (j > 0) {
        buf[i++] = tmp[--j];
    }
    buf[i] = '\0';
    return i;
}

/* A generic blocking function to read a full line of text with backspace support */
static void read_input_line(char* buffer, int max_len) {
    int buf_idx = 0;
    buffer[0] = '\0'; // Ensure buffer is empty initially

    /* Polling loop for keyboard input */
    while (1) {
        unsigned char key = get_key();
        if (key) {
            if (key == '\r' || key == '\n') {
                buffer[buf_idx] = '\0';
                vga_putc('\n');
                break;
            } else if (key == '\b' && buf_idx > 0) {
                buf_idx--;
                /* Visual backspace: move back, print space, move back again */
                vga_putc('\b'); vga_putc(' '); vga_putc('\b');
            } else if (buf_idx < max_len - 1 && key >= ' ' && key <= '~') { 
                /* Only store and echo printable ASCII characters */
                buffer[buf_idx++] = (char)key; 
                vga_putc((char)key);
            }
        }
    }
}

static void cmd_ver(void) {
    printv("Razzle codename Litewave");
    printv(" [Version ");
    print_int(version);
    printv(", Build ");
    print_int(build);
    printv("]\n");
    printv("Copyright (c) 2026 Jad. All rights reserved.\n");
    printv("License: GNU General Public License v3.0\n");
}

static void cmd_clear(void) {
    clear();
}

static void cmd_print(const char* rest) {
    if (rest && *rest) printv(rest);
    printv("\n");
}


/*
Copyright (c) 2026 Jad. All rights reserved.
shell - A simple command-line interface for Razzle OS, providing basic commands and system information.

*/


static void cmd_sysinfo(void) {
    /* Retrieve CPUID Vendor String */
    printv_color("CPU =======================================================\n", 0x0E);
    {
        unsigned int ebx, ecx, edx;
        __asm__ __volatile__(
            "cpuid"
            : "=b"(ebx), "=c"(edx), "=d"(ecx)
            : "a"(0)
        );
        char v[13];
        v[0] = (char)(ebx & 0xff);
        v[1] = (char)((ebx >> 8) & 0xff);
        v[2] = (char)((ebx >> 16) & 0xff);
        v[3] = (char)((ebx >> 24) & 0xff);
        v[4] = (char)(ecx & 0xff);
        v[5] = (char)((ecx >> 8) & 0xff);
        v[6] = (char)((ecx >> 16) & 0xff);
        v[7] = (char)((ecx >> 24) & 0xff);
        v[8] = (char)(edx & 0xff);
        v[9] = (char)((edx >> 8) & 0xff);
        v[10] = (char)((edx >> 16) & 0xff);
        v[11] = (char)((edx >> 24) & 0xff);
        v[12] = '\0';
        printv("Vendor: ");
        printv(v);
        printv("\n");
    }
    /* Detect RAM size using CMOS registers */
    printv_color("RAM =======================================================\n", 0x0E);
    {
        unsigned int low = cmos_read(0x17) | (cmos_read(0x18) << 8);
        unsigned int high = cmos_read(0x30) | (cmos_read(0x31) << 8);
        unsigned int total_kb = low + high;
        if (total_kb == 0) total_kb = low;
        unsigned int total_mb = total_kb / 1024;
        printv("Extended: ");
        print_uint(total_kb);
        printv(" KB (");
        print_uint(total_mb);
        printv(" MB)\n");
    }
    printv_color("Storage ===================================================\n", 0x0E);
    printv("Boot device: hard disk drive/USB\n");
}

static void cmd_help(void) {
    printv_color("***************************************\n", 0x0A);
    printv_color("*        Razzle Shell Commands        *\n", 0x0A);
    printv_color("*        Copyright Jad 2026           *\n", 0x0A);
    printv_color("***************************************\n", 0x0A);
    printv("help                 -     Show this help message\n");
    printv("about                -     Show information about Razzle\n");
    printv("version              -     Show version information\n");
    printv("systeminfo           -     Show basic system information\n");
    printv("dir                  -     List files in current directory (not implemented)\n");
    printv("cls / clear          -     Clear the screen\n");
    printv("echo <text>          -     Print the specified text\n");
    printv("raztools             -     Show Razzle Toolkit information\n");
    printv("raztools sysinfo     -     Show detailed system information\n");
    printv("raztools nasm-editor -     Run assembly instructions live on!\n");
    printv("raztools reboot      -     Reboot the system\n");
    printv("raztools halt        -     Halt the system\n");
    printv("raztools backtest    -     Run a color test to check if\nyour display is working correctly\n");
    printv("calc.exr             -     A simple calculator utility\n");
}

static void cmd_about(void) {
    colorscreen(0x00);
    colortext(0x0F);
    printv("Razzle codename Litewave - Lightweight small OS, [fallback mode] powered by Razzle Technology RT2\n");
    printv("Creators: JAD\n");
    printv("Copyright jad 2026.\n");
    printv("Razzle codename Litewave - Lightweight small OS, [fallback mode] powered by Razzle Technology RT2\n");
    printv("License: GNU General Public License v3.0\n");
    printv("Commands: help, about, sysinfo, dir, cls, reboot.\n");
    colortext(0x0F);
}

static void cmd_dir(void) {
    printv("Directory listing not implemented yet.\n");
}

static void system_colortest(void) {
    for (unsigned char color = 0; color < 16; color++) {
        colortext(color);
        
        printv_color(".:-*/||||||| the fox jumps over the lazy dog 1234567890 &é'(-è_çàà)^$ù*;:!\n", color);
        printv("");
    }
}


/* Allows users to input raw hex bytes and execute them as machine code.
   Extremely dangerous, but useful for live testing small assembly snippets. */
static void raznasm(void) {
    printv("Run Instructions :\n");
    char input_line[BUFFER_SIZE];
    read_input_line(input_line, BUFFER_SIZE);

    unsigned char code[BUFFER_SIZE / 3];
    int code_idx = 0;
    char* current_pos = input_line;
    char token_buffer[10]; // Temporary buffer for each hex token (e.g., "FF")

    while (*current_pos) {
        // Skip leading spaces
        while (*current_pos == ' ') {
            current_pos++;
        }
        if (!*current_pos) break; // End of string

        // Extract the hex token
        int token_len = 0;
        while (*current_pos != ' ' && *current_pos != '\0' && token_len < sizeof(token_buffer) - 1) {
            token_buffer[token_len++] = *current_pos;
            current_pos++;
        }
        token_buffer[token_len] = '\0'; // Null-terminate the token

        unsigned int byte = hex_to_uint(token_buffer);

        // Basic validation: check if the token was empty or contained non-hex chars
        // If token_len is 0, it means we had only spaces or reached end of string.
        // If byte is 0 and the token wasn't "0", it implies invalid hex.
        if (token_len == 0 || (byte == 0 && !(token_len == 1 && token_buffer[0] == '0'))) {
            printv_color("Invalid hex byte: ", 0x0C);
            printv(token_buffer);
            printv("\n");
            return;
        }
        code[code_idx++] = (unsigned char)byte;
    }

    if (code_idx == 0) {
        printv("No instructions entered.\n");
        return;
    }

    /* Cast the memory buffer to a function pointer and call it */
    void (*func)(void) = (void (*)(void))(unsigned int)code;
    func();
}



static void calc(void) {
    int first = 0;
    int second = 0; // Initialize second to 0
    char op;
    char input_buffer[BUFFER_SIZE]; // for reading numbers and operator

    printv("enter the first number: ");
    read_input_line(input_buffer, BUFFER_SIZE);
    first = string_to_int(input_buffer);

    printv("enter the operation sign (: * - + ) : ");
    read_input_line(input_buffer, BUFFER_SIZE);
    op = input_buffer[0]; // Take the first character as the operator

    printv("enter the second number: "); // Changed to printv
    read_input_line(input_buffer, BUFFER_SIZE);
    second = string_to_int(input_buffer);
    printv("\n");

    int result;
    switch (op) {
        case '*':
            result = first * second;
            printv("Result : ");
            print_int(result);
            printv("\n");
            break;
        case '-':
            result = first - second;
            printv("Result : ");
            print_int(result);
            printv("\n");
            break;
        case '+':
            result = first + second;
            printv("Result : ");
            print_int(result);
            printv("\n");
            break;
        case ':': // Assuming ':' is for division based on the original code
        case '/': // Also support '/' for division
            if (second == 0) {
                printv("Error: Division by zero!\n");
            } else {
                result = first / second;
                printv("Result : ");
                print_int(result);
                printv("\n");
            }
            break;
        default:
            printv("Error: Invalid operator!\n");
            break;
    }
}

static void razinfo(void) {

    RT_kernel_info();                    
    printv("  _____               _              \n");
    printv(" |  __ \\             | |             \n");
    printv(" | |__) |__ _ _______| | ___         \n");
    printv(" |  _  // _` |_  /_  / |/ _ \\        \n");
    printv(" | | \\ \\ (_| |/ / / /| |  __/        \n");
    printv(" |_|  \\_\\__,_/___/___|_|\\___|        \n");
    printv("  _______ ____   ____  _       _____ \n");
    printv(" |__   __/ __ \\ / __ \\|    |   ____|\n");
    printv("    | | | |  | | |  | | |    |  (___  \n");
    printv("    | | | |  | | |  | | |     \\___ \\ \n");
    printv("    | | | |__| | |__| | |____ ____) |\n");
    printv("    |_|  \\_\\__,_/___/___|_|\\_____|        \n");
          /*
          FUCK ASCCI ARTS!
          you dont know if your vtm will use them
          or trow at you "FIX UR OWN BULLSHIT CODES"

          fuck gcc-crosscompiler
          fuck elf-i686
          */                           
                                     
    printv("\n\n\n");
    printv("Razzle Toolkit - A collection of power utilities for Razzle Users.\n\n");
    printv_color("Help ========================================================================\n\n\n", 0x0A);
    printv("├ raztools backtest - Run a color test to check if your display is working correctly.\n");
    printv("├ raztools reboot - Reboot the system.\n");
    printv("├ raztools halt - Halt the system.\n");
    printv("├ history - Display command history.\n");
    printv("├ raztools info - Show information about Hardware Components.\n");
    printv("├ raztools nasm-editor - run assembly instructions live on!\n");
    printv("├  raztools dritest - Run a driver test.\n");
    printv("└  raztools administrative triple-fault - Test the triple fault handler.\n");
    printv("\n\n");

}

/* Displays the list of commands stored in the circular history buffer */
static void cmd_history(void) {
    if (history_count == 0) {
        printv("History is empty.\n");
        return;
    }
    for (int i = 0; i < history_count; i++) {
        // Calculate index from oldest to newest
        int idx = (history_write_ptr - history_count + i + HISTORY_SIZE) % HISTORY_SIZE;
        print_uint((unsigned int)(i + 1));
        printv(": ");
        printv(history[idx]);
        printv("\n");
    }
}

static void razhlt(void){
    __asm__ __volatile__("cli; hlt");
}


static void cmd_reboot(void) {
    printv_color("Rebooting...\n", 0x0C);
    /* Pulse the CPU reset line via the keyboard controller (8042)
       Sending 0xFE to port 0x64 is a standard legacy way to trigger a hard reset. */
    __asm__ __volatile__("cli; movb $0xFE, %al; outb %al, $0x64; hlt");
}



/* Basic command dispatcher. Parses the input buffer and routes to the correct function. */
static void shell_execute_command(char* buffer) {
    int i = 0;
    while (buffer[i] == ' ') i++;
    int cmd_start = i;
    while (buffer[i] && buffer[i] != ' ') i++;

    if (string_equal_ci("version", buffer + cmd_start)) {
        cmd_ver();
        return;
    }
    if (string_equal_ci("print", buffer + cmd_start)) {
        i = cmd_start + 5;
        if (buffer[i] == ' ') i++;
        cmd_print(buffer + i);
        return;
    }
    if (string_equal_ci("echo", buffer + cmd_start)) {
        i = cmd_start + 4;
        if (buffer[i] == ' ') i++;
        cmd_print(buffer + i);
        return;
    }
    if (string_equal_ci("info", buffer + cmd_start)) {
        cmd_sysinfo();
        return;
    }

    if (string_equal_ci("raztools sysinfo", buffer + cmd_start)) {
        cmd_sysinfo();
        return;
    }

    if (string_equal_ci("raztools nasm-editor", buffer + cmd_start)) {
        raznasm();
        return;
    }
    if (string_equal_ci("raztools halt", buffer + cmd_start)) {
        razhlt();
        return;
    }
    if (string_equal_ci("raztools backtest", buffer + cmd_start)) {
        system_colortest();
        return;
    }
    if (string_equal_ci("cls", buffer + cmd_start) || string_equal_ci("clear", buffer + cmd_start)) {
        cmd_clear();
        return;
    }

    if (string_equal_ci("raztools", buffer + cmd_start)) {
        razinfo();
        return;
    }
    if (string_equal_ci("help", buffer + cmd_start)) {
        cmd_help();
        return;
    }
    if (string_equal_ci("dir", buffer + cmd_start)) {
        cmd_dir();
        return;
    }
    if (string_equal_ci("raztools reboot", buffer + cmd_start)) {
        cmd_reboot();
        return;
    }
    if (string_equal_ci("about", buffer + cmd_start)) {
        cmd_about();
        return;
    }
    if (string_equal_ci("history", buffer + cmd_start)) {
        cmd_history();
        return;
    }

    if (string_equal_ci("calc.exr", buffer + cmd_start)) {
        calc();
        return;
    }

    if (string_equal_ci("raztools dritest bug", buffer + cmd_start)) {
        check_if_bugcheck_function_exists();
        return;
    }

    if (string_equal_ci("raztools dritest vga", buffer + cmd_start)) {
        test_vgaf();

        return;
    }
    if (string_equal_ci("raztools dritest keyboard", buffer + cmd_start)) {
        keyboardtest();
        return;
    }
    if (string_equal_ci("raztools dritest idt", buffer + cmd_start)) {
        test();
        return;
    }
    if (string_equal_ci("raztools dritest layout", buffer + cmd_start)) {
        layoutswitcher_test();
        
        return;
    }

    if (string_equal_ci("MILESTONE INFO", buffer + cmd_start)) {
        printv("Razzle Technology RT2 Build ");
    print_int(build);
    printv(".");
    print_int(version);
    printv(" -  LAST MILESTONE 1 BUILD.\n");
        return;
    }

    if (string_equal_ci("raztools dritest", buffer + cmd_start)) {
        printv("Available driver tests:\n");
        printv(" - raztools dritest bug: Test the bugcheck driver.\n");
        printv(" - raztools dritest vga: Test the VGA fallback driver.\n");
        printv(" - raztools dritest keyboard: Test the keyboard driver.\n");
        printv(" - raztools dritest idt: Test the IDT setup.\n");
        printv(" - raztools dritest layout: Test the layout switcher.\nCopyright (c) jad 2026 Driver Tester (DriTest)\n");

        return;
    }

    if (string_equal_ci("raztools administrative triple-fault", buffer + cmd_start)) {
        tripletest_and_test_idt();
        return;
    }
    /*
    usage: bugcheck <error code>

    how?

    it take your buffer
    it obvious

    like

    bugcheck hehehhehe
    */
    if (string_equal_ci("bugcheck", buffer + cmd_start)) {
        while (buffer[i] == ' ') i++;
            bugcheck(buffer + i);
        return;
    }
    printv_color("This is not recognized as an internal or external command,\nnor a operable program.\n", 0x04);
}

/* Main entry point for the Shell environment */
void shell_main() {
    char buffer[BUFFER_SIZE];
    // history_nav_offset: 0 for current input, 1 for newest history, 2 for second newest, etc.
    int history_nav_offset = 0; 
    int buf_idx = 0;

    // Initialize all history entries to empty strings
    for (int i = 0; i < HISTORY_SIZE; i++) {
        history[i][0] = '\0';
    }

    keyboard_init();
    layout_screen_init();

    char top_line[80];
    int top_len = 0;
    const char* title = "                            Razzle ";
    while (*title) top_line[top_len++] = *title++;
    top_len += int_to_str(version, top_line + top_len);
    const char* mid = " build ";
    while (*mid) top_line[top_len++] = *mid++;
    top_len += int_to_str(build, top_line + top_len);
    top_line[top_len] = '\0';

    uprint(top_line);
    printv("\n\n");

    printv_color("              +---------------------------------------------+\n", 0x0A);

    printv_color("              |", 0x0A);
    printv("              Razzle Shell                   ");
    printv_color("|\n", 0x0A);
    printv_color("              |", 0x0A);
    printv_color("                                             ", 0x0F);
    printv_color("|\n", 0x0A);
    printv_color("              |", 0x0A);
    printv_color("       (c) 2026 Jad. All rights reserved     ", 0x0F);
    printv_color("|\n", 0x0A);
    printv_color("              |", 0x0A);
    printv_color("      Licensed under the GPL v3 License      ", 0x0F);
    printv_color("|\n", 0x0A);
    printv_color("              |", 0x0A);
    printv_color("                                             ", 0x0F);
    printv_color("|\n", 0x0A);
    printv_color("              +---------------------------------------------+\n", 0x0A);

    printv_color(" Sucessfully loaded shell! Welcome to Razzle build ", 0xA0);
    
    print_int(build);
    printv_color("! type some stuffs here ! \n\n", 0xA0);
    while (1) {
        colortext(0x0E);
        printv("R:\\> ");
        colortext(0x0F);

        buf_idx = 0;
        buffer[0] = '\0';
        history_nav_offset = 0; // reset navigation offset for a new input line

        while (1) {
            unsigned char key = get_key();
            if (key) {
                if (key == '\r' || key == '\n') {
                    buffer[buf_idx] = '\0';
                    vga_putc('\n');
                    break;
                } else if (key == '\b' && buf_idx > 0) { // Handling backspace
                    buf_idx--;
                    vga_putc('\b');
                    vga_putc(' ');
                    vga_putc('\b');
                } else if (key == 0x48) { // uup Arrow
                    if (history_nav_offset < history_count) { // Check if there are older commands to show
                        history_nav_offset++;
                        
                        // clear the current shit on screen
                        for (int i = 0; i < buf_idx; i++) {
                            vga_putc('\b');
                            vga_putc(' ');
                            vga_putc('\b');
                        }
                        
                        // CALCULATE the actual index in the circular history buffer
                        int actual_history_idx = (history_write_ptr - history_nav_offset + HISTORY_SIZE) % HISTORY_SIZE;
                        
                        // lood the coomand from historyu and pritn onn the screen
                        buf_idx = 0;
                        while (history[actual_history_idx][buf_idx]) {
                            buffer[buf_idx] = history[actual_history_idx][buf_idx];
                            vga_putc(buffer[buf_idx]);
                            buf_idx++;
                        }
                        buffer[buf_idx] = '\0'; // null termibnate the buffrer after loading
                    }
                } else if (key == 0x50) { // down errow
                    if (history_nav_offset > 0) { // 
                        history_nav_offset--;
                        
                        // clear the current line on screen
                        for (int i = 0; i < buf_idx; i++) {
                            vga_putc('\b');
                            vga_putc(' ');
                            vga_putc('\b');
                        }
                        
                        if (history_nav_offset == 0) {
                            // if history nav offset becomes 0, it means we re back to the empty input line
                            buf_idx = 0;
                            buffer[0] = '\0'; // Clear the buffer
                        } else {
                            int actual_history_idx = (history_write_ptr - history_nav_offset + HISTORY_SIZE) % HISTORY_SIZE;
                            
                            buf_idx = 0;
                            while (history[actual_history_idx][buf_idx]) {
                                buffer[buf_idx] = history[actual_history_idx][buf_idx];
                                vga_putc(buffer[buf_idx]);
                                buf_idx++;
                            }
                            buffer[buf_idx] = '\0'; // null terminate the bufffet after loading
                        }
                    }
                } else if (buf_idx < BUFFER_SIZE - 1) {
                    // Store and echo printable characters
                    buffer[buf_idx++] = (char)key;
                    vga_putc((char)key);
                }
            }
        }

        // Only add non-empty commands to history
        if (buf_idx > 0) {
            // Copy the current command to the history buffer at history_write_ptr
            // Include the null terminator in the copy
            for (int i = 0; i <= buf_idx; i++) { 
                history[history_write_ptr][i] = buffer[i];
            }
            
            /*
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES
            FUCK YOU SCAN CODES            FUCK YOU SCAN CODES
            fuck YOUUUUUUUUUUUUUUUUUUU
            I LOST MY SANITY ON A FUCKING ARROW FUUUUUUUUUUUUUUUUUUUCK YOOOOOU 
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            FUCK YOU PS/2
            IDK HOW THE FUCK PEOPLE DID WITH USB SHIT....

            
            */
            if (history_count < HISTORY_SIZE) {
                history_count++;
            }
            
            // Move the write pointer foward uhh into the circle shit
            history_write_ptr = (history_write_ptr + 1) % HISTORY_SIZE;
        }
        
        shell_execute_command(buffer);
    }
}       
