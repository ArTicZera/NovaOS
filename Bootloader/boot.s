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

align 16

section .text

global _start

_start:
        push    eax
        push    ebx

        xor     ebp, ebp

        extern  main
        call    main

HaltKernel:
        hlt

        jmp     HaltKernel

section .data
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

;Animations
;ELF Programs
;[GLOBAL    bbeatSize]
;[GLOBAL     bytebeat]
;[GLOBAL badappleSize]
;[GLOBAL     badapple]
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
        txt: incbin "Include/Icons/txt.bmp"
        dir: incbin "Include/Icons/dir.bmp"
        exe: incbin "Include/Icons/exe.bmp"
        non: incbin "Include/Icons/default.vad"

section .bss

align 16
