#pragma once
#include <stdint.h>
#include <stdlang.h>
#include <stdbool.h>

void clrscr();
void putc(char c);
void puts(const char* str);
void printf(const char* fmt, ...);
void print_buffer(const char* msg, const void* buffer, uint32_t count);
void putcolor(int x,int y,uint8_t color);
void setcursor(int x, int y);
void getcursor(int* xx,int* yy);
char getchr(int x,int y);
void set_global_color(uint8_t what);
void enable_free_mode(bool en);
void move_cursor(int x,int y);
