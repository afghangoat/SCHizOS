#include "memdetect.h"

MemoryRegion global_memory_regions[MAX_REGIONS];
int global_memory_reg_count;

void memory_detect(MemoryInfo* memory_info){
	E820MemoryBlock block;
	uint32_t continuation=0;
	int ret;
	global_memory_reg_count=0;
	ret=x86_E820GetNextBlock(&block,&continuation);
	
	while(ret>0&&continuation !=0){
		
		global_memory_regions[global_memory_reg_count].Begin=block.Base;
		global_memory_regions[global_memory_reg_count].Length=block.Length;
		global_memory_regions[global_memory_reg_count].Type=block.Type;
		global_memory_regions[global_memory_reg_count].ACPI=block.ACPI;
		
		global_memory_reg_count++;
		printf("E820: base=0x%llx len=0x%llx type=0x%x\n",block.Base,block.Length,block.Type);
		
		ret=x86_E820GetNextBlock(&block,&continuation);
	} 
	
	memory_info->RegionCount=global_memory_reg_count;
	memory_info->Regions=global_memory_regions;
}
