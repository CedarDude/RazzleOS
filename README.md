# Razzle
Razzle Operating System.
# Razzle 1

## Features

- Color-coded command output (16 colors)
- Command history with up/down arrow navigation (10 commands)
- AZERTY keyboard layout support
- Case-insensitive command parsing
- Interactive shell

## stuff:
If you want to get the QWERTZ layout, just open layout_screen.c and key.c and remove the ```//``` comments, if it don't work or
require more info, please use `jad.helpabout@gmail.com`

## How to Build

### Requirements
- GCC (i386 cross-compiler or multilib support)
- GNU Assembler (gas)
- GNU Linker (ld)
- GRUB tools (grub-mkrescue)
- Make
- WSL/Linux environment recommended

### Build Steps

1. **Navigate to project directory**
2. **Build kernel and ISO image**
   ```bash
   make x86
   ```
## How to Run

### QEMU Emulation (Recommended for Testing)

```bash
qemu-system-i386 -cdrom razzle.iso
```

### Real Hardware

1. Insert USB drive
2. Burn ISO to USB:
   ```bash
   sudo dd if=razzle.iso of=/dev/sdX bs=4M
   ```
3. Boot from USB drive (change BIOS boot order)


## OR use rufus
Click on rufus software.
Chose the USB and ISO
then use DD, not ISO mode thing.


### Virtual Machine

- **VirtualBox**: Create new VM, use `razzle.iso` as boot media
- **VMware**: Attach `razzle.iso` as CD/DVD drive
- **Hyper-V**: Create a VM, attach an ISO, and set to Legacy Boot
## Version Information

- **Shell Version**: 0.1 BETA
- **Build**: 150
- **Last Updated**: April 2026
- **Copyright**: (c) 2026 Jad
- **License**: GNU General Public License (GPL)


