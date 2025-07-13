#pragma once
#include <stdint.h>

typedef struct {
	//In order, up to down
	uint32_t ds;	//data segment
	uint32_t edi,esi,ebp,kern_esp,ebx,edx,ecx,eax; //pusha pushed data segments
	uint32_t interrupt,error;
	uint32_t eip,cs,eflags,esp,ss; //pushed automatically, reversed order.
} __attribute__((packed)) Registers;

/*struct interrupt_frame //Example for frame from wiki.osdev.org
{
    uword_t ip;
    uword_t cs;
    uword_t flags;
    uword_t sp;
    uword_t ss;
};*/

typedef void (*ISRHandler)(Registers* regs);


void i686_ISR_Init();
void i686_ISR_RegisterHandler(int interrupt,ISRHandler handler);
