#include "io.h"

#define UNUSED_PORT 0x80

//Write into an unused port.
void i686_io_wait(){
	x86_outb(UNUSED_PORT,0);
}
