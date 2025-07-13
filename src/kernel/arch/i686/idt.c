#include "idt.h"
#include <utils/binary.h>

typedef struct{
	uint16_t BaseLow; //Lower bits of base
	uint16_t SegmentSelector;
	uint8_t Reserved;
	uint8_t Flags;
	uint16_t BaseHigh; //Upper bits of base
	
} __attribute__((packed)) IDTEntry;

typedef struct{
	uint16_t Limit;
	IDTEntry* Ptr;
	
} __attribute__((packed)) IDTDescriptor;

IDTEntry global_idt[256];
IDTDescriptor global_idt_desc={sizeof(global_idt)-1,global_idt};

void __attribute__((cdecl)) i686_IDT_Load(IDTDescriptor* idt_descriptor);

void i686_IDT_SetGate(int interrupt,void* base,uint16_t seg_descriptor,uint8_t flags){
	global_idt[interrupt].BaseLow=((uint32_t)base)&0xFFFF;
	global_idt[interrupt].SegmentSelector=seg_descriptor;
	global_idt[interrupt].Reserved=0;
	global_idt[interrupt].Flags=flags;
	global_idt[interrupt].BaseHigh=((uint32_t)base>>16)&0xFFFF;
}

void i686_IDT_EnableGate(int interrupt){
	FLAG_SET(global_idt[interrupt].Flags,IDT_FLAG_PRESENT);
}
void i686_IDT_DisableGate(int interrupt){
	FLAG_UNSET(global_idt[interrupt].Flags,IDT_FLAG_PRESENT);
}

void i686_IDT_Init(){
	i686_IDT_Load(&global_idt_desc);
}
