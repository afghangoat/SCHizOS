#include "irq.h"
#include "pic.h"
#include "io.h"
#include <memdefs.h>
#include <stdio.h>

#define PIC_REMAP_OFFSET 0x20
#define IRQ_NUMS 16

IRQ_handler global_IRQ_handlers[IRQ_NUMS];

void i686_IRQ_handler(Registers* regs){
	int irq=regs->interrupt-PIC_REMAP_OFFSET;
	
	/*if(global_IRQ_handlers[irq]!=NULL){
		//handle interrupt
		global_IRQ_handlers[irq](regs);
	}else{
		printf("Unhandled IRQ %d...\r\n",irq);
	}*/
	
	uint8_t pic_isr = i686_PIC_read_in_service();
    uint8_t pic_irr = i686_PIC_read_irq();

    if (global_IRQ_handlers[irq] != NULL)
    {
        // handle IRQ
        global_IRQ_handlers[irq](regs);
		flush_irq();
    }
    else
    {
        printf("Unhandled IRQ %d  ISR=%x  IRR=%x...\n", irq, pic_isr, pic_irr);
    }
	
	//send EOI
	i686_PIC_send_eoi(irq);
}
void i686_IRQ_reg_handlers(int irq,IRQ_handler handler){
	global_IRQ_handlers[irq]=handler;
}

void i686_IRQ_Init(){
	i686_PIC_Conf(PIC_REMAP_OFFSET,PIC_REMAP_OFFSET+8);
	
	//register ISR handlers for each of the 16 irq lines
	for(int i=0;i<IRQ_NUMS;i++){
		i686_ISR_RegisterHandler(PIC_REMAP_OFFSET+i,i686_IRQ_handler);
	}
	//enable interrupts
	i686_enable_interrupts();
	
}
