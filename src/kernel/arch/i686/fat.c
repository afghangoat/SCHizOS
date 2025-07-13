#include "fat.h"
#include <stdio.h>
#include <memdefs.h>
//#include <ctype.h>
//#include "../../../libs/math.h"
//#include "../../../libs/string.h"
#include "../../memory.h" //WHY
#include <stdlang.h>

const char* strchr(const char* str,char chr){
	if(str==NULL){
		return NULL;
	}
	while(*str!='\0'){
		if(*str==chr){
			return str;
		}
		str++;
	}
	return NULL;
}

char* strcpy(char* dst,char* src){
	char* original_dst=dst;
	if(dst==NULL){
		return NULL;
	}
	if(src==NULL){
		*dst='\0';
		return dst;
	}
	while(*src!='\0'){
		*dst=*src;
		++dst;
		++src;
	}
	*dst='\0';
	return original_dst;
}

unsigned strlen(const char* str){
	unsigned len=0;
	while(*str!='\0'){
		len++;
		++str;
	}
	return len;
}

uint32_t min(uint32_t num1,uint32_t num2){
	if(num1<num2){
		return num1;
	}
	return num2;
}
bool islower(char chr){
    return chr >= 'a' && chr <= 'z';
}

char toupper(char chr){
	if(islower(chr)==true){
		return chr-32;
	} else {
		return chr;
	}
}


void* memcpy(void* dst, const void* src,uint16_t num){ //TEMP
	uint8_t* u8Dst=(uint8_t*)dst;
	const uint8_t* u8Src=(const uint8_t*)src;
	
	for(uint16_t i=0;i<num;i++){
		u8Dst[i]=u8Src[i];
	}
	return dst;
}
int memcmp(const void* ptr1,const void* ptr2,uint16_t num){
	const uint8_t* u8ptr1=(const uint8_t*)ptr1;
	const uint8_t* u8ptr2=(const uint8_t*)ptr2;
	
	for(uint16_t i=0;i<num;i++){
		if(u8ptr1[i]!=u8ptr2[i]){
			return 1;
		}
	}
	return 0;
}


typedef struct {
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
    uint32_t VolumeId;          // serial number, value doesn't matter
    uint8_t VolumeLabel[FAT_NAME_LENGTH];    // 11 bytes, padded with spaces
    uint8_t SystemId[8];

} __attribute__((packed)) FAT_BootSector;



typedef struct{
    uint8_t Buffer[SECTOR_SIZE];
    FAT_File Public;
    bool Opened;
    uint32_t FirstCluster;
    uint32_t CurrentCluster;
    uint32_t CurrentSectorInCluster;

} FAT_FileData;

typedef struct{
    union{
        FAT_BootSector BootSector;
        uint8_t BootSectorBytes[SECTOR_SIZE];
    } BS;
    FAT_FileData RootDirectory;
    FAT_FileData OpenedFiles[MAX_FILE_HANDLES];
} FAT_Data;

static FAT_Data* global_data;
static uint8_t* global_fat = NULL;
static uint32_t global_data_sec_lba;


bool FAT_ReadBootSector(DISK* disk){
    return disk_read_sectors(disk, 0, 1, global_data->BS.BootSectorBytes);
}

bool FAT_ReadFat(DISK* disk){
    return disk_read_sectors(disk, global_data->BS.BootSector.ReservedSectors, global_data->BS.BootSector.SectorsPerFat, global_fat);
}

