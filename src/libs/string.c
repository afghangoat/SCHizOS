#include <string.h>
#include <stdint.h>
#include <math.h>
#include <memdefs.h> //for NULL

int internal_error_array[5]={0,0,0,0,0};
char* err_msg_strings[5]={
	"strchr returned NULL",
	"Err 2",
	"Err 3",
	"Err 4",
	"Err 5"
};
//For str error

const char* strchr(const char* str,char chr){
	if(str==NULL){
		return NULL;
	}
	while(*str!='\0'){
		if(*str==chr){
			return str;
		}
		str++;
	}
	return NULL;
}

char* strcpy(char* dst,char* src){
	char* original_dst=dst;
	if(dst==NULL){
		return NULL;
	}
	if(src==NULL){
		*dst='\0';
		return dst;
	}
	while(*src!='\0'){
		*dst=*src;
		++dst;
		++src;
	}
	*dst='\0';
	return original_dst;
}

unsigned strlen(const char* str){
	unsigned len=0;
	while(*str!='\0'){
		len++;
		++str;
	}
	return len;
}

//For todo: THESE ARE NOT MEMORY SAFE
char* end_of_str(char* str){
	unsigned len=0;
	while(*str!='\0'){
		len++;
		++str;
	}
	
	return str;
}
char *strcat(char *dest, const char *src){
	char* end_dest =end_of_str(dest);
	unsigned srclen=strlen(src);
	
	for(unsigned i=0;i<srclen;i++){
		*end_dest=src[i];
		++end_dest;
	}
	*end_dest='\0';
	return dest;
}
//Appends the string pointed to, by src to the end of the string pointed to by dest.

char *strncat(char *dest, const char *src, size_t n){
	char* end_dest =end_of_str(dest);
	unsigned srclen=strlen(src);
	srclen=min(n,srclen);
	
	for(unsigned i=0;i<srclen;i++){
		*end_dest=src[i];
		++end_dest;
	}
	*end_dest='\0';
	return dest;
}
//Appends the string pointed to, by src to the end of the string pointed to, by dest up to n characters long.

int strcmp(const char *str1, const char *str2){
	unsigned str1_len=strlen(str1);
	unsigned str2_len=strlen(str2);
	if(str1_len>str2_len){
		return -2;
	} else if(str1_len<str2_len){
		return -1;
	} else {
		
		for(unsigned i=0;i<str1_len;i++){
			if(str1[i]!=str2[i]){
				return 0;
			}
		}
	}
	return 1;
}
//Compares the string pointed to, by str1 to the string pointed to by str2.

int strncmp(const char *str1, const char *str2, size_t n){
	unsigned str1_len=strlen(str1);
	str1_len=min(n,str1_len);
	unsigned str2_len=strlen(str2);
	str2_len=min(n,str2_len);
		
	for(unsigned i=0;i<str1_len;i++){
		if(str1[i]!=str2[i]){
			return 0;
		}
	}
	return 1;
}
//Compares at most the first n bytes of str1 and str2.

char *strncpy(char *dest, const char *src, size_t n){
	char* original_dst=dest;
	if(dest==NULL){
		return NULL;
	}
	if(src==NULL){
		*dest='\0';
		return dest;
	}
	size_t counter=0;
	while(*src!='\0'||counter!=n){
		*dest=*src;
		++dest;
		++src;
		++counter;
	}
	*dest='\0';
	return original_dst;
}
//Copies up to n characters from the string pointed to, by src to dest.

size_t strcspn(const char *str1, const char *str2){
	
	if(str1==NULL){
		return 0;
	}
	if(str2==NULL){
		return strlen(str1);
	}
	
	size_t str1_len=strlen(str1);
	for(size_t i=0;i<str1_len;i++){
		if(strchr(str2,str1[i])!=NULL){
			return i;
		}
	}
	
	return str1_len;
}
//Calculates the length of the initial segment of str1 which consists entirely of characters not in str2.

char *strerror(int errnum){
	if(internal_error_array[errnum]==0){
		return NULL;
	}
	return err_msg_strings[errnum];
}
//Searches an internal array for the error number errnum and returns a pointer to an error message string.

char *strpbrk( char *str1, char *str2){
	if(str1==NULL){
		return NULL;
	}
	if(str2==NULL){
		return NULL;
	}
	size_t str1_len=strlen(str1);
	for(size_t i=0;i<str1_len;i++){
		if(strchr(str2,str1[i])!=NULL){
			return str2;
		}
		str1++;
	}
	return NULL;
}
//Finds the first character in the string str1 that matches any character specified in str2.

char *strrchr(char *str, int c){
	char* ret=NULL;
	size_t str1_len=strlen(str);
	for(size_t i=0;i<str1_len;i++){
		if(strchr(str,c)!=NULL){
			ret=str;
		}
		str++;
	}
	return ret;
}
//Searches for the last occurrence of the character c (an unsigned char) in the string pointed to by the argument str.

size_t strspn(const char *str1, const char *str2){
	if(str1==NULL){
		return 0;
	}
	if(str2==NULL){
		return 0;
	}
	size_t str1_len=strlen(str1);
	for(size_t i=0;i<str1_len;i++){
		if(strchr(str2,str1[i])==NULL){
			return i;
		}
	}
	return str1_len;
}
//Calculates the length of the initial segment of str1 which consists entirely of characters in str2.

char *strstr(const char *haystack, const char *needle){
if (haystack == NULL || needle == NULL) {
        return NULL;
    }

    size_t haystack_len = strlen(haystack);
    size_t needle_len = strlen(needle);

    // Special case: if `needle` is an empty string, return `haystack`
    if (needle_len == 0) {
        return (char *)haystack;
    }

    // Iterate through `haystack` to find the substring
    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        // Check if the substring matches
        if (strncmp(&haystack[i], needle, needle_len) == 0) {
            return (char *)&haystack[i]; // Return pointer to the start of the substring
        }
    }

    // Return NULL if no match is found
    return NULL;
}
//Finds the first occurrence of the entire string needle (not including the terminating null character) which appears in the string haystack.
