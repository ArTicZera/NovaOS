;Flags
MBOOT_PAGE_ALIGN EQU 1 << 0
MBOOT_MEM_INFO   EQU 1 << 1
MBOOT_USE_GFX    EQU 1 << 2

;Magic number used to identify the header
MBOOT_MAGIC      EQU 0x1BADB002

;Combine all flags
MBOOT_FLAGS      EQU MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_USE_GFX

;Must make the sum of the first three fields equal zero
MBOOT_CHECKSUM   EQU -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot

align 4
        DD MBOOT_MAGIC
        DD MBOOT_FLAGS
        DD MBOOT_CHECKSUM
        DD 0, 0, 0, 0, 0

        DD 1    ;Graphical mode requested
        DD 1280 ;Width
        DD 720  ;Height
        DD 32   ;Color depth

align 16

section .text

global _start

_start:
        ;Provides Multiboot Magic Number
        push    eax

        ;Provides address of Multiboot struct
        push    ebx

        ;Prevents containing an unknown value
        xor     ebp, ebp

        extern  kmain
        call    kmain

HaltKernel:
        hlt

        jmp     HaltKernel

;TODO: Make all icons
section .data
;Images
[GLOBAL backgrd]

;General Icons
[GLOBAL     error]
[GLOBAL   warning]
[GLOBAL  question]
[GLOBAL      info]
[GLOBAL      user]
[GLOBAL     user2]

[GLOBAL        doom]
[GLOBAL  ClassiCube]
[GLOBAL      bootup]

        ;Images
        backgrd: incbin "Include/bg.bmp"
        
        ;sprite: incbin "Include/Icons/gridicons.bmp"
        

        ;General Icons
        ;error:    incbin "Include/Icons/error.vad"
        ;warning:  incbin "Include/Icons/warning.vad"
        ;question: incbin "Include/Icons/question.vad"
        ;info:     incbin "Include/Icons/info.vad"
        ;user:     incbin "Include/Icons/user1.vad"
        user2:    incbin "Include/Icons/user2.vad"
        ;terminal: incbin "Include/Icons/terminal.vad"

        ;File Extensions
        txt: incbin "Include/Icons/txt.bmp"
        ;exe: incbin "Include/Icons/exe.bmp"
        non: incbin "Include/Icons/default.vad"

        doom:       incbin "Bootloader/doomgeneric"
        ClassiCube: incbin "Bootloader/ClassiCube.elf"

        bootup:   incbin "Include/bootup.pcm"

section .bss

align 16

;Tell the linker that this object doesnt contain an executable stack
section .note.GNU-stack noalloc noexec nowrite progbits
