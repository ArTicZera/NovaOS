;Coded by ArTic/JhoPro
;
;In this part, we prepares the system to be in Protected Mode! 
;We change the data registers from GDT Data Segment. And also
;important: the Stack.


[BITS    32]

pmode:
        mov     ax, DATASEG

        ;Setup Data Again
        ;(Now with GDT)
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax

        ;Setup x86 Stack
        mov     ebp, 0x90000
        mov     esp, ebp
        mov     ss, ax

        jmp     KERNEL
