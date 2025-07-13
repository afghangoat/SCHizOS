#include "audio.h"
#include <arch/i686/io.h>

#define SPEAKER_PORT 0x61

void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
    //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	x86_outb(0x43, 0xb6);
 	x86_outb(0x42, (uint8_t) (Div) );
 	x86_outb(0x42, (uint8_t) (Div >> 8));
 
    //And play the sound using the PC speaker
 	tmp = x86_inb(SPEAKER_PORT);
  	if (tmp != (tmp | 3)) {
 		x86_outb(SPEAKER_PORT, tmp | 3);
 	}
}
 
//make it shut up
void nosound() {
 	uint8_t tmp = x86_inb(SPEAKER_PORT) & 0xFC;
 	x86_outb(SPEAKER_PORT, tmp);
	
}
