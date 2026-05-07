#include "idt.h"
#include "../include/bugcheck.h"
#include "../drivers/VGAf/vgaf.h"

idt_entry_t idt[256];
idt_ptr_t idtp;

static void idt_load(void) {
    __asm__ __volatile__("lidt %0" : : "m"(idtp));
}

void idt_set_gate(unsigned char num, uint32_t base, unsigned short sel, unsigned char flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

static void validate_idt(void) {
    for (int i = 0; i < 32; i++) {
        if (idt[i].flags == 0 || (idt[i].base_low == 0 && idt[i].base_high == 0)) {
            bugcheck("IDT exception handler missing");
        }
    }
}

void init_idt() {
    idtp.limit = (sizeof(idt_entry_t) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    if (idtp.base == 0 || idtp.limit != (sizeof(idt_entry_t) * 256) - 1) {
        bugcheck("Invalid IDT descriptor");
    }

    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    validate_idt();
    idt_load();
}

void test(void) {
    printv("IDT test driver operation had been successful.\nCopyright (c) jad 2026 Driver Tester (DriTest)\n");
}