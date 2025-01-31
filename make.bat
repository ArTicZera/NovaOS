@ECHO OFF

cls

echo "Compiling ASM Files"
nasm -fbin Bootloader/boot.asm   -o Binaries/boot.bin
nasm -felf Interrupts/idt.asm    -o Binaries/asmidt.o 
nasm -felf GDT/gdt.asm           -o Binaries/asmgdt.o

echo "Compiling C Files"
i686-elf-gcc -ffreestanding -m32 -g -c Kernel/kernel.c       -o Binaries/kernel.o
i686-elf-gcc -ffreestanding -m32 -g -c Kernel/panic.c        -o Binaries/panic.o
i686-elf-gcc -ffreestanding -m32 -g -c GDT/gdt.c             -o Binaries/gdt.o
i686-elf-gcc -ffreestanding -m32 -g -c Graphics/graphics.c   -o Binaries/graphics.o
i686-elf-gcc -ffreestanding -m32 -g -c Include/ports.c       -o Binaries/ports.o
i686-elf-gcc -ffreestanding -m32 -g -c Include/math.c        -o Binaries/math.o
i686-elf-gcc -ffreestanding -m32 -g -c Font/text.c           -o Binaries/text.o
i686-elf-gcc -ffreestanding -m32 -g -c Memory/mem.c          -o Binaries/mem.o
i686-elf-gcc -ffreestanding -m32 -g -c Memory/alloc.c        -o Binaries/alloc.o
i686-elf-gcc -ffreestanding -m32 -g -c Memory/vmm.c          -o Binaries/vmm.o
i686-elf-gcc -ffreestanding -m32 -g -c Interrupts/idt.c      -o Binaries/idt.o
i686-elf-gcc -ffreestanding -m32 -g -c Timer/timer.c         -o Binaries/timer.o
i686-elf-gcc -ffreestanding -m32 -g -c Drivers/keyboard.c    -o Binaries/keyboard.o
i686-elf-gcc -ffreestanding -m32 -g -c Drivers/mouse.c       -o Binaries/mouse.o
i686-elf-gcc -ffreestanding -m32 -g -c Drivers/qemu.c        -o Binaries/qemu.o
i686-elf-gcc -ffreestanding -m32 -g -c Network/net.c         -o Binaries/net.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/cpu.c        -o Binaries/cpu.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/pci.c        -o Binaries/pci.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/disk.c       -o Binaries/disk.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/cmos.c       -o Binaries/cmos.o
i686-elf-gcc -ffreestanding -m32 -g -c FileSystem/memfs.c    -o Binaries/memfs.o
i686-elf-gcc -ffreestanding -m32 -g -c Shell/shell.c         -o Binaries/shell.o
i686-elf-gcc -ffreestanding -m32 -g -c ELF/elf.c             -o Binaries/elf.o

i686-elf-gcc -ffreestanding -m32 -g -c Userspace/userspace.c -o Binaries/userspace.o
i686-elf-gcc -ffreestanding -m32 -g -c Userspace/GUI/gui.c   -o Binaries/gui.o
i686-elf-gcc -ffreestanding -m32 -g -c Userspace/login.c     -o Binaries/login.o

i686-elf-gcc -ffreestanding -m32 -g -c Programs/starfield.c  -o Programs/Binaries/starfield.o
i686-elf-gcc -ffreestanding -m32 -g -c Programs/scroll.c     -o Programs/Binaries/scroll.o

echo "Linking Programs"
i686-elf-ld -Ttext 0x100000 Programs/Binaries/starfield.o Binaries/graphics.o Binaries/math.o -o Programs/Binaries/stars.elf
i686-elf-ld -Ttext 0x100000 Programs/Binaries/scroll.o Binaries/graphics.o Binaries/text.o Binaries/math.o -o Programs/Binaries/scroll.elf

echo "Making Kernel Entry"
nasm -felf Kernel/entry.asm -o Binaries/entry.o

echo "Linking ELF Files"
i686-elf-ld -o Binaries/fullkernel.bin -Ttext 0x7E00 Binaries/entry.o Binaries/kernel.o Binaries/panic.o Binaries/asmgdt.o Binaries/gdt.o Binaries/graphics.o Binaries/ports.o Binaries/math.o Binaries/text.o Binaries/mem.o Binaries/alloc.o Binaries/vmm.o Binaries/asmidt.o Binaries/idt.o Binaries/timer.o Binaries/keyboard.o Binaries/cpu.o Binaries/pci.o Binaries/disk.o Binaries/cmos.o Binaries/mouse.o Binaries/qemu.o Binaries/net.o Binaries/memfs.o Binaries/elf.o Binaries/shell.o Binaries/gui.o Binaries/userspace.o Binaries/login.o --oformat binary

echo "Mounting IMG"
cat Binaries/boot.bin Binaries/fullkernel.bin > "NovaOS.img"

echo "Running NovaOS (WITH RTL8139 NETWORK DRIVER)"
qemu-system-i386 -net nic,model=rtl8139 -net user -drive format=raw,file="NovaOS.img" 
