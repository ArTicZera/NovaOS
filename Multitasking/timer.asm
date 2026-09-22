[BITS 32]

[GLOBAL TimerIRQ]

[EXTERN Scheduler]
[EXTERN PITIRQ0]

TimerIRQ:

    pusha

    push    ds
    push    es
    push    fs
    push    gs

    mov     ax, 0x10

    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    call    PITIRQ0

    push    esp
    call    Scheduler
    add     esp, 4

    mov     edx, eax

    mov     al, 0x20
    out     0x20, al

    mov     esp, edx

    pop     gs
    pop     fs
    pop     es
    pop     ds

    popa

    iret

section .note.GNU-stack noalloc noexec nowrite progbits
