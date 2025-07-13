#pragma once

#include "isr.h"

typedef void (*IRQ_handler)(Registers* regs);

void i686_IRQ_Init();
void i686_IRQ_reg_handlers(int irq,IRQ_handler handler);