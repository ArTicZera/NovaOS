#ifndef SYSCALL_H
#define SYSCALL_H

#include "idt.h"

void SyscallHandler(struct InterruptRegisters* regs);

#endif