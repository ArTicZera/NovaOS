#include "../Include/stdint.h"
#include "../Font/text.h"
#include "idt.h"
#include "syscall.h"

void SyscallHandler(struct InterruptRegisters* regs)
{
    switch(regs->eax)
    {
        /* write */
        case 4:
        {
            const char* buf = (const char*)regs->ecx;
            size_t count = regs->edx;

            for(size_t i = 0; i < count; i++)
                PrintOut(buf[i], 0xFFFFFF00);

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
        case 3:
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

        /* exit */
        case 1:
        {
            //SysExit(regs->ebx);
            break;
        }

        default:
            regs->eax = -1;
            break;
    }
}