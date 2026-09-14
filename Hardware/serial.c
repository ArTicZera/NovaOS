// coded by nicolasbickhoff11

#include "serial.h"
#include "../Include/stdint.h"
#include "../Include/ports.h"

void InitSerial() {
    outb(PORT + 1, 0x00);    // Disable all interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB
    outb(PORT + 0, 0x03);    // Divisor lo byte (38400 baud)
    outb(PORT + 1, 0x00);    // Divisor hi byte
    outb(PORT + 3, 0x03);    // 8 bits, no parity, 1 stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO, clear
    outb(PORT + 4, 0x0B);    // Enable IRQs, RTS/DSR
}

void com1PutChar(char c) {
    while ((inb(PORT + 5) & 0x20) == 0);
    outb(PORT, c);
}

