CC = gcc
AS = as
NASM = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Wpedantic -Wshadow -Wconversion
ASFLAGS = --32
NASMFLAGS = -f elf32 -Wall -Werror
LDFLAGS = -m elf_i386 -T src/linker.ld -Map=system.map

OBJ = Boot/boot.o src/system/system.o src/system/idt.o src/drivers/VGAf/vgaf.o src/drivers/KEYdriver/key.o src/Shell_old/Shell_old.o src/Shell_old/layout_screen.o src/RazzleFS/ReqFiles/sata.o src/include/bugcheck.o src/branding/branding.o

.PHONY: buildinfo

all: system.bin

system.bin: buildinfo $(OBJ) src/linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

buildinfo:
	python3 scripts/update_build.py version.txt

src/system/system.o: src/system/system.c
	$(CC) $(CFLAGS) -c $< -o $@

src/RazzleFS/ReqFiles/sata.o: src/RazzleFS/ReqFiles/sata.c
	$(CC) $(CFLAGS) -c $< -o $@

Boot/boot.o: Boot/boot.s
	$(AS) $(ASFLAGS) $< -o $@

src/system/idt.o: src/system/idt.c
	$(CC) $(CFLAGS) -c $< -o $@

src/drivers/VGAf/vgaf.o: src/drivers/VGAf/vgaf.c
	$(CC) $(CFLAGS) -c $< -o $@

src/drivers/KEYdriver/key.o: src/drivers/KEYdriver/key.c
	$(CC) $(CFLAGS) -c $< -o $@

src/drivers/KEYdriver/keyboard.o: src/drivers/KEYdriver/keyboard.s
	$(AS) $(ASFLAGS) $< -o $@

src/include/bugcheck.o: src/include/bugcheck.c
	$(CC) $(CFLAGS) -c $< -o $@

src/branding/branding.o: src/branding/branding.c version.txt
	$(CC) $(CFLAGS) -DRAZZLE_VERSION=$$(cat version.txt) -c $< -o $@

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

x86py: clean
	python3 ./src/scripts/update_build.py version.txt
	python3 ./src/scripts/build.py
	all iso
	qemu-system-i386 -cdrom razzle.iso
clean:

	rm -f $(OBJ) system.bin razzle.iso ./*.o ./Shell_old/Shell_old.o ./src/system/system.o ./src/system/idt.o ./src/drivers/VGAf/*.o ./src/drivers/KEYdriver/*.o ./src/RazzleFS/ReqFiles/*.o ./src/include/bugcheck.o ./iso/boot/*.o ./iso/boot/*.bin ./iso/boot/*.map

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