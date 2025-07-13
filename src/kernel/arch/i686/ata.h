#pragma once
#include <stdint.h>

#define ASM_FUNCTION __attribute__((cdecl))
void ASM_FUNCTION ata_chs_read(uint32_t chs_values,uint32_t num_of_sectors,uint32_t* address_buffer);

void ASM_FUNCTION ata_lba_read(uint32_t block_of_sectors,uint32_t num_of_sectors,uint32_t* address_buffer);

void ASM_FUNCTION ata_lba_write(uint32_t block_of_sectors,uint32_t num_of_sectors,uint32_t* address_buffer);

void ASM_FUNCTION ata_disk_reset(uint8_t drive_num);
