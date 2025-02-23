;Coded by ArTic/JhoPro
;
;Here it's the entry of our kernel! We can call a C function from here
;and as you can see, load a bunch of data to be used by the kernel,
;since images, icons, programs, etc.

[BITS        32]
[EXTERN    main]

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

section .text
        global  _start

_start:
        call    main

        jmp     $

section .data
        ;Images
        bootscr: incbin "Include/bootscr.vad"
        backgrd: incbin "Include/background.vad"

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
