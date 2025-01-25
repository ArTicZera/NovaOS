global gdt_flush

gdt_flush:
    MOV eax, [esp+4]
    LGDT [eax]

    MOV eax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax
    MOV ss, ax
    JMP 0x08:.flush
.flush:
    RET

global tss_flush
tss_flush:
    MOV ax, 0x2B
    LTR ax
    RET
