#include "hal.h"
#include <arch/i686/gdt.h>
#include <arch/i686/idt.h>
#include <arch/i686/isr.h>
#include <arch/i686/irq.h>

void HAL_init(){
	i686_GDT_Initialize();
	i686_IDT_Init(); //Initialize Interrupt Descriptor Table
	i686_ISR_Init();
	i686_IRQ_Init();
}