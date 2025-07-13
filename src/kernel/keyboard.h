#pragma once
#include <arch/i686/isr.h>
#include <arch/i686/irq.h>
#include <arch/i686/io.h>

void keystroke_reg(Registers* regs);
void init_keyboard();

//See the keyboard.c file for more information.
void change_language(int l_id);
