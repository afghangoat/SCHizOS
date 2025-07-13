#pragma once
#include <stdint.h>
#include <stdbool.h>

bool __attribute__((cdecl)) x86_rand(uint32_t *rptr);
