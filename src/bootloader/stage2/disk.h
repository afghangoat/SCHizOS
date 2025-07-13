#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "x86.h"

#define TRY_COUNT 3

typedef struct{
	uint8_t id;
	uint16_t cylinders;
	uint16_t sectors;
	uint16_t heads;
} DISK;

bool disk_initialize(DISK* disk, uint8_t drivenum);
bool disk_read_sectors(DISK* disk, uint32_t lba,uint8_t sectors,void* data_out);
