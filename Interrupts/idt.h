#ifndef IDT_H
#define IDT_H

#define TOTALGATES 256
#define CODESEG 0x08

struct IDT_GateDescriptor
{
    WORD offset;   //offset (0 - 15)
    WORD selector; //code segment selector (16 - 31)
    BYTE reserved; //reserved (32 - 39)
    BYTE flags;    //gatetype, dpl, p, 0 (40 - 47)
    WORD offset2;  //offset (48 - 63)
} __attribute__((packed));

struct IDT_Descriptor
{
    WORD  size;   //IDT size in bytes
    DWORD offset; //linear addr to IDT
} __attribute__((packed));

struct InterruptRegisters
{
    DWORD cr2;
    DWORD ds;
    DWORD edi, esi, ebp, esp, ebx, edx, ecx, eax;
    DWORD int_no, err_code;
    DWORD eip, csm, eflags, useresp, ss;
};

typedef void (*IRQHandler_t)(struct InterruptRegisters*);

void SetupIDTGate(BYTE entry, DWORD isr, WORD selector, BYTE flags);
void SetupIDT(void);

void ISRHandler(struct InterruptRegisters* regs);

void IRQInstallHandler(int irq, IRQHandler_t handler);
void IRQUninstallHandler(int irq, IRQHandler_t handler);
void IRQHandler(struct InterruptRegisters* regs);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr128();
extern void isr177();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
