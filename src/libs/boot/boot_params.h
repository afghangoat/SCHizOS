#pragma once

#include <stdint.h>
#include <fs/vfs.h>

typedef struct{
	uint64_t Begin;
	uint64_t Length;
	uint32_t Type;
	uint32_t ACPI;
} MemoryRegion;

typedef struct{
	int RegionCount;
	MemoryRegion* Regions;
} MemoryInfo;

typedef struct {
	MemoryInfo Memory;
	uint8_t BootDevice;
	
	void* initrd_ptr;
    uint32_t initrd_size;
	
	MemoryFileEntry FileEntries[MAX_FILE_ENTRIES_IN_LIVE_BOOT];
	uint32_t file_count;
	
} BootParams;