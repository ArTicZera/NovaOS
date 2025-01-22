;Coded by ArTic/JhoPro and jewelcodes
;
;The bootsector loads on the memory segment 0x7C00 and it has in total
;512 bytes long, but the last two is a identifier with a "MAGIC NUMBER"
;Which is 0xAA55. When the VM boots up it will check this number for boot.

[BITS    16]
[ORG 0x7C00]

%define KERNEL 0x7E00

BootMain:
        ;Clear Flags
        cli
        cld

        ;Setup Data
        xor     ax, ax
        mov     ds, ax
        mov     es, ax

        ;Setup Stack
        mov     sp, 0x7C00
        mov     ss, ax

        sti

        ;Setup VESA (640x480 8bpp)
        mov     ax, 0x4F02
        mov     bx, 0x4101
        int     0x10

        call    ReadSectors
        call    SetupGDT

        jmp     CODESEG:pmode

;---------------------------------------

ReadSectors:
        ;Counter
        mov     cx, 10

.loop:
        push    cx

        ;Read Extended Sectors
        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        ;Modify DAP for Next Read
        add     dword [DAP + 8], 127
        add     word  [DAP + 6], 0xFE0

        pop     cx

        ;Loop till CX goes 0x00
        loop    .loop

        ret

align 4

;Disk Address Packet
DAP:
        db      0x10        ;DAP     byte [dap+0]
        db      0x00        ;Unused  byte [dap+1]
        dw      0x7F        ;Sectors byte [dap+2]
        dw      0x0000      ;Segment byte [dap+3]
        dw      KERNEL >> 4 ;Offset  word [DAP+6]
        dq      1           ;LBA     word [DAP+8]

;---------------------------------------

%include "Bootloader/gdt.asm"
%include "Bootloader/pmode.asm"

;Fill 1 sector (510 bytes + Boot's Magic Number)
times 510 - ($ - $$) db 0x00
dw 0xAA55
