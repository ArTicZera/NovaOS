clear

#Change here to your ELF compiler path
export PATH="$PATH:/home/artic/build-i686-elf/bin"

echo "Compiling ASM Files"
nasm -felf32 Bootloader/boot.s  -o Bootloader/boot.o
nasm -felf   GDT/gdt.asm        -o Binaries/asmgdt.o
nasm -felf   Interrupts/idt.asm -o Binaries/idt.o

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
i686-elf-gcc -ffreestanding -m32 -g -c Shell/npad.c          -o Binaries/npad.o
i686-elf-gcc -ffreestanding -m32 -g -c ELF/elf.c             -o Binaries/elf.o

echo "Compiling tinyGL Files"
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/zbuffer.c      -o Binaries/tinygl/zbuffer.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/init.c         -o Binaries/tinygl/init.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/zline.c        -o Binaries/tinygl/zline.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/zmath.c        -o Binaries/tinygl/zmath.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/ztriangle.c    -o Binaries/tinygl/ztriangle.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/msghandling.c  -o Binaries/tinygl/msghandling.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/ztext.c        -o Binaries/tinygl/ztext.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/zraster.c      -o Binaries/tinygl/zraster.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/zpostprocess.c -o Binaries/tinygl/zpostprocess.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/vertex.c       -o Binaries/tinygl/vertex.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/texture.c      -o Binaries/tinygl/texture.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/specbuf.c      -o Binaries/tinygl/specbuf.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/select.c       -o Binaries/tinygl/select.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/misc.c         -o Binaries/tinygl/misc.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/memory.c       -o Binaries/tinygl/memory.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/matrix.c       -o Binaries/tinygl/matrix.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/list.c         -o Binaries/tinygl/list.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/light.c        -o Binaries/tinygl/light.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/image_util.c   -o Binaries/tinygl/image_util.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/get.c          -o Binaries/tinygl/get.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/clip.c         -o Binaries/tinygl/clip.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/clear.c        -o Binaries/tinygl/clear.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/arrays.c       -o Binaries/tinygl/arrays.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/api.c          -o Binaries/tinygl/api.o
i686-elf-gcc -ffreestanding -m32 -g -c tinygl/src/accum.c        -o Binaries/tinygl/accum.o

#echo "Generating Lib"
#gcc -fPIC -m32 -g -c Graphics/graphics.c -o ELF/lib/graphics.o
#gcc -fPIC -m32 -g -c Font/text.c -o ELF/lib/text.o
#gcc -m32 -shared -o ELF/lib/novalib.so ELF/lib/graphics.o ELF/lib/text.o

echo "Compiling tinyGL examples"
i686-elf-gcc -ffreestanding -m32 -g -c Kernel/gfx/gears.c -o Kernel/gfx/gears.o

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
i686-elf-ld -Ttext 0xC90000 Programs/Binaries/starfield.o Binaries/graphics.o Binaries/math.o -o Programs/Binaries/stars.elf
i686-elf-ld -Ttext 0xC90000 Programs/Binaries/scroll.o Binaries/graphics.o Binaries/text.o Binaries/math.o -o Programs/Binaries/scroll.elf
i686-elf-ld -Ttext 0xC90000 Programs/Binaries/bytebeat.o Binaries/ports.o Binaries/sb16.o -o Programs/Binaries/bytebeat.elf

echo "Linking Kernel"
ld -m elf_i386 -T Kernel/linker.ld -o fullkernel Bootloader/boot.o Binaries/kernel.o Binaries/panic.o Binaries/asmgdt.o Binaries/gdt.o Binaries/graphics.o Binaries/ports.o Binaries/math.o Binaries/text.o Binaries/mem.o Binaries/alloc.o Binaries/vmm.o Binaries/asmidt.o Binaries/idt.o Binaries/timer.o Binaries/keyboard.o Binaries/cpu.o Binaries/pci.o Binaries/disk.o Binaries/cmos.o Binaries/mouse.o Binaries/qemu.o Binaries/sb16.o Binaries/ata.o Binaries/netutils.o Binaries/iptcp.o Binaries/net.o Binaries/arp.o Binaries/icmp.o Binaries/memfs.o Binaries/elf.o Binaries/shell.o Binaries/npad.o Binaries/gui.o Binaries/userspace.o Binaries/login.o Binaries/desktop.o Binaries/tinygl/zbuffer.o Binaries/tinygl/init.o Binaries/tinygl/zline.o Binaries/tinygl/zmath.o Binaries/tinygl/ztriangle.o Binaries/tinygl/msghandling.o Binaries/tinygl/ztext.o Binaries/tinygl/zraster.o Binaries/tinygl/zpostprocess.o Binaries/tinygl/vertex.o Binaries/tinygl/texture.o Binaries/tinygl/specbuf.o Binaries/tinygl/select.o Binaries/tinygl/misc.o Binaries/tinygl/memory.o Binaries/tinygl/matrix.o Binaries/tinygl/list.o Binaries/tinygl/light.o Binaries/tinygl/image_util.o Binaries/tinygl/get.o Binaries/tinygl/clip.o Binaries/tinygl/clear.o Binaries/tinygl/arrays.o Binaries/tinygl/api.o Binaries/tinygl/accum.o Kernel/gfx/gears.o
mv fullkernel NovaOS/boot/kernel

echo "Generating ISO File"
grub-mkrescue -o NovaOS.iso NovaOS/

#Enable below if you're using linux
echo "Running NovaOS"
#qemu-img create -f raw disk.raw 256M
#qemu-system-x86_64 -device sb16 -net nic,model=rtl8139 -net user -drive file=NovaOS.iso,format=raw,if=ide,index=0 -drive file=disk.raw,format=raw,if=ide,index=2 #-d int
