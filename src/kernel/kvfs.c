#include "kvfs.h"

#include <stdbool.h>
#include <string.h>

char* initrd=NULL;
uint32_t initrd_size=0;

MemoryFileEntry copyToFile(const char* data) {
    MemoryFileEntry entry;
    
	for(int i=0;i<MAX_FILE_SIZE_IN_LIVE_BOOT;i++){
		entry.FileContents[i]=data[i];
		
		if(data[i]=='\0'){
			break;
		}
	}

    return entry;
}

int a(){
	return 1;
}

bool readFromBoot(MemoryFileEntry entries[],uint32_t file_count){
	
	for(uint32_t i=0;i<file_count;i++){
		GlobalEntries[i] = entries[i];
	}
	LoadedFileSizes=file_count;
	return true;
}


char* getFileContents(const char* filename) {
	bool FileFound = false;
	
    for (int i = 0; i < LoadedFileSizes; i++) {

        FileFound = true;
        for (int j = 0; j < FAT_NAME_LENGTH; j++) {
            if (GlobalEntries[i].FileName[j] != filename[j]) {
                FileFound = false;
                break;
            }
        }

        if (FileFound) {
            return GlobalEntries[i].FileContents;
        }
    }

    return NULL;
}

void printRoot(){
	for (uint32_t i = 0; i < initrd_size; i += FAT_NAME_LENGTH + 1) { //Fat12 -> 11 byte + new line
		if(initrd[i]=='\0'){
			break;
		}
		for (int j = 0; j < 11 && i+j < initrd_size; j++) {
			putc(initrd[i+j]);
		}
		putc('\n');
	}
}