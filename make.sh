clear

#Change here to your ELF compiler path
export PATH="$PATH:/home/artic/build-i686-elf/bin"

echo "Compiling ASM Files"
nasm -felf32 Bootloader/boot.s  -o Bootloader/boot.o
nasm -felf32 GDT/gdt.asm        -o Binaries/asmgdt.o
nasm -felf32 Interrupts/idt.asm -o Binaries/asmidt.o
#nasm -felf32 doom/libc/crt0.s   -o doom/libc/crt0.o

echo "Compiling C Files"
gcc -ffreestanding -m32 -g -c Kernel/kernel.c       -o Binaries/kernel.o
gcc -ffreestanding -m32 -g -c Kernel/panic.c        -o Binaries/panic.o
gcc -ffreestanding -m32 -g -c GDT/gdt.c             -o Binaries/gdt.o
gcc -ffreestanding -m32 -g -c Graphics/graphics.c   -o Binaries/graphics.o
gcc -ffreestanding -m32 -g -c Include/ports.c       -o Binaries/ports.o
gcc -ffreestanding -m32 -g -c Include/math.c        -o Binaries/math.o
gcc -ffreestanding -m32 -g -c Font/text.c           -o Binaries/text.o
gcc -ffreestanding -m32 -g -c Memory/mem.c          -o Binaries/mem.o
gcc -ffreestanding -m32 -g -c Memory/alloc.c        -o Binaries/alloc.o
gcc -ffreestanding -m32 -g -c Memory/vmm.c          -o Binaries/vmm.o
gcc -ffreestanding -m32 -g -c Interrupts/idt.c      -o Binaries/idt.o
gcc -ffreestanding -m32 -g -c Interrupts/syscall.c  -o Binaries/syscall.o
gcc -ffreestanding -m32 -g -c Timer/timer.c         -o Binaries/timer.o
gcc -ffreestanding -m32 -g -c Drivers/keyboard.c    -o Binaries/keyboard.o
gcc -ffreestanding -m32 -g -c Drivers/mouse.c       -o Binaries/mouse.o
gcc -ffreestanding -m32 -g -c Drivers/qemu.c        -o Binaries/qemu.o
gcc -ffreestanding -m32 -g -c Drivers/sb16.c        -o Binaries/sb16.o
gcc -ffreestanding -m32 -g -c Drivers/ata.c         -o Binaries/ata.o
gcc -ffreestanding -m32 -g -c Hardware/cpu.c        -o Binaries/cpu.o
gcc -ffreestanding -m32 -g -c Hardware/pci.c        -o Binaries/pci.o
gcc -ffreestanding -m32 -g -c Hardware/disk.c       -o Binaries/disk.o
gcc -ffreestanding -m32 -g -c Hardware/cmos.c       -o Binaries/cmos.o
gcc -ffreestanding -m32 -g -c FileSystem/memfs.c    -o Binaries/memfs.o
gcc -ffreestanding -m32 -g -c Shell/shell.c         -o Binaries/shell.o
gcc -ffreestanding -m32 -g -c Shell/npad.c          -o Binaries/npad.o
gcc -ffreestanding -m32 -g -c ELF/elf.c             -o Binaries/elf.o

echo "Compiling tinyGL Files"
gcc -ffreestanding -m32 -g -c tinygl/src/zbuffer.c      -o Binaries/tinygl/zbuffer.o
gcc -ffreestanding -m32 -g -c tinygl/src/init.c         -o Binaries/tinygl/init.o
gcc -ffreestanding -m32 -g -c tinygl/src/zline.c        -o Binaries/tinygl/zline.o
gcc -ffreestanding -m32 -g -c tinygl/src/zmath.c        -o Binaries/tinygl/zmath.o
gcc -ffreestanding -m32 -g -c tinygl/src/ztriangle.c    -o Binaries/tinygl/ztriangle.o
gcc -ffreestanding -m32 -g -c tinygl/src/msghandling.c  -o Binaries/tinygl/msghandling.o
gcc -ffreestanding -m32 -g -c tinygl/src/ztext.c        -o Binaries/tinygl/ztext.o
gcc -ffreestanding -m32 -g -c tinygl/src/zraster.c      -o Binaries/tinygl/zraster.o
gcc -ffreestanding -m32 -g -c tinygl/src/zpostprocess.c -o Binaries/tinygl/zpostprocess.o
gcc -ffreestanding -m32 -g -c tinygl/src/vertex.c       -o Binaries/tinygl/vertex.o
gcc -ffreestanding -m32 -g -c tinygl/src/texture.c      -o Binaries/tinygl/texture.o
gcc -ffreestanding -m32 -g -c tinygl/src/specbuf.c      -o Binaries/tinygl/specbuf.o
gcc -ffreestanding -m32 -g -c tinygl/src/select.c       -o Binaries/tinygl/select.o
gcc -ffreestanding -m32 -g -c tinygl/src/misc.c         -o Binaries/tinygl/misc.o
gcc -ffreestanding -m32 -g -c tinygl/src/memory.c       -o Binaries/tinygl/memory.o
gcc -ffreestanding -m32 -g -c tinygl/src/matrix.c       -o Binaries/tinygl/matrix.o
gcc -ffreestanding -m32 -g -c tinygl/src/list.c         -o Binaries/tinygl/list.o
gcc -ffreestanding -m32 -g -c tinygl/src/light.c        -o Binaries/tinygl/light.o
gcc -ffreestanding -m32 -g -c tinygl/src/image_util.c   -o Binaries/tinygl/image_util.o
gcc -ffreestanding -m32 -g -c tinygl/src/get.c          -o Binaries/tinygl/get.o
gcc -ffreestanding -m32 -g -c tinygl/src/clip.c         -o Binaries/tinygl/clip.o
gcc -ffreestanding -m32 -g -c tinygl/src/clear.c        -o Binaries/tinygl/clear.o
gcc -ffreestanding -m32 -g -c tinygl/src/arrays.c       -o Binaries/tinygl/arrays.o
gcc -ffreestanding -m32 -g -c tinygl/src/api.c          -o Binaries/tinygl/api.o
gcc -ffreestanding -m32 -g -c tinygl/src/accum.c        -o Binaries/tinygl/accum.o

