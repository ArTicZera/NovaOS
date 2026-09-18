extern tract_key

kb_isr:
    pusha
    call kb_hue
    mov al, 0x20
    out 0x20, al
    popa
    iret

kb_hue:
    mov cl, 0x01
    
.loop_lines:
    mov dx, 0x61
    mov al, cl
    out dx, al

    mov dx, 0x60
    in al, dx
    
    cmp al, 0
    jne .find_key

    shl cl, 1
    cmp cl, 0x10
    jne .loop_lines
    ret

.find_key:
    push eax
    push ecx
    call tract_key
    add esp, 8
    ret
