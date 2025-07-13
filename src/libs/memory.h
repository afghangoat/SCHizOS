#pragma once
#include <string.h> //For size_t
#include <stdint.h>
#include "fast_memcpy.h"
#include <memdefs.h>

//copies one pointer content with num of bytes to another destination pointer.
void* memcpy(void* dst, const void* src,uint16_t num);

//sets the data at the pointer at a given lengthy value
void* memset(void* ptr, int value,uint16_t num);

//compares 2 pointer data to a specific lengths.
int memcmp(const void* ptr1,const void* ptr2,uint16_t num);

//TODO
void *memchr(const void *str, int c, size_t n);
//Searches for the first occurrence of the character c (an unsigned char) in the first n bytes of the string pointed to, by the argument str.

void *memmove(void *dest, const void *src, size_t n);
//Another function to copy n characters from str2 to str1.