bool FAT_Initialize(DISK* disk){
    global_data = (FAT_Data*)MEMORY_FAT_ADDR;
	//printf(FAT_ERR"aAAA");
    // read boot sector
    if (FAT_ReadBootSector(disk)==false){
        printf(FAT_ERR MSG_DELIM ERR_READ_BOOT);
        return false;
    }

    // read FAT
    global_fat = (uint8_t*)global_data + sizeof(FAT_Data);
    uint32_t fatSize = global_data->BS.BootSector.BytesPerSector * global_data->BS.BootSector.SectorsPerFat;
    if (sizeof(FAT_Data) + fatSize >= MEMORY_FAT_MAX){
        printf(FAT_ERR MSG_DELIM ERR_FAT_MEM_OUT, sizeof(FAT_Data) + fatSize, MEMORY_FAT_MAX);
        return false;
    }

    if (FAT_ReadFat(disk)==false){
        printf(FAT_ERR MSG_DELIM ERR_FAT_READ_FAIL);
        return false;
    }

    // open and initialize root directory file
    uint32_t rootDirLba = global_data->BS.BootSector.ReservedSectors + global_data->BS.BootSector.SectorsPerFat * global_data->BS.BootSector.FatCount;
    uint32_t rootDirSize = sizeof(FAT_DirectoryEntry) * global_data->BS.BootSector.DirEntryCount;

    global_data->RootDirectory.Public.Handle = ROOT_DIRECTORY_HANDLE;
    global_data->RootDirectory.Public.IsDirectory = true;
    global_data->RootDirectory.Public.Position = 0;
    global_data->RootDirectory.Public.Size = sizeof(FAT_DirectoryEntry) * global_data->BS.BootSector.DirEntryCount;
    global_data->RootDirectory.Opened = true;
    global_data->RootDirectory.FirstCluster = rootDirLba;
    global_data->RootDirectory.CurrentCluster = rootDirLba;
    global_data->RootDirectory.CurrentSectorInCluster = 0;

	//is lba invalid
    if (disk_read_sectors(disk, rootDirLba, 1, global_data->RootDirectory.Buffer)==false){
        printf(FAT_ERR MSG_DELIM ERR_ROOT_DIR_READ);
        return false;
    }

    // calculate data section
    uint32_t rootDirSectors = (rootDirSize + global_data->BS.BootSector.BytesPerSector - 1) / global_data->BS.BootSector.BytesPerSector;
    global_data_sec_lba = rootDirLba + rootDirSectors;

    // reset opened files
    for (int i = 0; i < MAX_FILE_HANDLES; i++){
        global_data->OpenedFiles[i].Opened = false;
	}

    return true;
}

uint32_t FAT_ClusterToLba(uint32_t cluster){
    return global_data_sec_lba + (cluster - 2) * global_data->BS.BootSector.SectorsPerCluster;
}

FAT_File* FAT_OpenEntry(DISK* disk, FAT_DirectoryEntry* entry)
{
    //find available handle
    int handle = -1;
    for (int i = 0; i < MAX_FILE_HANDLES && handle < 0; i++){
        if (global_data->OpenedFiles[i].Opened==false){
            handle = i;
		}
    }

    // out of handles
    if (handle < 0){
        printf(FAT_ERR MSG_DELIM ERR_NO_AVAIL_HANDLE);
        return false;
    }

    // setup variable and default values
    FAT_FileData* file_data = &global_data->OpenedFiles[handle];
    file_data->Public.Handle = handle;
    file_data->Public.IsDirectory = (entry->Attributes & FAT_ATTRIBUTE_DIRECTORY) != 0;
    file_data->Public.Position = 0;
    file_data->Public.Size = entry->Size;
    file_data->FirstCluster = entry->FirstClusterLow + ((uint32_t)entry->FirstClusterHigh << 16);
    file_data->CurrentCluster = file_data->FirstCluster;
    file_data->CurrentSectorInCluster = 0;

    //if (disk_read_sectors(disk, FAT_ClusterToLba(file_data->CurrentCluster), 1, file_data->Buffer)==false){
    //    printf(FAT_ERR MSG_DELIM ERR_INIT_ERROR);
    //    return false;
    //}
	if (!disk_read_sectors(disk, FAT_ClusterToLba(file_data->CurrentCluster), 1, file_data->Buffer)){
        printf(FAT_ERR MSG_DELIM ERR_INIT_ERROR"\nOpen entry failed - read error cluster=%u lba=%u\n", file_data->CurrentCluster, FAT_ClusterToLba(file_data->CurrentCluster));
        for (int i = 0; i < FAT_NAME_LENGTH; i++){
            printf("%c", entry->Name[i]);
		}
        printf("\n");
        return false;
    }

    file_data->Opened = true;
    return &file_data->Public;
}

uint32_t FAT_NextCluster(uint32_t currentCluster){    
    uint32_t fatIndex = currentCluster * 3 / 2;

    if (currentCluster % 2 == 0){
        return (*(uint16_t*)(global_fat + fatIndex)) & 0x0FFF;
    }else{
        return (*(uint16_t*)(global_fat + fatIndex)) >> 4;
	}
}

