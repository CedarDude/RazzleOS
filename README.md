# Razzle
Razzle Operating System.

## Features
- Colored text output (16 colors)
- Command history with up/down arrow navigation (10 commands)
- AZERTY keyboard layout support
- Case-insensitive command parsing
- Interactive shell

## How to Build
### Requirements
- GNU Cross-Compiler (GCC), GNU Make and binutils for i386
- GRUB tools (grub-mkrescue, grub-pc-bin)
- Linux environment

### The Build Process

```bash
# Build for x86
make x86
```

## How to Run
### Real Hardware
#### Linux
1. Insert USB drive
2. Burn ISO to USB:
```bash
sudo dd if=razzle.iso of=/dev/sdX bs=4M
sync
```
3. Boot from the freshly written USB drive

#### Windows
Run Rufus. Choose the USB drive and Razzle ISO. Write the image using DD mode.

### Virtual Machine
- **VirtualBox**: Create new VM, use ISO as boot media
- **VMware**: Attach the ISO image to a CD/DVD drive
- **Hyper-V**: Create a VM, use the Razzle boot image, and set to Legacy Boot
- **QEMU**: `qemu-system-i386 -cdrom razzle.iso`

## Version Information
- **Shell Version**: 0.2 BETA
- **Build**: 450
- **Last Updated**: May 2026
- **Copyright**: (c) 2026 Jad
- **License**: GNU General Public License 3 (GPL v3)



