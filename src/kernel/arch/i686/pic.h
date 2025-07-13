#pragma once
#include <stdint.h>

void i686_PIC_Conf(uint8_t offset_pic1,uint8_t offset_pic2);
void i686_PIC_send_eoi(int irq);

void i686_PIC_mask(int irq);
void i686_PIC_un_mask(int irq);

void i686_PIC_disable();

uint16_t i686_PIC_read_irq();
uint16_t i686_PIC_read_in_service();