uint32_t FAT_Read(DISK* disk, FAT_File* file, uint32_t byteCount, void* dataOut)
{
    // get file data
    FAT_FileData* file_data;
	if(file->Handle == ROOT_DIRECTORY_HANDLE){
		file_data=&global_data->RootDirectory;
	} else {
		file_data=&global_data->OpenedFiles[file->Handle];
	}

    uint8_t* u8DataOut = (uint8_t*)dataOut;

    // don't read past the end of the file
    if (!file_data->Public.IsDirectory){
        byteCount = min(byteCount, file_data->Public.Size - file_data->Public.Position);
	}
	
    while (byteCount > 0){
        uint32_t leftInBuffer = SECTOR_SIZE - (file_data->Public.Position % SECTOR_SIZE);
        uint32_t take = min(byteCount, leftInBuffer);

        memcpy(u8DataOut, file_data->Buffer + file_data->Public.Position % SECTOR_SIZE, take);
        u8DataOut += take;
        file_data->Public.Position += take;
        byteCount -= take;

        // printf("leftInBuffer=%lu take=%lu\r\n", leftInBuffer, take);
        // See if we need to read more data
        if (leftInBuffer == take){
            // Special handling for root directory
            if (file_data->Public.Handle == ROOT_DIRECTORY_HANDLE){
                ++file_data->CurrentCluster;

                // read next sector
                if (disk_read_sectors(disk, file_data->CurrentCluster, 1, file_data->Buffer)==false){
                    printf(FAT_ERR MSG_DELIM ERR_READ);
                    break;
                }
            }else{
                // calculate next cluster & sector to read
                if (++file_data->CurrentSectorInCluster >= global_data->BS.BootSector.SectorsPerCluster){
                    file_data->CurrentSectorInCluster = 0;
                    file_data->CurrentCluster = FAT_NextCluster(file_data->CurrentCluster);
                }

                if (file_data->CurrentCluster >= 0xFF8){
                    // Mark end of file
                    file_data->Public.Size = file_data->Public.Position;
                    break;
                }

                // read next sector
                if (disk_read_sectors(disk, FAT_ClusterToLba(file_data->CurrentCluster) + file_data->CurrentSectorInCluster, 1, file_data->Buffer)==false){
                    printf(FAT_ERR MSG_DELIM ERR_READ);
                    break;
                }
            }
        }
    }
    return u8DataOut - (uint8_t*)dataOut;
}

bool FAT_ReadEntry(DISK* disk, FAT_File* file, FAT_DirectoryEntry* dirEntry){
    return FAT_Read(disk, file, sizeof(FAT_DirectoryEntry), dirEntry) == sizeof(FAT_DirectoryEntry);
}

void FAT_Close(FAT_File* file){
    if (file->Handle == ROOT_DIRECTORY_HANDLE){
        file->Position = 0;
        global_data->RootDirectory.CurrentCluster = global_data->RootDirectory.FirstCluster;
    }else{
        global_data->OpenedFiles[file->Handle].Opened = false;
    }
}

bool FAT_FindFile(DISK* disk, FAT_File* file, const char* name, FAT_DirectoryEntry* entryOut){
    char fatName[12];
    FAT_DirectoryEntry entry;

    // convert from name to fat name
    memset(fatName, ' ', sizeof(fatName));
    fatName[FAT_NAME_LENGTH] = '\0';

    const char* ext = strchr(name, '.');
    if (ext == NULL){
        ext = name + FAT_NAME_LENGTH;
	}

    for (int i = 0; i < 8 && name[i] && name + i < ext; i++){
        fatName[i] = toupper(name[i]);
	}

    if (ext != NULL){
        for (int i = 0; i < 3 && ext[i + 1]; i++){
            fatName[i + 8] = toupper(ext[i + 1]);
		}
    }

    while (FAT_ReadEntry(disk, file, &entry)){
        if (memcmp(fatName, entry.Name, FAT_NAME_LENGTH) == 0){
            *entryOut = entry;
            return true;
        }
    }
    
    return false;
}

FAT_File* FAT_Open(DISK* disk, const char* path){
    char name[MAX_PATH_SIZE];

    // ignore leading slash
    if (path[0] == '/'){
        path++;
	}

    FAT_File* current = &global_data->RootDirectory.Public;

    while (*path) {
        // extract next file name from path
        bool isLast = false;
        const char* delim = strchr(path, '/');
        if (delim != NULL){
            memcpy(name, path, delim - path);
            name[delim - path + 1] = '\0';
            path = delim + 1;
        }else{
            unsigned len = strlen(path);
            memcpy(name, path, len);
            name[len + 1] = '\0';
            path += len;
            isLast = true;
        }
        
        // find directory entry in current directory
        FAT_DirectoryEntry entry;
        if (FAT_FindFile(disk, current, name, &entry)){
            FAT_Close(current);
            // check if directory
            if (isLast==false && entry.Attributes & FAT_ATTRIBUTE_DIRECTORY == 0){
                printf(FAT_ERR MSG_DELIM ERR_NO_VALID_DIR, name);
                return NULL;
            }

            // open new directory entry
            current = FAT_OpenEntry(disk, &entry);
        }else{
            FAT_Close(current);
            printf(FAT_ERR MSG_DELIM ERR_INVALID, name);
            return NULL;
        }
    }
    return current;
}
