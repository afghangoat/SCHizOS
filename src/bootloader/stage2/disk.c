#include "disk.h"
#include "stdio.h"
#include "x86.h"
#include <stdlang.h>

bool disk_initialize(DISK* disk, uint8_t drivenum){
	uint8_t drive_type;
	uint16_t cylinders,sectors,heads;
	
	if(x86_Disk_GetDriveParams(disk->id,&drive_type,&cylinders,&sectors,&heads)==false){
		printf(DISK_ERR MSG_DELIM ERR_DRIVE_PARAMS);
		return false;
	}
	disk->id=drivenum;
	disk->cylinders=cylinders;
	disk->heads=heads;
	disk->sectors=sectors;
	return true;
}
//counting heads,sectors from lowest to highest
void disk_lba2chs(DISK* disk,uint32_t lba,uint16_t* cyl_out,uint16_t* sector_out,uint16_t* head_out){
	
	//sector=LBA%sectors per track+1
	*sector_out=lba% disk->sectors+1;
	
	//cyl=(LBL/sector per track) / heads
	*cyl_out=(lba/disk->sectors)/disk->heads;
	
	//cyl=(LBL/sector per track) % heads
	*head_out=(lba/disk->sectors)%disk->heads;
}
bool disk_read_sectors(DISK* disk, uint32_t lba,uint8_t sectors,void* data_out){
	
	uint16_t cylinder, sector, head;

    disk_lba2chs(disk, lba, &cylinder, &sector, &head);

    for (int i = 0; i < TRY_COUNT; i++){
        if (x86_Disk_Read(disk->id, cylinder, sector, head, sectors, data_out)){
            return true;
		}
        x86_Disk_Reset(disk->id);
    }
	printf(DISK_ERR MSG_DELIM ERR_READ_SECTORS);
    return false;
}

