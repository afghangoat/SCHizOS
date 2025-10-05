#pragma once

#include <fs/vfs.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "memdefs.h"
#include "memory.h"

static MemoryFileEntry GlobalEntries[MAX_FILE_ENTRIES_IN_LIVE_BOOT];
static int LoadedFileSizes=0;

extern char* initrd;
extern uint32_t initrd_size;

//MemoryFileEntry copyToFile(const char* data);

//int a();

void printRoot();

bool readFromBoot(MemoryFileEntry entries[],uint32_t file_count);

char* getFileContents(const char* filename);