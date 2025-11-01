#include "kvfs.h"

#include <stdbool.h>
#include <string.h>
bool isupper(char chr){
	return chr >= 'A' && chr <= 'Z';
}	//Checks whether a character is an uppercase letter

char tolower(char chr){
	if(isupper(chr)==true){
		return chr+32;
	} else {
		return chr;
	}
}	//Returns a lowercase version of a character


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

bool isFileInDir(int fileIndex){
	bool isIn = true;
		
	for (int j = 0; j < MAX_DIR_NAME_DEPTH; j++) {
        if (tolower(GlobalEntries[fileIndex].InDir[j]) != CurrentPath[j]) {
            isIn = false;
            break;
        } else if(GlobalEntries[fileIndex].InDir[j] == '\0'){
			break;
		}
    }
	return isIn;
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
			bool isIn = isFileInDir(i);
			if(isIn == true){
				return GlobalEntries[i].FileContents;
			} else {
				printf("\n" CMD_ERR MSG_DELIM ERR_CMD_FILE_NOT_FOUND_IN_DIR);
				return "";
			}
			
            
        }
    }
	printf("\n" CMD_ERR MSG_DELIM ERR_CMD_FILE_NOT_FOUND_IN_DIR);
    return "";
}

void pwd(){
	printf("Current directory: %s",CurrentPath);
	if(CurrentPath[0]==ROOT_SYMBOL_CHAR){
		printf(" (root)");
	}
	
	printf("\n");
}

void printRealName(const char* fname){
	for(int i=0;i<FAT_NAME_LENGTH;i++){
		printf("%c",fname[i]);
		if(i == 7){
			printf(".");
		}
	}
}

void printFilesInPath(){
	
	for (int i=0;i<LoadedFileSizes;i++){
		bool isIn = isFileInDir(i);

        if (isIn == true) {
            printf("  ");
			printRealName(GlobalEntries[i].FileName);
			printf("\n");
        }
	}
}

void setCurrentPath(const char* newpath){
	int i=0;
	while(newpath[i]!='\0'||i<FAT_NAME_LENGTH){
		CurrentPath[i]=newpath[i];
		
		i++;
	}
	CurrentPath[i]='\0';
}
void setPathToRoot(){
	for(int i=0;i<MAX_DIR_NAME_DEPTH;i++){
		CurrentPath[i]='\0';
	}
	CurrentPath[0]=ROOT_SYMBOL_CHAR;
}

void printDir(){
	if (CurrentPath[0]==ROOT_SYMBOL_CHAR){
		printRoot();
	} else {
		printf("\n%s:\n",CurrentPath);
		printFilesInPath();
	}
}

void printRoot(){
	for (uint32_t i = 0; i < initrd_size; i += FAT_NAME_LENGTH + 1) { //Fat12 -> 11 byte + new line
		if(initrd[i]=='\0'){
			break;
		}
		for (int j = 0; j < FAT_NAME_LENGTH && i+j < initrd_size; j++) {
			putc(initrd[i+j]);
		}
		putc('\n');
	}
}