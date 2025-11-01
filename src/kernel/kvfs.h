#pragma once

#include <fs/vfs.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "memdefs.h"
#include "memory.h"

#define ROOT_SYMBOL "#"
#define ROOT_SYMBOL_CHAR '#'

static MemoryFileEntry GlobalEntries[MAX_FILE_ENTRIES_IN_LIVE_BOOT];
static int LoadedFileSizes=0;
static char CurrentPath[MAX_DIR_NAME_DEPTH]=ROOT_SYMBOL;

extern char* initrd;
extern uint32_t initrd_size;

//MemoryFileEntry copyToFile(const char* data);

//int a();

void printRoot();

void printDir();

void setCurrentPath(const char* newpath);
void setPathToRoot();

void pwd();

bool readFromBoot(MemoryFileEntry entries[],uint32_t file_count);

char* getFileContents(const char* filename);