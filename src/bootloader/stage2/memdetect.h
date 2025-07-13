#pragma once

#include "x86.h"
#include "stdio.h"
#include <boot/boot_params.h>

#define MAX_REGIONS 256

void memory_detect(MemoryInfo* memory_info);