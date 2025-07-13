#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    uint8_t BootJumpInstruction[3];
    uint8_t OemIdentifier[8];
    uint16_t BytesPerSector;
    uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;
    uint8_t FatCount;
    uint16_t DirEntryCount;
    uint16_t TotalSectors;
    uint8_t MediaDescriptorType;
    uint16_t SectorsPerFat;
    uint16_t SectorsPerTrack;
    uint16_t Heads;
    uint32_t HiddenSectors;
    uint32_t LargeSectorCount;
    uint8_t DriveNumber;
    uint8_t _Reserved;
    uint8_t Signature;
    uint32_t VolumeId;
    uint8_t VolumeLabel[11];
    uint8_t SystemId[8];


} __attribute__((packed)) BootSector;
typedef struct{
	uint8_t Name[11];
	uint8_t Attributes;
	uint8_t _Reserved;
	uint8_t CreatedTimeTenths;
	uint16_t CreatedTime;
	uint16_t CreatedDate;
	uint16_t AccessedDate;
	uint16_t FirstClusterHigh;
	uint16_t ModifiedTime;
	uint16_t ModifiedDate;
	uint16_t FirstClusterLow;
	uint32_t Size;
} __attribute__((packed)) DirEntry;

BootSector global_boot_sector;
uint8_t* global_fat=NULL;
DirEntry* root_dir=NULL;
uint32_t root_dir_end;

bool read_boot_sector(FILE* disk){
	if(fread(&global_boot_sector,sizeof(global_boot_sector),1,disk)>0){
		return true;
	}
	return false;
}

bool read_sectors(FILE* disk, uint32_t lba,uint32_t count, void* bufferOut){
	bool ok=true;
	ok=ok&&(fseek(disk,lba*global_boot_sector.BytesPerSector,SEEK_SET)==0);
	ok=ok&&(fread(bufferOut,global_boot_sector.BytesPerSector,count,disk)==count);
	return ok;
}
bool read_fat(FILE* disk){
	global_fat= (uint8_t*) malloc(global_boot_sector.SectorsPerFat*global_boot_sector.BytesPerSector);
	return read_sectors(disk, global_boot_sector.ReservedSectors,global_boot_sector.SectorsPerFat,global_fat);
}
bool read_root_dir(FILE* disk){
	uint32_t lba=global_boot_sector.ReservedSectors+global_boot_sector.SectorsPerFat*global_boot_sector.FatCount;
	uint32_t size=sizeof(DirEntry)*global_boot_sector.DirEntryCount;
	uint32_t sectors = (size/global_boot_sector.BytesPerSector);
	if(size%global_boot_sector.BytesPerSector>0){
		sectors++;
	}
	root_dir_end=lba+sectors;
	root_dir= (DirEntry*) malloc(sectors*global_boot_sector.BytesPerSector);
	//CHECK NOT NULL
	return read_sectors(disk, lba,sectors,root_dir);
}
DirEntry* scan_file(const char* name){
	for(uint32_t i=0;i<global_boot_sector.DirEntryCount;i++){
		if(memcmp(name,root_dir[i].Name,11)==0){
			return &root_dir[i];
		}
	}
	return NULL;
}
bool read_file(DirEntry* file, FILE* disk, uint8_t* outBuffer){
	
	bool ok=true;
	uint16_t cur_cluster=file->FirstClusterLow;
	do{
		uint32_t lba=root_dir_end+(cur_cluster-2)*global_boot_sector.SectorsPerCluster;
		ok=ok&&read_sectors(disk,lba,global_boot_sector.SectorsPerCluster,outBuffer);
		outBuffer+=global_boot_sector.SectorsPerCluster*global_boot_sector.BytesPerSector;
		
		uint32_t fat_index=cur_cluster*3/2;
		if(cur_cluster%2==0){
			cur_cluster=(*(uint16_t*)(global_fat+fat_index))&0x0FFF;
		} else {
			cur_cluster=(*(uint16_t*)(global_fat+fat_index))>>4;
		}
	} while(ok&&cur_cluster<0x0FF8);
	return ok;
}

int main(int argc, char** argv){
	if (argc<3){
		printf("Usage: %s <disk image> <filename>\n",argv[0]);
		return -1;
	}
	FILE* disk = fopen(argv[1],"rb");
	if(disk==NULL){
		fprintf(stderr,"Cannot open disk image %s!",argv[1]);
		fclose(disk);
		return -1;
	}
	if(read_boot_sector(disk)==false){
		fprintf(stderr,"Cannot read boot sector!");
		fclose(disk);
		return -2;
	}
	if(read_fat(disk)==false){
		fprintf(stderr,"Could not read FAT!");
		free(global_fat);
		fclose(disk);
		return -3;
	}
	if(read_root_dir(disk)==false){
		fprintf(stderr,"Could not create or read in the root directory!");
		free(global_fat);
		free(root_dir);
		fclose(disk);
		return -4;
	}
	DirEntry* example_file=scan_file(argv[2]);
	if(example_file==NULL){
		fprintf(stderr,"Could not scan or find file %s!",argv[2]);
		free(global_fat);
		free(root_dir);
		fclose(disk);
		return -5;
	}
	uint8_t* buffer=(uint8_t*) malloc(example_file->Size+global_boot_sector.BytesPerSector);
	if(read_file(example_file,disk,buffer)==false){
		fprintf(stderr,"Could not read file %s!",argv[2]);
		free(global_fat);
		free(root_dir);
		free(buffer);
		fclose(disk);
		return -6;
	}
	for(size_t i=0;i<example_file->Size;i++){
		if(isprint(buffer[i])){
			fputc(buffer[i],stdout);
		} else {
			printf("<%02x>",buffer[i]);
		}
	}
	printf("\n");
	
	free(global_fat);
	free(root_dir);
	free(buffer);
	fclose(disk);
	return 0;
}