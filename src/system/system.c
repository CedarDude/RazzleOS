#include "../drivers/VGAf/vgaf.h"
#include "../drivers/KEYdriver/key.h"
#include "../Shell_old/Shell_old.h"

extern void init_idt(void);

static void boot_screen() {
    printv("Booting...\n");
    printv("COPYRIGHT JAD 2026\n\n");
    printv("razzle Operating System!\n\n");
    
}

void kernel_main() {
    vga_init(); // init the vga
    keyboard_init(); // init the keyboard
    init_idt(); // init the idt
    boot_screen();
    colorscreen(0x00);
    shell_main(); // start shell
}