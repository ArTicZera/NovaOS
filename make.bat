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
i686-elf-gcc -ffreestanding -m32 -g -c Drivers/sb16.c        -o Binaries/sb16.o
i686-elf-gcc -ffreestanding -m32 -g -c Drivers/ata.c         -o Binaries/ata.o
i686-elf-gcc -ffreestanding -m32 -g -c Network/netutils.c    -o Binaries/netutils.o
i686-elf-gcc -ffreestanding -m32 -g -c Network/iptcp.c       -o Binaries/iptcp.o
i686-elf-gcc -ffreestanding -m32 -g -c Network/icmp.c        -o Binaries/icmp.o
i686-elf-gcc -ffreestanding -m32 -g -c Network/net.c         -o Binaries/net.o
i686-elf-gcc -ffreestanding -m32 -g -c Network/arp.c         -o Binaries/arp.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/cpu.c        -o Binaries/cpu.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/pci.c        -o Binaries/pci.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/disk.c       -o Binaries/disk.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/cmos.c       -o Binaries/cmos.o
i686-elf-gcc -ffreestanding -m32 -g -c FileSystem/memfs.c    -o Binaries/memfs.o
i686-elf-gcc -ffreestanding -m32 -g -c Shell/shell.c         -o Binaries/shell.o
i686-elf-gcc -ffreestanding -m32 -g -c ELF/elf.c             -o Binaries/elf.o

echo "Compiling NovaGL"
i686-elf-gcc -ffreestanding -m32 -g -c Graphics/novagl.c     -o Binaries/novagl.o

echo "Compiling Userspace Items"
i686-elf-gcc -ffreestanding -m32 -g -c Userspace/userspace.c -o Binaries/userspace.o
i686-elf-gcc -ffreestanding -m32 -g -c Userspace/desktop.c   -o Binaries/desktop.o
i686-elf-gcc -ffreestanding -m32 -g -c Userspace/GUI/gui.c   -o Binaries/gui.o
i686-elf-gcc -ffreestanding -m32 -g -c Userspace/login.c     -o Binaries/login.o

echo "Compiling Programs"
i686-elf-gcc -ffreestanding -m32 -g -c Programs/starfield.c  -o Programs/Binaries/starfield.o
i686-elf-gcc -ffreestanding -m32 -g -c Programs/bytebeat.c   -o Programs/Binaries/bytebeat.o
i686-elf-gcc -ffreestanding -m32 -g -c Programs/scroll.c     -o Programs/Binaries/scroll.o

echo "Linking Programs"
i686-elf-ld -Ttext 0x100000 Programs/Binaries/starfield.o Binaries/graphics.o Binaries/math.o -o Programs/Binaries/stars.elf
i686-elf-ld -Ttext 0x100000 Programs/Binaries/scroll.o Binaries/graphics.o Binaries/text.o Binaries/math.o -o Programs/Binaries/scroll.elf
i686-elf-ld -Ttext 0x100000 Programs/Binaries/bytebeat.o Binaries/ports.o Binaries/sb16.o -o Programs/Binaries/bytebeat.elf

echo "Making Kernel Entry"
nasm -felf Kernel/entry.asm -o Binaries/entry.o

echo "Linking ELF Files"
i686-elf-ld -o Binaries/fullkernel.bin -Ttext 0x7E00 Binaries/entry.o Binaries/kernel.o Binaries/panic.o Binaries/asmgdt.o Binaries/gdt.o Binaries/graphics.o Binaries/ports.o Binaries/math.o Binaries/text.o Binaries/mem.o Binaries/alloc.o Binaries/vmm.o Binaries/asmidt.o Binaries/idt.o Binaries/timer.o Binaries/keyboard.o Binaries/cpu.o Binaries/pci.o Binaries/disk.o Binaries/cmos.o Binaries/mouse.o Binaries/qemu.o Binaries/sb16.o Binaries/ata.o Binaries/netutils.o Binaries/iptcp.o Binaries/net.o Binaries/arp.o Binaries/icmp.o Binaries/memfs.o Binaries/elf.o Binaries/shell.o Binaries/novagl.o Binaries/gui.o Binaries/userspace.o Binaries/login.o Binaries/desktop.o --oformat binary

echo "Mounting IMG"
cat Binaries/boot.bin Binaries/fullkernel.bin > "NovaOS.img"

echo "Running NovaOS (WITH RTL8139 NETWORK DRIVER)"

qemu-img create -f raw disk.raw 256M
qemu-system-x86_64 -device sb16 -net nic,model=rtl8139 -net user -drive file=NovaOS.img,format=raw,if=ide,index=0 -drive file=disk.raw,format=raw,if=ide,index=2
