#pragma once
#include <stdint.h>
#include <stdbool.h>

void __attribute__((cdecl)) x86_outb(uint16_t port, uint8_t value);
uint8_t __attribute__((cdecl)) x86_inb(uint16_t port);
uint8_t __attribute__((cdecl)) i686_enable_interrupts();
uint8_t __attribute__((cdecl)) i686_disable_interrupts();

void i686_io_wait();
void __attribute__((cdecl)) i686_panic();
void __attribute__((cdecl)) flush_irq();
void __attribute__((cdecl)) start_pic();
