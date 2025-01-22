;Coded by mell-o-tron

[BITS        32]
[GLOBAL LoadIDT]

LoadIDT:
        mov     eax, [esp + 4]
        lidt    [eax]

        sti
        ret

%macro ISRNOERRORCODE 1
        global isr%1

        isr%1:
                cli
                push    long 0
                push    long %1
                jmp     ISRCommonStub
%endmacro

%macro ISRERRORCODE 1
        global isr%1

        isr%1:
                cli
                push    long %1
                jmp     ISRCommonStub
%endmacro

%macro IRQ 2
        global irq%1

        irq%1:
                cli
                push    long 0
                push    long %2
                jmp     IRQCommonStub
%endmacro

ISRNOERRORCODE 0
ISRNOERRORCODE 1
ISRNOERRORCODE 2
ISRNOERRORCODE 3
ISRNOERRORCODE 4
ISRNOERRORCODE 5
ISRNOERRORCODE 6
ISRNOERRORCODE 7

ISRERRORCODE 8
ISRNOERRORCODE 9 
ISRERRORCODE 10
ISRERRORCODE 11
ISRERRORCODE 12
ISRERRORCODE 13
ISRERRORCODE 14
ISRNOERRORCODE 15
ISRNOERRORCODE 16
ISRNOERRORCODE 17
ISRNOERRORCODE 18
ISRNOERRORCODE 19
ISRNOERRORCODE 20
ISRNOERRORCODE 21
ISRNOERRORCODE 22
ISRNOERRORCODE 23
ISRNOERRORCODE 24
ISRNOERRORCODE 25
ISRNOERRORCODE 26
ISRNOERRORCODE 27
ISRNOERRORCODE 28
ISRNOERRORCODE 29
ISRNOERRORCODE 30
ISRNOERRORCODE 31
ISRNOERRORCODE 128
ISRNOERRORCODE 177

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

[EXTERN ISRHandler]

ISRCommonStub:
        pusha

        mov     eax, ds

        push    eax

        mov     eax, cr2

        push    eax

        mov     ax, 0x10
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax

        push    esp

        call    ISRHandler

        add     esp, 0x08

        pop     ebx

        mov     ds, bx
        mov     es, bx
        mov     fs, bx
        mov     gs, bx

        popa

        add     esp, 0x08

        sti
        iret

[EXTERN IRQHandler]

IRQCommonStub:
        pusha

        mov     eax, ds

        push    eax

        mov     eax, cr2

        push    eax

        mov     ax, 0x10
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax

        push    esp

        call    IRQHandler

        add     esp, 0x08
        
        pop     ebx

        mov     ds, bx
        mov     es, bx
        mov     fs, bx
        mov     gs, bx

        popa

        add     esp, 0x08

        sti
        iret
