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

void __attribute__((cdecl)) start(uint16_t bootDrive){
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
	
	// browse files in root
    /*FAT_File* file_data = FAT_Open(&disk, "/");
    FAT_DirectoryEntry entry;
    int i = 0;
    while (FAT_ReadEntry(&disk, file_data, &entry) && i++ < 5){
        printf(TAB_PAD);
        for (int i = 0; i < 11; i++)
            putc(entry.Name[i]);
        printf("\r\n");
    }*/
    //FAT_Close(file_data);
	//puts("Hello world from C!\r\n");
	//printf("AAAA %s aaa %d","bb",21);
	/*
    // read test.txt
    char buffer[100];
    uint32_t read;
    file_data = FAT_Open(&disk, "mydir/test.txt");
    while ((read = FAT_Read(&disk, file_data, sizeof(buffer), buffer))){
        for (uint32_t i = 0; i < read; i++){
            if (buffer[i] == '\n'){
                putc('\r');
			}
            putc(buffer[i]);
        }
    }
    FAT_Close(file_data);*/
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
