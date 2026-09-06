;No brackets
;BITS 32

msg: db 'TESTEABCDE'

label:
        mov     eax, 0x04 
        mov     edx, 10    
        mov     ecx, msg
        int     0x80

        ret