echo "Compiling tinyGL examples"
gcc -ffreestanding -m32 -g -c Kernel/gfx/gears.c -o Kernel/gfx/gears.o

echo "Compiling Userspace Items"
gcc -ffreestanding -m32 -g -c Userspace/GUI/win.c   -o Binaries/win.o
gcc -ffreestanding -m32 -g -c Userspace/userspace.c -o Binaries/userspace.o
gcc -ffreestanding -m32 -g -c Userspace/desktop.c   -o Binaries/desktop.o
gcc -ffreestanding -m32 -g -c Userspace/GUI/gui.c   -o Binaries/gui.o
gcc -ffreestanding -m32 -g -c Userspace/login.c     -o Binaries/login.o

echo "Compiling Programs"
gcc -ffreestanding -m32 -g -c Programs/bytebeat.c   -o Programs/Binaries/bytebeat

#echo "Linking Programs"
#i686-elf-ld -Ttext 0xC90000 Programs/Binaries/bytebeat.o Binaries/ports.o Binaries/sb16.o -o Programs/Binaries/bytebeat.elf

echo "Linking Kernel"
ld -m elf_i386 -T Kernel/linker.ld -o fullkernel Bootloader/boot.o Binaries/kernel.o Binaries/panic.o Binaries/asmgdt.o Binaries/gdt.o Binaries/graphics.o Binaries/ports.o Binaries/math.o Binaries/text.o Binaries/mem.o Binaries/alloc.o Binaries/vmm.o Binaries/asmidt.o Binaries/idt.o Binaries/syscall.o Binaries/timer.o Binaries/keyboard.o Binaries/cpu.o Binaries/pci.o Binaries/disk.o Binaries/cmos.o Binaries/mouse.o Binaries/qemu.o Binaries/sb16.o Binaries/ata.o Binaries/memfs.o Binaries/elf.o Binaries/win.o Binaries/shell.o Binaries/npad.o Binaries/gui.o Binaries/userspace.o Binaries/login.o Binaries/desktop.o \
Binaries/tinygl/zbuffer.o Binaries/tinygl/init.o Binaries/tinygl/zline.o Binaries/tinygl/zmath.o Binaries/tinygl/ztriangle.o Binaries/tinygl/msghandling.o Binaries/tinygl/ztext.o Binaries/tinygl/zraster.o Binaries/tinygl/zpostprocess.o Binaries/tinygl/vertex.o Binaries/tinygl/texture.o Binaries/tinygl/specbuf.o Binaries/tinygl/select.o Binaries/tinygl/misc.o Binaries/tinygl/memory.o Binaries/tinygl/matrix.o Binaries/tinygl/list.o Binaries/tinygl/light.o Binaries/tinygl/image_util.o Binaries/tinygl/get.o Binaries/tinygl/clip.o Binaries/tinygl/clear.o Binaries/tinygl/arrays.o Binaries/tinygl/api.o Binaries/tinygl/accum.o Kernel/gfx/gears.o 
mv fullkernel NovaOS/boot/kernel

echo "Generating ISO File"
grub-mkrescue -o NovaOS.iso NovaOS/

#Enable below if you're using linux
#echo "Running NovaOS"
#qemu-img create -f raw disk.raw 256M
#qemu-system-x86_64 -device sb16 -netdev user,id=net0 -net nic,model=rtl8139,netdev=net0 -object filter-dump,id=f1,netdev=net0,file=net.pcap -serial stdio -drive file=NovaOS.iso,format=raw,if=ide,index=0 -drive file=disk.raw,format=raw,if=ide,index=2
