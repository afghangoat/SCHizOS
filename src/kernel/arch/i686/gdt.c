#include "gdt.h"

//Global descriptor table implementation
typedef struct{
	uint16_t LimitLow; 	//The limit (bits 0-16) which either stores the limit in 1 bytes or 4kb
	uint16_t BaseLow; 	//Base 0-15 bits
	uint8_t BaseMiddle;	//Base 16-23 bits
	uint8_t Access;
	uint8_t FlagsLimitHi;	//Limit bits 16-19 and the flags stored in upper bits
	uint8_t BaseHigh; 		//Base bits 24-31, end of base
	
	
	
} __attribute__((packed)) GDTEntry;

typedef struct{
	uint16_t Limit; //sizeof(gdt)-1
	GDTEntry* Ptr;
} __attribute__((packed)) GDTDescriptor;

typedef enum{
	
	//Bit 0 -access bit
	GDT_ACCESSED=0x01, //Set automatically by CPU
	
	//Bit 1 -readable attrib
	GDT_ACCESS_CODE_READABLE=0x02,
	GDT_ACCESS_DATA_WRITEABLE=0x02,
	
	//Bit 2 -segmentation
	GDT_ACCESS_CODE_CONFORMING=0x04,
	GDT_ACCESS_DATA_DIRECTION_NORMAL=0x00,
	GDT_ACCESS_DATA_DIRECTION_DOWN=0x04, //Is it expanding in reverse direction?
	
	//Bit 3 -executable bit
	
	//Bit 4 -is it code or data
	GDT_ACCESS_DATA_SEGMENT=0x10,
	GDT_ACCESS_CODE_SEGMENT=0x18,
	GDT_ACCESS_DESCRIPTOR_TSS=0x00,
	
	//Bits 5-6 privilege level
	GDT_ACCESS_RING0=0x00,
	GDT_ACCESS_RING1=0x20,
	GDT_ACCESS_RING2=0x40,
	GDT_ACCESS_RING3=0x60,
	
	//Bit 7 is bit present or not?
	GDT_ACCESS_PRESENT=0x80
} GDT_ACCESS;

typedef enum{
	GDT_FLAG_64BIT=0x20,
	GDT_FLAG_32BIT=0x40,
	GDT_FLAG_16BIT=0x00,
	GDT_FLAG_GRANULARITY_1B=0x00,
	GDT_FLAG_GRANULARITY_4K=0x80,
	//Bit 4 -ignored by CPU, only read by OS
} GDT_FLAGS;

//Helper macros
#define GDT_LIMIT_LOW(limit) (limit&0xFFFF)
#define GDT_BASE_LOW(base) (base&0xFFFF)
#define GDT_BASE_MIDDLE(base) ((base>>16)&0xFF) //Lower 8 bit
#define GDT_BASE_HIGH(base) ((base>>24)&0xFF)
#define GDT_FLAGS_LIMIT_HI(limit,flags) (((limit>>16)&0xF) | (flags&0xF0))

#define GDT_ENTRY(base,limit,access,flags){ \
	GDT_LIMIT_LOW(limit), \
	GDT_BASE_LOW(base), \
	GDT_BASE_MIDDLE(base), \
	access, \
	GDT_FLAGS_LIMIT_HI(limit,flags), \
	GDT_BASE_HIGH(base) \
}

//Global descriptor table
GDTEntry global_GDT[]={
	GDT_ENTRY(0,0,0,0), //Null descriptor
	
	GDT_ENTRY(
	0,
	0xFFFFF,
	GDT_ACCESS_PRESENT|GDT_ACCESS_RING0|GDT_ACCESS_CODE_SEGMENT|GDT_ACCESS_CODE_READABLE,
	GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K
	), //32 Bit code segment (Kernel)
	
	GDT_ENTRY(
	0,
	0xFFFFF,
	GDT_ACCESS_PRESENT|GDT_ACCESS_RING0|GDT_ACCESS_DATA_SEGMENT|GDT_ACCESS_DATA_WRITEABLE,
	GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K
	), //32 Bit data segment (Kernel)
};

GDTDescriptor global_descriptor={sizeof(global_GDT)-1,global_GDT};

void __attribute__((cdecl)) i686_GDT_Load(GDTDescriptor* descriptor,uint16_t code_seg,uint16_t data_seg);

void i686_GDT_Initialize(){
	i686_GDT_Load(&global_descriptor,i686_GDT_CODE_SEGMENT,i686_GDT_DATA_SEGMENT);
}
