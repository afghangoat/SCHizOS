#pragma once
#include <stdint.h>

typedef enum {
	//Trap gates: assuming the task needs to be reran
	IDT_FLAG_GATE_TASK=0x5,
	IDT_FLAG_GATE_16BIT_INT=0x6,
	IDT_FLAG_GATE_16BIT_TRAP=0x7,
	IDT_FLAG_GATE_32BIT_INT=0xE,
	IDT_FLAG_GATE_32BIT_TRAP=0xF,
	//Bit 4 is zero
	
	//Bit 5
	IDT_FLAG_RING0=(0<<5),
	IDT_FLAG_RING1=(1<<5),
	IDT_FLAG_RING2=(2<<5),
	IDT_FLAG_RING3=(3<<5),
	
	//Bit present
	IDT_FLAG_PRESENT=0x80
	
} IDT_FLAGS;

void i686_IDT_Init();
void i686_IDT_EnableGate(int interrupt);
void i686_IDT_DisableGate(int interrupt);
void i686_IDT_SetGate(int interrupt,void* base,uint16_t seg_descriptor,uint8_t flags);
