MBOOT_PAGE_ALIGN EQU 1 << 0
MBOOT_MEM_INFO   EQU 1 << 1
MBOOT_USE_GFX    EQU 1 << 2

MBOOT_MAGIC      EQU 0x1BADB002
MBOOT_FLAGS      EQU MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_USE_GFX
MBOOT_CHECKSUM   EQU -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot

align 4
        DD MBOOT_MAGIC
        DD MBOOT_FLAGS
        DD MBOOT_CHECKSUM
        DD 0, 0, 0, 0, 0

        DD 1
        DD 1280
        DD 720
        DD 32

section .bss

align 16

stackTop:

section .text

global _start

;Images
[GLOBAL bootscr]
[GLOBAL backgrd]

;General Icons
[GLOBAL     error]
[GLOBAL   warning]
[GLOBAL  question]
[GLOBAL      info]
[GLOBAL      user]
[GLOBAL     user2]
[GLOBAL  terminal]

;Extension Icons
[GLOBAL       non]
[GLOBAL       txt]
[GLOBAL       dir]
[GLOBAL       exe]

;ELF Programs
[GLOBAL  starsSize]
[GLOBAL      stars]
[GLOBAL scrollSize]
[GLOBAL     scroll]
[GLOBAL  bbeatSize]
[GLOBAL   bytebeat]

_start:
        push    ebx

        xor     ebp, ebp

        extern  main
        call    main

HaltKernel:
        hlt

        jmp     HaltKernel

section .data
        ;Images
        bootscr: incbin "Include/logo.bmp"
        backgrd: incbin "Include/bg.bmp"

        ;General Icons
        error:    incbin "Include/Icons/error.vad"
        warning:  incbin "Include/Icons/warning.vad"
        question: incbin "Include/Icons/question.vad"
        info:     incbin "Include/Icons/info.vad"
        user:     incbin "Include/Icons/user1.vad"
        user2:    incbin "Include/Icons/user2.vad"
        terminal: incbin "Include/Icons/terminal.vad"

        ;File Extensions
        txt: incbin "Include/Icons/txt.vad"
        dir: incbin "Include/Icons/dir.vad"
        exe: incbin "Include/Icons/exe.vad"
        non: incbin "Include/Icons/default.vad"

        ;Programs
        starsBase: equ $ - stars
        stars:     incbin "Programs/Binaries/stars.elf"
        stars_end: 
        starsSize: dd stars_end - stars

        scrollBase: equ $ - scroll
        scroll:     incbin "Programs/Binaries/scroll.elf"
        scroll_end: 
        scrollSize: dd scroll_end - scroll

        bytebeatBase: equ $ - bytebeat
        bytebeat:     incbin "Programs/Binaries/bytebeat.elf"
        bytebeat_end: 
        bbeatSize: dd bytebeat_end - bytebeat
