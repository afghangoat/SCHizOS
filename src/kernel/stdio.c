#include "stdio.h"
#include <arch/i686/io.h>

#include <stdarg.h>

enum vga_color { //use these if possible
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

const unsigned SCREEN_WIDTH=80;
const unsigned SCREEN_HEIGHT=25;

const uint8_t DEFAULT_COLOR=0x7;

uint8_t* global_screen_buffer=(uint8_t*)0xB8000;
int global_screen_x=0;
int global_screen_y=0;

uint8_t global_color=0; //0 for disabling

bool en_freemode=false;
void enable_free_mode(bool en){
	en_freemode=en;
}
void set_global_color(uint8_t what){
	//putcolor
	global_color=what;
}
void putcolor(int x,int y,uint8_t color){
	global_screen_buffer[2*(y*SCREEN_WIDTH+x)+1]=color;
}
//16 color text mode
void putchr(int x,int y,char c){
	//x86_Video_WriteCharTeletype(c,0);
	global_screen_buffer[2*(y*SCREEN_WIDTH+x)]=c;
	if(global_color!=0){
		putcolor(x,y,global_color);
	}
}
char getchr(int x,int y){
	return global_screen_buffer[2*(y*SCREEN_WIDTH+x)];
}
uint8_t getcolor(int x,int y){
	return global_screen_buffer[2*(y*SCREEN_WIDTH+x)+1];
}
void setcursor(int x, int y)
{
    int pos = y * SCREEN_WIDTH + x;

    x86_outb(0x3D4, 0x0F); //SEND instructions to vga chip
    x86_outb(0x3D5, (uint8_t)(pos & 0xFF));
    x86_outb(0x3D4, 0x0E);
    x86_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
void move_cursor(int x,int y){
	global_screen_y+=y;
	global_screen_x+=x;
	
	if(global_screen_x>=SCREEN_WIDTH){
		global_screen_x=SCREEN_WIDTH;
	}
	if(global_screen_x<0){
		global_screen_x=0;
	}
	
	if(global_screen_y>=SCREEN_HEIGHT){
		global_screen_y=SCREEN_HEIGHT;
	}
	if(global_screen_y<0){
		global_screen_y=0;
	}
	
	setcursor(global_screen_x,global_screen_y);
}

void clrscr(){
    for (int y = 0; y < SCREEN_HEIGHT; y++){
        for (int x = 0; x < SCREEN_WIDTH; x++){
            putchr(x, y, '\0');
            putcolor(x, y, DEFAULT_COLOR);
        }
	}
    global_screen_x = 0;
    global_screen_y = 0;
    setcursor(global_screen_x, global_screen_y);
}
void getcursor(int* xx,int* yy){
	*yy=global_screen_y;
	*xx=global_screen_x;
}

void scrollback(int lines){
    for (int y = lines; y < SCREEN_HEIGHT; y++){
        for (int x = 0; x < SCREEN_WIDTH; x++){
            putchr(x, y - lines, getchr(x, y));
            putcolor(x, y - lines, getcolor(x, y));
        }
	}
    for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++){
        for (int x = 0; x < SCREEN_WIDTH; x++){
            putchr(x, y, '\0');
            putcolor(x, y, DEFAULT_COLOR);
        }
	}
    global_screen_y -= lines;
}
/*void puts(const char* str){
	while(*str!='\0'){
		putc(*str);
		str++;
	}
}
void puts_f(const char far* str)
{
    while(*str!='\0'){
        putc(*str);
        str++;
    }
}*/
void putc(char c){
    switch (c){
        case '\n':
            global_screen_x = 0;
            global_screen_y++;
            break;
    
        case '\t':
            for (int i = 0; i < 4 - (global_screen_x % 4); i++)
                putc(' ');
            break;

        case '\r':
            global_screen_x = 0;
            break;
		
		case '\b': //backslash
			putc(' ');
            global_screen_x-=2;
			if(global_screen_x<0){
				if(en_freemode==true){
					global_screen_x=SCREEN_WIDTH-1;
					global_screen_y--;
					if(global_screen_y<0){
						global_screen_y=0;
					}
				} else {
					global_screen_x=0;
				}
				
			}
            break;

        default:
            putchr(global_screen_x, global_screen_y, c);
            global_screen_x++;
            break;
    }

    if (global_screen_x >= SCREEN_WIDTH){
        global_screen_y++;
        global_screen_x = 0;
    }
	
    if (global_screen_y >= SCREEN_HEIGHT){
        scrollback(1);
	}

    setcursor(global_screen_x, global_screen_y);
}

void puts(const char* str){
    while(*str!='\0'){
        putc(*str);
        str++;
    }
}

#define PRINTF_NORMAL 0
#define PRINTF_LEN 1
#define PRINTF_LEN_SHORT 2
#define PRINTF_LEN_LONG 3
#define PRINTF_SPEC_STATE 4


#define P_LEN_DEF 0
#define P_LEN_SHORT 1
#define P_LEN_SHORT_SHORT 2
#define P_LEN_LONG 3
#define P_LEN_LONG_LONG 4

const char hex_chars[]="0123456789abcdef";
void printf_unsigned(unsigned long long number, int radix){
    char buffer[32];
    int pos = 0;

    // convert number to ASCII
    do {
        unsigned long long rem = number % radix;
        number /= radix;
        buffer[pos++] = hex_chars[rem];
    } while (number > 0);

    //Print buffer in rev, because the last char is the first
    while (--pos >= 0){
        putc(buffer[pos]);
	}
}

void printf_signed(long long number, int radix){
    if (number < 0){
        putc('-');
        printf_unsigned(-number, radix);
    }
    else{
		printf_unsigned(number, radix);
	}
}
/*int* printf_num(int* argp,int length,bool sign,int radix){
	char buffer[32];
	unsigned long long number;
	int number_sign=1;
	int pos=0;
	switch(length){
		case P_LEN_SHORT:
		case P_LEN_SHORT_SHORT:
		case P_LEN_DEF:
			if(sign==true){
				int n=*argp;
				if(n<0){
					n=-n;
					number_sign=-1;
				}
				number=(unsigned long long)n;
			} else {
				number= *(unsigned int*)argp;
			}
			argp++;
			break;
		case P_LEN_LONG:
			if(sign==true){
				long int n=*(long int*)argp;
				if(n<0){
					n=-n;
					number_sign=-1;
				}
				number=(unsigned long long)n;
			} else {
				number= *(unsigned long int*)argp;
			}
			argp+=2;
			break;
		case P_LEN_LONG_LONG:
			if(sign==true){
				long int n=*(long long int*)argp;
				if(n<0){
					n=-n;
					number_sign=-1;
				}
				number=(unsigned long long)n;
			} else {
				number= *(unsigned long long int*)argp;
			}
			argp+=4;
			break;
	}
	//Tostring
	do{
		uint32_t rem;
		x86_div64_32(number,radix,&number,&rem);
		buffer[pos]=hex_chars[rem];
		pos++;
	} while(number>0);
	
	if(sign==true&&number_sign<0){
		buffer[pos]='-';
		pos++;
	}
	
	while(--pos>=0){
		putc(buffer[pos]);
	}
	return argp;
}*/

void printf(const char* fmt,...){
	
	//int* argp = (int*)&fmt;
	va_list args;
    va_start(args, fmt);
	
	int state=PRINTF_NORMAL;
	int length=P_LEN_DEF;
	int radix=10;
	bool sign=false;
	bool number=false;
	
	
	while(*fmt){
		switch(state){
			case PRINTF_NORMAL:
				switch(*fmt){
					case '%':
						state=PRINTF_LEN;
						break;
					default:
						putc(*fmt);
						break;
				}
				break;
			case PRINTF_LEN:
				switch(*fmt){
					case 'h':
						length=P_LEN_SHORT;
						state=PRINTF_LEN_SHORT;
						break;
					case 'l':
						length=P_LEN_LONG;
						state=PRINTF_LEN_LONG;
						break;
					default:
						goto PRINTF_SPEC_STATE_L;
				}
				break;
			case PRINTF_LEN_SHORT:
				if(*fmt =='h'){
					length=P_LEN_SHORT_SHORT;
					state=PRINTF_SPEC_STATE;
				} else {
					goto PRINTF_SPEC_STATE_L;
				}
				break;
			case PRINTF_LEN_LONG:
				if(*fmt =='l'){
					length=P_LEN_LONG_LONG;
					state=PRINTF_SPEC_STATE;
				} else {
					goto PRINTF_SPEC_STATE_L;
				}
				break;
			case PRINTF_SPEC_STATE:
			PRINTF_SPEC_STATE_L:
				switch(*fmt){
					case 'c':
						putc((char)va_arg(args,int));
						break;
					case 's':
						puts(va_arg(args,const char*));
						break;
					case '%':
						putc('%');
						break;
					case 'd':
					case 'i':
						radix=10;
						sign=true;
						number=true;
						break;
					case 'u':
						radix=10;
						sign=false;
						number=true;
						break;
					case 'X':
					case 'x':
					case 'p':
						radix=16;
						sign=false;
						number=true;
						break;
					case 'o':
						radix=8;
						sign=false;
						number=true;
						break;
					default:
						//Ignore
						puts(IO_ERR MSG_DELIM ERR_BAD_PRINTF_FORMAT);
						break;
				}
				
				if(number==true){
					if(sign==true){
						switch(length){
							case P_LEN_SHORT_SHORT:
							case P_LEN_SHORT:
							case P_LEN_DEF:
								printf_signed(va_arg(args,int),radix);
								break;
							case P_LEN_LONG:
								printf_signed(va_arg(args,long),radix);
								break;
							case P_LEN_LONG_LONG:
								printf_signed(va_arg(args,long long),radix);
								break;
							
						}
					} else {
						switch(length){
							case P_LEN_SHORT_SHORT:
							case P_LEN_SHORT:
							case P_LEN_DEF:
								printf_unsigned(va_arg(args,int),radix);
								break;
							case P_LEN_LONG:
								printf_unsigned(va_arg(args,long),radix);
								break;
							case P_LEN_LONG_LONG:
								printf_unsigned(va_arg(args,long long),radix);
								break;
							
						}
					}
				}
				state=PRINTF_NORMAL;
				length=P_LEN_DEF;
				radix=10;
				sign=false;
				number=false; //IF ERROR THIS IS
				break;
			default:
				//TODO error back here
				break;
		}
		fmt++;
	}
	va_end(args);
}

void print_buffer(const char* msg, const void* buffer, uint32_t count){
    const uint8_t* u8Buffer = (const uint8_t*)buffer;
    
    puts(msg);
    for (uint16_t i = 0; i < count; i++){
        putc(hex_chars[u8Buffer[i] >> 4]);
        putc(hex_chars[u8Buffer[i] & 0xF]);
    }
    puts("\n");
}