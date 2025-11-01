#pragma once

#define MAX_FILE_ENTRIES_IN_LIVE_BOOT 10

#define MAX_FILE_SIZE_IN_LIVE_BOOT 512

#define MAX_DIR_NAME_DEPTH 255

#ifndef FAT_NAME_LENGTH
	#define FAT_NAME_LENGTH 11
#endif

typedef struct {
	char FileContents[MAX_FILE_SIZE_IN_LIVE_BOOT];
	char FileName[FAT_NAME_LENGTH];
	char InDir[MAX_DIR_NAME_DEPTH];
} MemoryFileEntry;

MemoryFileEntry copyToFile(const char* data);

int a();