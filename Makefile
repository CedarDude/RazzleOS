CC = gcc
AS = as
NASM = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Wpedantic -Wshadow -Wconversion
ASFLAGS = --32
NASMFLAGS = -f elf32 -Wall -Werror
LDFLAGS = -m elf_i386 -T src/linker.ld -Map=system.map

OBJ = Boot/boot.o src/system/system.o src/system/idt.o src/system/idt_stubs.o src/drivers/VGAf/vgaf.o src/drivers/KEYdriver/key.o src/Shell_old/Shell_old.o src/Shell_old/layout_screen.o src/RazzleFS/ReqFiles/sata.o src/include/bugcheck.o
MEMOS_OBJ = memOS/boot.o memOS/kernel.o
MEMOS_LDFLAGS = -m elf_i386 -T memOS/linker.ld -nostdlib

all: system.bin

system.bin: $(OBJ) src/linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

memtest.bin: $(MEMOS_OBJ) memOS/linker.ld
	$(LD) $(MEMOS_LDFLAGS) -o $@ $(MEMOS_OBJ)

memos: memtest.bin

src/system/system.o: src/system/system.c
	$(CC) $(CFLAGS) -c $< -o $@

src/RazzleFS/ReqFiles/sata.o: src/RazzleFS/ReqFiles/sata.c
	$(CC) $(CFLAGS) -c $< -o $@

Boot/boot.o: Boot/boot.s
	$(AS) $(ASFLAGS) $< -o $@

src/system/idt.o: src/system/idt.c
	$(CC) $(CFLAGS) -c $< -o $@

src/system/idt_stubs.o: src/system/idt_stubs.s
	$(AS) $(ASFLAGS) $< -o $@

src/drivers/VGAf/vgaf.o: src/drivers/VGAf/vgaf.c
	$(CC) $(CFLAGS) -c $< -o $@

src/drivers/KEYdriver/key.o: src/drivers/KEYdriver/key.c
	$(CC) $(CFLAGS) -c $< -o $@

src/drivers/KEYdriver/keyboard.o: src/drivers/KEYdriver/keyboard.s
	$(AS) $(ASFLAGS) $< -o $@

src/include/bugcheck.o: src/include/bugcheck.c
	$(CC) $(CFLAGS) -c $< -o $@

src/Shell_old/Shell_old.o: src/Shell_old/Shell_old.c
	$(CC) $(CFLAGS) -c $< -o $@

src/Shell_old/layout_screen.o: src/Shell_old/layout_screen.c
	$(CC) $(CFLAGS) -c $< -o $@

iso: system.bin
	mkdir -p iso/boot
	cp -r Boot/grub iso/boot/
	cp system.bin iso/boot/kernel.bin
	cp system.bin iso/boot/kernel.bin
	grub-mkrescue -o razzle.iso iso/

x86: clean all iso 
	qemu-system-i386 -cdrom razzle.iso
clean:

	rm -f $(OBJ) system.bin razzle.iso memtest.bin memOS/*.o ./*.o ./Shell_old/Shell_old.o ./src/system/system.o ./src/system/idt.o ./src/drivers/VGAf/*.o ./src/drivers/KEYdriver/*.o ./src/RazzleFS/ReqFiles/*.o ./src/include/bugcheck.o ./iso/boot/*.o ./iso/boot/*.bin ./iso/boot/*.map

help:
	@echo "  █████████                                    ██                ███      ███             ██                 " 
	@echo " ░██     ░██                                  ░██               ░████   ░████            ░██                 " 
	@echo " ░██     ░██  ░██████   ░█████████ ░█████████ ░██  ░███████     ░██░██ ░██░██  ░██████   ░██    ░██ ░███████ " 
	@echo " ░█████████        ░██       ░███       ░███  ░██ ░██    ░██    ░██ ░████ ░██       ░██  ░██   ░██ ░██    ░██" 
	@echo " ░██   ░██    ░███████     ░███       ░███    ░██ ░█████████    ░██  ░██  ░██  ░███████  ░███████  ░█████████" 
	@echo " ░██    ░██  ░██   ░██   ░███       ░███      ░██ ░██           ░██       ░██ ░██   ░██  ░██   ░██ ░██       " 
	@echo " ░██     ░██  ░█████░██ ░█████████ ░█████████ ░██  ░███████     ░██       ░██  ░█████░██ ░██    ░██ ░███████ " 
	@echo
	@echo
	@echo "Usage: make [thing]"
	@echo "things:"
	@echo "  all     - Build the system binary"
	@echo "  iso     - Create a bootable ISO image"
	@echo "  x86     - create, build and run the system"
	@echo "  clean   - Remove build artifacts"
	@echo "  help    - Display this help message"