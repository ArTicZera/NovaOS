/*
    Coded by mell-o-tron

    I was getting triple fault with my IDT, so I just copied from 
    his repo. My bad, but that was too much harder for me.
*/


#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Font/text.h"
#include "../Memory/mem.h"
#include "../Kernel/panic.h"

#include "idt.h"

extern void LoadIDT(DWORD);

struct IDT_GateDescriptor idt[TOTALGATES];
struct IDT_Descriptor idtr;

void SetupIDTGate(BYTE entry, DWORD isr, WORD selector, BYTE flags)
{
    idt[entry].selector = selector;
    idt[entry].reserved = 0x00;
    idt[entry].flags = flags;

    idt[entry].offset  = isr & 0xFFFF; //Low 16 bits
    idt[entry].offset2 = (isr >> 16) & 0xFFFF; //High 16 bits
}

void SetupIDT(void)
{
    int IDTSize = sizeof(struct IDT_GateDescriptor) * TOTALGATES;

    idtr.size   = IDTSize - 1;
    idtr.offset = (DWORD) &idt;

    memset(&idt, 0, IDTSize);

    //PIC Init
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    //Interruptions Base Vector
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    //Link PIC with other
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    //Setup Operating Mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    //Setup PICs Mask
    outb(0x21, 0x00);
    outb(0xA1, 0x00);

    //Interrupt Service Routines
    SetupIDTGate(0, (DWORD)isr0,0x08, 0x8E);
    SetupIDTGate(1, (DWORD)isr1,0x08, 0x8E);
    SetupIDTGate(2, (DWORD)isr2,0x08, 0x8E);
    SetupIDTGate(3, (DWORD)isr3,0x08, 0x8E);
    SetupIDTGate(4, (DWORD)isr4, 0x08, 0x8E);
    SetupIDTGate(5, (DWORD)isr5, 0x08, 0x8E);
    SetupIDTGate(6, (DWORD)isr6, 0x08, 0x8E);
    SetupIDTGate(7, (DWORD)isr7, 0x08, 0x8E);
    SetupIDTGate(8, (DWORD)isr8, 0x08, 0x8E);
    SetupIDTGate(9, (DWORD)isr9, 0x08, 0x8E);
    SetupIDTGate(10, (DWORD)isr10, 0x08, 0x8E);
    SetupIDTGate(11, (DWORD)isr11, 0x08, 0x8E);
    SetupIDTGate(12, (DWORD)isr12, 0x08, 0x8E);
    SetupIDTGate(13, (DWORD)isr13, 0x08, 0x8E);
    SetupIDTGate(14, (DWORD)isr14, 0x08, 0x8E);
    SetupIDTGate(15, (DWORD)isr15, 0x08, 0x8E);
    SetupIDTGate(16, (DWORD)isr16, 0x08, 0x8E);
    SetupIDTGate(17, (DWORD)isr17, 0x08, 0x8E);
    SetupIDTGate(18, (DWORD)isr18, 0x08, 0x8E);
    SetupIDTGate(19, (DWORD)isr19, 0x08, 0x8E);
    SetupIDTGate(20, (DWORD)isr20, 0x08, 0x8E);
    SetupIDTGate(21, (DWORD)isr21, 0x08, 0x8E);
    SetupIDTGate(22, (DWORD)isr22, 0x08, 0x8E);
    SetupIDTGate(23, (DWORD)isr23, 0x08, 0x8E);
    SetupIDTGate(24, (DWORD)isr24, 0x08, 0x8E);
    SetupIDTGate(25, (DWORD)isr25, 0x08, 0x8E);
    SetupIDTGate(26, (DWORD)isr26, 0x08, 0x8E);
    SetupIDTGate(27, (DWORD)isr27, 0x08, 0x8E);
    SetupIDTGate(28, (DWORD)isr28, 0x08, 0x8E);
    SetupIDTGate(29, (DWORD)isr29, 0x08, 0x8E);
    SetupIDTGate(30, (DWORD)isr30, 0x08, 0x8E);
    SetupIDTGate(31, (DWORD)isr31, 0x08, 0x8E);

    //Interrupt Request Lines
    SetupIDTGate(32, (DWORD)irq0, 0x08, 0x8E);
    SetupIDTGate(33, (DWORD)irq1, 0x08, 0x8E);
    SetupIDTGate(34, (DWORD)irq2, 0x08, 0x8E);
    SetupIDTGate(35, (DWORD)irq3, 0x08, 0x8E);
    SetupIDTGate(36, (DWORD)irq4, 0x08, 0x8E);
    SetupIDTGate(37, (DWORD)irq5, 0x08, 0x8E);
    SetupIDTGate(38, (DWORD)irq6, 0x08, 0x8E);
    SetupIDTGate(39, (DWORD)irq7, 0x08, 0x8E);
    SetupIDTGate(40, (DWORD)irq8, 0x08, 0x8E);
    SetupIDTGate(41, (DWORD)irq9, 0x08, 0x8E);
    SetupIDTGate(42, (DWORD)irq10, 0x08, 0x8E);
    SetupIDTGate(43, (DWORD)irq11, 0x08, 0x8E);
    SetupIDTGate(44, (DWORD)irq12, 0x08, 0x8E);
    SetupIDTGate(45, (DWORD)irq13, 0x08, 0x8E);
    SetupIDTGate(46, (DWORD)irq14, 0x08, 0x8E);
    SetupIDTGate(47, (DWORD)irq15, 0x08, 0x8E);

    //Syscall
    SetupIDTGate(128, (DWORD)isr128, 0x08, 0x8E);
    SetupIDTGate(177, (DWORD)isr177, 0x08, 0x8E);

    LoadIDT((DWORD)&idtr);
}

const char* exception_messages[] = 
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Fault",
    "Machine Check", 
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void ISRHandler(struct InterruptRegisters* regs)
{
    if (regs->int_no < 32)
    {
        DeathScreen(exception_messages[regs->int_no]);

        Print("\nEAX: ", 0x0F);
        PrintHex(regs->eax, 0x0F);

        Print("\nEBX: ", 0x0F);
        PrintHex(regs->ebx, 0x0F);

        Print("\nECX: ", 0x0F);
        PrintHex(regs->ecx, 0x0F);

        Print("\nEDX: ", 0x0F);
        PrintHex(regs->edx, 0x0F);

        Print("\nESP: ", 0x0F);
        PrintHex(regs->esp, 0x0F);

        Print("\nEBP: ", 0x0F);
        PrintHex(regs->ebp, 0x0F);

        Print("\nESI: ", 0x0F);
        PrintHex(regs->esi, 0x0F);

        Print("\nEDI: ", 0x0F);
        PrintHex(regs->edi, 0x0F);

        for (;;);
    }
}

void* IRQRoutines[16] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void IRQInstallHandler(int irq, void (*handler)(struct InterruptRegisters *r))
{
    IRQRoutines[irq] = handler;
}

void IRQUninstallHandler(int irq)
{
    IRQRoutines[irq] = 0;
}

void IRQHandler(struct InterruptRegisters* regs)
{
    void (*handler)(struct InterruptRegisters *regs);

    handler = IRQRoutines[regs->int_no - 32];

    if (handler)
    {
        handler(regs);
    }

    if (regs->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    outb(0x20,0x20);
}
