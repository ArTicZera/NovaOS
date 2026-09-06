/*
    Coded by ArTic/JhoPro

    Userspace syscalls
*/

#include "../Include/stdint.h"
#include "../Font/text.h"
#include "idt.h"
#include "syscall.h"

void SyscallHandler(struct InterruptRegisters* regs)
{
    switch(regs->eax)
    {
        /* exit */
        case 1:
            while (1)
            {
                asm volatile("cli");
                asm volatile("hlt");
            }
            break;

        /* putchar */
        case 3:
            const char c = (const char)regs->ebx;
            PrintOut(c, regs->ecx);
            break;

        /* write */
        case 4:
        {
            char* buffer = (char*)regs->ecx;
            int count = regs->edx;

            for(int i = 0; i < count; i++)
            {
                PrintOut(buffer[i], 0xFF00FF00);
            }

            regs->eax = count;

            break;
        }

        /* sbrk */
        case 45:
        {
            //regs->eax = (uint32_t)SysSbrk((int)regs->ebx);
            break;
        }

        /* read */
        case 7:
        {
            //regs->eax = SysRead(regs->ebx, (void*)regs->ecx, regs->edx);
            break;
        }

        /* open */
        case 5:
        {
            //regs->eax = SysOpen((const char*)regs->ebx, regs->ecx);
            break;
        }

        /* close */
        case 6:
        {
            //regs->eax = SysClose(regs->ebx);
            break;
        }

        /* lseek */
        case 19:
        {
            //regs->eax = SysLseek(regs->ebx, regs->ecx, regs->edx);
            break;
        }

        default:
            regs->eax = -1;
            break;
    }
}
