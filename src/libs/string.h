#pragma once

typedef unsigned size_t;

//TODO implement the rest
//Finds the first char occurence in a give string and returns a pointer to it.
const char* strchr(const char* str,char chr);

//Copies the src string to the dst string pointer.
char* strcpy(char* dst,char* src);

size_t strlen(const char* str);
//TODO excercise implement ALL, memory.h ------

//Returns a pointer to the end of the string.
char* end_of_str(char* str);

char *strcat(char *dest, const char *src);
//Appends the string pointed to, by src to the end of the string pointed to by dest.

char *strncat(char *dest, const char *src, size_t n);
//Appends the string pointed to, by src to the end of the string pointed to, by dest up to n characters long.

int strcmp(const char *str1, const char *str2);
//Compares the string pointed to, by str1 to the string pointed to by str2.

int strncmp(const char *str1, const char *str2, size_t n);
//Compares at most the first n bytes of str1 and str2.

char *strncpy(char *dest, const char *src, size_t n);
//Copies up to n characters from the string pointed to, by src to dest.

size_t strcspn(const char *str1, const char *str2);
//Calculates the length of the initial segment of str1 which consists entirely of characters not in str2.

char *strerror(int errnum);
//Searches an internal array for the error number errnum and returns a pointer to an error message string.

char *strpbrk( char *str1, char *str2);
//Finds the first character in the string str1 that matches any character specified in str2.

char *strrchr(char *str, int c);
//Searches for the last occurrence of the character c (an unsigned char) in the string pointed to by the argument str.

size_t strspn(const char *str1, const char *str2);
//Calculates the length of the initial segment of str1 which consists entirely of characters in str2.

char *strstr(const char *haystack, const char *needle);
//Finds the first occurrence of the entire string needle (not including the terminating null character) which appears in the string haystack.
