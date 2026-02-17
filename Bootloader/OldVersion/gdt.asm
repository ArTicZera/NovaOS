;Coded by ArTic/JhoPro and Daedalus
;
;This code loads a Global Descriptor Table, it allows to define memory 
;segments with many attributes and you can also separate the memory spaces 
;between the kernel and users and many more things. One of then too, is
;that it enables you to change to Protected Mode.

[BITS    16]

SetupGDT:
        ;Disable interrupts
        cli

        ;Load GDT Address
        lgdt    [GDTR]

        ;Enable Protected Mode
        mov     eax, cr0
        or      eax, 0x01
        mov     cr0, eax
        
        ret

GDT:
        ;Not Used
        GDT.Null:
                dd  0x00
                dd  0x00

        ;Code Segment
        GDT.Code:
                ;Segment Limit
                dw  0xFFFF

                ;Base
                dw  0x00
                db  0x00

                ;1001 1010b (Executable, R/W, Code Segment)
                db  0x9A

                ;Additional Attributes
                db  0xCF

                ;Reserved
                db  0x00

        GDT.Data:
                ;Segment Limit
                dw  0xFFFF

                ;Base
                dw  0x00
                db  0x00

                ;Specifies Data Segment
                db  0x92

                ;Additional Attributes
                db  0xCF

                ;Reserved
                db  0x00

;GDT Struct to Store Address and Limit
GDTR:
        dw GDTR - GDT - 1
        dd GDT

;Segments definitions
CODESEG equ GDT.Code - GDT
DATASEG equ GDT.Data - GDT
