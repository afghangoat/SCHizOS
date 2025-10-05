//#include "stdint.h"
#include <stdint.h>
#include "stdio.h"
//#include "stdbool.h"
#include <stdbool.h>
#include <stdlang.h>
#include "disk.h"
#include "fat.h"
#include "memdetect.h"
#include <boot/boot_params.h>

#include "memdefs.h"
#include "memory.h"

uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

BootParams boot_params;

typedef void (*KernelStart)(BootParams* boot_params);
//TODO ctype,string,! to false

//CDECL calling conventions:
/*
Arguments:
-passed through stack
-pushed from right to left
-caller removes parameters from stack

Returns:
-EAX for ints,*-s
-ST0 for float

Registers:
-EAX,ECX,EDX saved by the caller, all others saved by callee, c function in asm begin with _
*/

//void far* g_data = (void far*)0x00500200;

MemoryFileEntry copyToFileK(const char* data) {
    MemoryFileEntry entry;
    
	for(int i=0;i<MAX_FILE_SIZE_IN_LIVE_BOOT;i++){
		entry.FileContents[i]=data[i];
		
		if(data[i]=='\0'){
			break;
		}
	}

    return entry;
}

void __attribute__((cdecl)) start(uint16_t bootDrive){
	boot_params.file_count=0;
	
    clrscr();
	printf(BOOT_MSG MSG_DELIM INFO_STAGE2_INI);
    DISK disk;
    if (disk_initialize(&disk, bootDrive)==false){
        printf(DISK_ERR MSG_DELIM ERR_INIT);
        goto end;
    }
	printf(BOOT_MSG MSG_DELIM INFO_FAT_INI);
    if (FAT_Initialize(&disk)==false){
        printf(FAT_ERR MSG_DELIM ERR_INIT);
        goto end;
    }
	
	//puts("Hello world from C!\r\n");
	//printf("AAAA %s aaa %d","bb",21);
	
    // read test.txt
    char buffer[100];
    uint32_t read2;
    FAT_File* file_data = FAT_Open(&disk, "userland/userinfo.txt");
    while ((read2 = FAT_Read(&disk, file_data, sizeof(buffer), buffer))){
        for (uint32_t i = 0; i < read2; i++){
            if (buffer[i] == '\n'){
                putc('\r');
			}
            putc(buffer[i]);
        }
    }
    FAT_Close(file_data);
	
	boot_params.FileEntries[boot_params.file_count]=copyToFileK(buffer);
	const char* filenamea="userinfotxt";
	for(int i=0;i<FAT_NAME_LENGTH;i++){
		boot_params.FileEntries[boot_params.file_count].FileName[i]=filenamea[i];
	}
	
	boot_params.file_count++;
	

	/*FAT_File* f1 = FAT_Open(&disk, "mydir/test.txt");
	uint32_t total = 0;
	uint32_t read2;
	uint8_t* target = (uint8_t*)MEMORY_FAT_ADDR;

	while ((read2 = FAT_Read(&disk, f1, MEMORY_LOAD_SIZE, KernelLoadBuffer))) {
		memcpy(target + total, KernelLoadBuffer, read2);
		total += read2;
	}
	FAT_Close(f1);

	boot_params.initrd_ptr  = (void*)MEMORY_FAT_ADDR;
	boot_params.initrd_size = total;*/
	
	printf(BOOT_MSG MSG_DELIM INFO_KERNEL_LOAD);
	
	//prepare boot params
	boot_params.BootDevice=bootDrive;
	memory_detect(&boot_params.Memory);
	//debug break;
	//for(;;);
	
    // load kernel
    FAT_File* fd = FAT_Open(&disk, "/kernel.bin");
    uint32_t read;
    uint8_t* kernelBuffer = Kernel;
    while ((read = FAT_Read(&disk, fd, MEMORY_LOAD_SIZE, KernelLoadBuffer))){
        memcpy(kernelBuffer, KernelLoadBuffer, read);
        kernelBuffer += read;
    }
    FAT_Close(fd);
	
	// browse files in root
    /*FAT_File* file_data = FAT_Open(&disk, "/");
	FAT_DirectoryEntry entry;

	uint8_t* target = (uint8_t*)MEMORY_FAT_ADDR;
	uint32_t total = 0;
	int i = 0;

	while (FAT_ReadEntry(&disk, file_data, &entry) && i++ < 64) {
		memcpy(target + total, &entry, sizeof(FAT_DirectoryEntry));
		total += sizeof(FAT_DirectoryEntry);
	}

	FAT_Close(file_data);

	boot_params.initrd_ptr  = (void*)MEMORY_FAT_ADDR;
	boot_params.initrd_size = total;*/

	FAT_File* file_data2 = FAT_Open(&disk, "/");
	FAT_DirectoryEntry entry;

	uint8_t* target1 = (uint8_t*)MEMORY_FAT_ADDR;
	uint32_t total1 = 0;
	int i = 0;

	while (FAT_ReadEntry(&disk, file_data2, &entry) && i++ < 64) {
		// copy 11-char FAT DIR/FILE name
		memcpy(target1 + total1, entry.Name, 11);
		total1 += 11;

		// add newline
		target1[total1++] = '\n';
	}

	FAT_Close(file_data2);

	boot_params.initrd_ptr  = (void*)target1;
	boot_params.initrd_size = total1;


	target1[total1] = 0;
		

    // execute kernel
	printf(BOOT_MSG MSG_DELIM INFO_KERNEL_EXEC);
    KernelStart kernelStart = (KernelStart)Kernel;
    kernelStart(&boot_params);

end:
    for (;;);
}
/*
void dbg_putc(char c){
	x68_outb(0xE9,c);
}

void dbg_puts(char* str){
	while(*str!='\0'){
		dbg_putc(*str);
		str++;
	}
}
*/