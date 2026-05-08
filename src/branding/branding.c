
/*
COPYRIGHT JAD 2026
Licensed under the GNU General Public License v3.0

branding file, don't wanna modify 300000+ files because of a name changing or smt.

*/

#include "../drivers/VGAf/vgaf.h"
// daily build counter?
//maybe
int build = RAZZLE_VERSION;
int version = 1;

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
    while (i > 0) {
        vga_putc(buf[--i]);
    }
}

static void print_int(int n) {
    if (n < 0) {
        vga_putc('-');
        print_uint((unsigned int)(-n));
    } else {
        print_uint((unsigned int)n);
    }
}

void RT_kernel_info(void) {
    printv("Razzle Technology RT2 Build ");
    print_int(build);
    printv(".");
    print_int(version);
    printv("\n");
}
