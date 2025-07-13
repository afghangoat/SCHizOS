#include "pic.h"
#include "io.h"

#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1

enum {
	//0 IC4 - if set, pic expects to recieve ICW4 during initialization
	PIC_ICW1_ICW4=0x01,
	//1 SGNL - if set, only 1 pic is in the system, if not there will be more slave PICs
	PIC_ICW1_SINGLE=0x02,
	
	//2 ADI - call addr interval. Ignored on x86, set to 0
	PIC_ICW1_INTERVAL4=0x04,
	PIC_ICW1_LEVEL=0x08,
	//3 LTIM - set set, operate in level triggered mode, if unsed, operate in edge triggered mode
	//4 INIT - if set to 1, initialize PIC
	PIC_ICW1_INITIALIZE=0x10,
	//5-7 - Ignored on x86, default value:0
} PIC_ICW1;

//Init Control Word 4
enum {
	//0 uPM - 80/86 mode toggle
	PIC_ICW4_8086=0x01,
	//1 AEOI - if set, on last interrupt acknowledge pulse, controller automatically performs an end of interrupt operation
	PIC_ICW4_AUTO_EOI=0x02,
	
	//2 M/S - selects buffer master. Else select buffer slave
	PIC_ICW4_BUFFER_MASTER=0x04,
	PIC_ICW4_BUFFER_SLAVE=0x0,
	
	//3 BUF - set set, operates in buffered mode
	PIC_ICW4_BUFFERED=0x8,
	//4 SFNM - specially fully nested mode; used in systems with large number of cascaded controllers
	PIC_ICW4_SFNM=0x10,
	//5-7 - Reserved, set to 1
} PIC_ICW4;

enum {
	PIC_CMD_END_OF_INTERRUPT = 0x20,
	PIC_CMD_READ_IRR=0x0A,
	PIC_CMD_READ_ISR=0x0B,
} PIC_CMD;

//Interrupt Control Word setups
void i686_PIC_Conf(uint8_t offset_pic1,uint8_t offset_pic2){
	//Init control word 1
	x86_outb(PIC1_COMMAND_PORT,PIC_ICW1_ICW4|PIC_ICW1_INITIALIZE);
	i686_io_wait();
	x86_outb(PIC2_COMMAND_PORT,PIC_ICW1_ICW4|PIC_ICW1_INITIALIZE);
	i686_io_wait();
	
	//Init control word 2 - offsets
	x86_outb(PIC1_DATA_PORT,offset_pic1);
	i686_io_wait();
	x86_outb(PIC2_DATA_PORT,offset_pic2);
	i686_io_wait();
	
	//Init control word 3
	x86_outb(PIC1_DATA_PORT,0x4); 	//slave at IRQ2
	i686_io_wait();
	x86_outb(PIC2_DATA_PORT,0x2);	//cascade identity
	i686_io_wait();
	
	//Init control word 4
	x86_outb(PIC1_DATA_PORT,PIC_ICW4_8086);
	i686_io_wait();
	x86_outb(PIC2_DATA_PORT,PIC_ICW4_8086);
	i686_io_wait();
	
	//clear data regs
	x86_outb(PIC1_DATA_PORT,0);
	i686_io_wait();
	x86_outb(PIC2_DATA_PORT,0);
	i686_io_wait();
}

void i686_PIC_mask(int irq){
	uint8_t port;
	if(irq<8){//Check interrupt request number
		uint8_t mask=x86_inb(PIC1_DATA_PORT);
		x86_outb(PIC1_DATA_PORT,mask|(1<<irq));
	}else{ 
		irq-=8;
		uint8_t mask=x86_inb(PIC2_DATA_PORT);
		x86_outb(PIC2_DATA_PORT,mask|(1<<irq));
	}
}

void i686_PIC_un_mask(int irq){
	uint8_t port;
	if(irq<8){//Check interrupt request number
		uint8_t mask=x86_inb(PIC1_DATA_PORT);
		x86_outb(PIC1_DATA_PORT,mask&~(1<<irq));
	}else{ 
		irq-=8;
		uint8_t mask=x86_inb(PIC2_DATA_PORT);
		x86_outb(PIC2_DATA_PORT,mask&~(1<<irq));
	}
}

void i686_PIC_disable(){
	//Disable legacy PIC, mask all
	x86_outb(PIC1_DATA_PORT,0xFF);
	i686_io_wait();
	x86_outb(PIC2_DATA_PORT,0xFF);
	i686_io_wait();
}

void i686_PIC_send_eoi(int irq){ //Send end of interrupt
	if(irq>=8){
		x86_outb(PIC2_COMMAND_PORT,PIC_CMD_END_OF_INTERRUPT);
	}
	x86_outb(PIC1_COMMAND_PORT,PIC_CMD_END_OF_INTERRUPT);
}

uint16_t i686_PIC_read_irq(){
	x86_outb(PIC1_COMMAND_PORT,PIC_CMD_READ_IRR);
	x86_outb(PIC2_COMMAND_PORT,PIC_CMD_READ_IRR);
	return ((uint16_t)x86_inb(PIC2_COMMAND_PORT)) | (((uint16_t)x86_inb(PIC2_COMMAND_PORT)) << 8);
}

uint16_t i686_PIC_read_in_service(){
	x86_outb(PIC1_COMMAND_PORT,PIC_CMD_READ_ISR);
	x86_outb(PIC2_COMMAND_PORT,PIC_CMD_READ_ISR);
	return ((uint16_t)x86_inb(PIC2_COMMAND_PORT)) | (((uint16_t)x86_inb(PIC2_COMMAND_PORT)) << 8);
}

