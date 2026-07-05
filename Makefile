# ======================================================
# NovaOS Makefile
# ======================================================

CC      = gcc
ASM     = nasm
LD      = ld

CFLAGS  = -ffreestanding -m32 -g
ASMFLAGS= -felf32
LDFLAGS = -m elf_i386 -T Kernel/linker.ld

BIN     = Binaries

# ------------------------------------------------------
# Source files
# ------------------------------------------------------

C_SRCS := $(shell find . -name "*.c" \
            -not -path "./DOOM/*")

ASM_SRCS := $(shell find . -name "*.asm" \
			-not -path "./NovaOS/*")

S_SRCS := $(shell find . -name "*.s")

# ------------------------------------------------------
# Object files
# ------------------------------------------------------

C_OBJS := $(patsubst ./%.c,$(BIN)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst ./%.asm,$(BIN)/%.o,$(ASM_SRCS))
S_OBJS := $(patsubst ./%.s,$(BIN)/%.o,$(S_SRCS))

OBJS := $(C_OBJS) $(ASM_OBJS) $(S_OBJS)

BOOT_OBJ = Bootloader/boot.o

S_OBJS := $(filter-out $(BIN)/Bootloader/boot.o,$(S_OBJS))

OBJS := $(C_OBJS) $(ASM_OBJS) $(S_OBJS)

GEARS_OBJ = Kernel/gfx/gears.o
OBJS := $(filter-out $(BIN)/Kernel/gfx/gears.o,$(OBJS))

# ------------------------------------------------------

all: kernel iso

remakeboot:
	rm -f doomgeneric
	rm -f boot.o
	nasm -felf32 Bootloader/boot.s -o Bootloader/boot.o

# ------------------------------------------------------
# Compile C
# ------------------------------------------------------

$(BIN)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "\033[1;34m[CC]\033[0m %s\n" "$<"
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------
# Compile ASM
# ------------------------------------------------------

$(BIN)/%.o: %.asm
	@mkdir -p $(dir $@)
	@printf "\033[1;36m[ASM]\033[0m %s\n" "$<"
	$(ASM) $(ASMFLAGS) $< -o $@

# ------------------------------------------------------
# Compile .s
# ------------------------------------------------------

$(BOOT_OBJ): Bootloader/boot.s
	@printf "\033[1;32m[BOOT]\033[0m %s\n" "$<"
	$(ASM) $(ASMFLAGS) $< -o $@

$(BIN)/%.o: %.s
	@mkdir -p $(dir $@)
	@printf "\033[1;36m[ASM]\033[0m %s\n" "$<"
	$(ASM) $(ASMFLAGS) $< -o $@

# ------------------------------------------------------
# gears
# ------------------------------------------------------

$(GEARS_OBJ): Kernel/gfx/gears.c
	@printf "\033[1;34m[CC]\033[0m %s\n" "$<"
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------
# Link
# ------------------------------------------------------

kernel: $(BOOT_OBJ) $(GEARS_OBJ) $(OBJS)
	@printf "\033[1;33m[Linking]\033[0m\n"
	@$(LD) $(LDFLAGS) -o fullkernel \
		$(BOOT_OBJ) \
		$(OBJS) \
		$(GEARS_OBJ)

	mv fullkernel NovaOS/boot/kernel

# ------------------------------------------------------
# ISO
# ------------------------------------------------------

iso: kernel
	grub-mkrescue -o NovaOS.iso NovaOS/

# ------------------------------------------------------
# Run
# ------------------------------------------------------

run: iso
	qemu-system-x86_64 \
		-device sb16 \
		-netdev user,id=net0 \
		-net nic,model=rtl8139,netdev=net0 \
		-object filter-dump,id=f1,netdev=net0,file=net.pcap \
		-serial stdio \
		-drive file=NovaOS.iso,format=raw,if=ide,index=0 \
		-drive file=disk.raw,format=raw,if=ide,index=2

# ------------------------------------------------------
# Clean
# ------------------------------------------------------

clean:
	rm -rf Binaries
	rm -f Bootloader/boot.o
	rm -f Kernel/gfx/gears.o
	rm -f fullkernel
	rm -f NovaOS.iso

.PHONY: all kernel iso run clean
