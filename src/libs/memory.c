#include "memory.h"

void* memcpy(void* dst, const void* src,uint16_t num){
	uint8_t* u8Dst=(uint8_t*)dst;
	const uint8_t* u8Src=(const uint8_t*)src;
	
	for(uint16_t i=0;i<num;i++){
		u8Dst[i]=u8Src[i];
	}
	return dst;
}

void* memset(void* ptr, int value,uint16_t num){
	uint8_t* u8ptr=(uint8_t*)ptr;
	for(uint16_t i=0;i<num;i++){
		u8ptr[i]=(uint8_t)value;
	}
	return ptr;
}

int memcmp(const void* ptr1,const void* ptr2,uint16_t num){
	const uint8_t* u8ptr1=(const uint8_t*)ptr1;
	const uint8_t* u8ptr2=(const uint8_t*)ptr2;
	
	for(uint16_t i=0;i<num;i++){
		if(u8ptr1[i]!=u8ptr2[i]){
			return 1;
		}
	}
	return 0;
}

void *memchr(const void *str, int c, size_t n){
	if (str == NULL) {
        return NULL;
    }
    const unsigned char *ptr = (const unsigned char *)str; // Cast to unsigned char for byte comparison
    unsigned char target = (unsigned char)c;              // Convert `c` to unsigned char

    for (size_t i = 0; i < n; i++) {
        if (ptr[i] == target) { // Compare each byte
            return (void *)(ptr + i); // Return pointer to the found byte
        }
    }
    return NULL; // Return NULL if not found
}
//Searches for the first occurrence of the character c (an unsigned char) in the first n bytes of the string pointed to, by the argument str.

void *memmove(void *dest, const void *src, size_t n){
	if (dest == NULL || src == NULL) {
        return NULL;
    }
    unsigned char *d = (unsigned char *)dest;         // Cast destination to unsigned char *
    const unsigned char *s = (const unsigned char *)src; // Cast source to unsigned char *

    if (d < s || d >= s + n) {
        // Non-overlapping regions, copy forward
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        // Overlapping regions, copy backward
        for (size_t i = n; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }
    return dest;
}
//Another function to copy n characters from str2 to str1.
