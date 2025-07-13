#include "keyboard.h"
#include "stdio.h"
#include <stdbool.h>
#include <command.h>

#define TIME_OUT_TICKS 1000
#define SCAN_KEY_UP 0x80
#define SCAN_IS_IN_PORT 0x64
#define SCAN_GET_PORT 0x60

#define KEYCODE_ENTER -1 //was \n
#define KEYCODE_CTRL -2
#define KEYCODE_LEFT_SHIFT -3
#define KEYCODE_RIGHT_SHIFT -4
#define KEYCODE_ALT -5
#define KEYCODE_CAPS_LOCK -6
bool caps_down=false;
#define KEYCODE_F1 -7
#define KEYCODE_F2 -8
#define KEYCODE_F3 -9
#define KEYCODE_F4 -10
#define KEYCODE_F5 -11
#define KEYCODE_F6 -12
#define KEYCODE_F7 -13
#define KEYCODE_F8 -14
#define KEYCODE_F9 -15
#define KEYCODE_F10 -16
#define KEYCODE_NUM_LOCK -17
bool num_lock_on=false;
#define KEYCODE_SCROLL_LOCK -18
bool scroll_lock_on=false;
#define KEYCODE_HOME -19
#define KEYCODE_PAGE_UP -20
#define KEYCODE_UP -21
#define KEYCODE_LEFT -22
#define KEYCODE_RIGHT -23
#define KEYCODE_DOWN -24
#define KEYCODE_END -25
#define KEYCODE_PAGE_DOWN -26
#define KEYCODE_INSERT -27
#define KEYCODE_DEL -28
#define KEYCODE_F11 -29
#define KEYCODE_F12 -30

//TODO make this not english keyboard
int language=0; //EN-0|
const char* lang_labels[]={"EN","HU","DE","XU"};

//EN

//Original from https://github.com/szhou42/osdev/blob/master/src/kernel/drivers/keyboard.c
const char kbdus_EN[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b',   /* Backspace */
    '\t',           /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', KEYCODE_ENTER,       /* Enter key */
    KEYCODE_CTRL,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   /* 39 */
    '\'', '`',   KEYCODE_LEFT_SHIFT,     /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',         /* 49 */
    'm', ',', '.', '/',   KEYCODE_RIGHT_SHIFT,                    /* Right shift */
    '*',
    KEYCODE_ALT,  /* Alt */
    ' ',    /* Space bar */
    KEYCODE_CAPS_LOCK,  /* Caps lock */
    KEYCODE_F1,  /* 59 - F1 key ... > */
    KEYCODE_F2,   KEYCODE_F3,   KEYCODE_F4,   KEYCODE_F5,   KEYCODE_F6,   KEYCODE_F7,   KEYCODE_F8,   KEYCODE_F9,
    KEYCODE_F10,  /* < ... F10 */
    KEYCODE_NUM_LOCK,  /* 69 - Num lock*/
    KEYCODE_SCROLL_LOCK,  /* Scroll Lock */
    KEYCODE_HOME,  /* Home key */
    KEYCODE_UP,  /* Up Arrow */
    KEYCODE_PAGE_UP,  /* Page Up */
    '-',
    KEYCODE_LEFT,  /* Left Arrow */
    0,
    KEYCODE_RIGHT,  /* Right Arrow */
    '+',
    KEYCODE_END,  /* 79 - End key*/
    KEYCODE_DOWN,  /* Down Arrow */
    KEYCODE_PAGE_DOWN,  /* Page Down */
    KEYCODE_INSERT,  /* Insert Key */
    KEYCODE_DEL,  /* Delete Key */
    0,   0,   0,
    KEYCODE_F11,  /* F11 Key */
    KEYCODE_F12,  /* F12 Key */
    0,  /* All other keys are undefined */
};

//DE y and z switched
const char kbdus_DE[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b',   /* Backspace */
    '\t',           /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'z', 'u', 'i', 'o', 'p', '[', ']', KEYCODE_ENTER,       /* Enter key */
    KEYCODE_CTRL,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'e',   /* 39 */
    's', 'u',   KEYCODE_LEFT_SHIFT,     /* Left shift */ //also s instead of ß
    'i', 'y', 'x', 'c', 'v', 'b', 'n',         /* 49 */
    'm', ',', '.', '/',   KEYCODE_RIGHT_SHIFT,                    /* Right shift */
    '*',
    KEYCODE_ALT,  /* Alt */
    ' ',    /* Space bar */
    KEYCODE_CAPS_LOCK,  /* Caps lock */
    KEYCODE_F1,  /* 59 - F1 key ... > */
    KEYCODE_F2,   KEYCODE_F3,   KEYCODE_F4,   KEYCODE_F5,   KEYCODE_F6,   KEYCODE_F7,   KEYCODE_F8,   KEYCODE_F9,
    KEYCODE_F10,  /* < ... F10 */
    KEYCODE_NUM_LOCK,  /* 69 - Num lock*/
    KEYCODE_SCROLL_LOCK,  /* Scroll Lock */
    KEYCODE_HOME,  /* Home key */
    KEYCODE_UP,  /* Up Arrow */
    KEYCODE_PAGE_UP,  /* Page Up */
    '-',
    KEYCODE_LEFT,  /* Left Arrow */
    0,
    KEYCODE_RIGHT,  /* Right Arrow */
    '+',
    KEYCODE_END,  /* 79 - End key*/
    KEYCODE_DOWN,  /* Down Arrow */
    KEYCODE_PAGE_DOWN,  /* Page Down */
    KEYCODE_INSERT,  /* Insert Key */
    KEYCODE_DEL,  /* Delete Key */
    0,   0,   0,
    KEYCODE_F11,  /* F11 Key */
    KEYCODE_F12,  /* F12 Key */
    0,  /* All other keys are undefined */
};

//HU - nincs ékezet
const char kbdus_HU[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', 'o', 'u', '\b',   /* Backspace */
    '\t',           /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'z', 'u', 'i', 'o', 'p', '[', ']', KEYCODE_ENTER,       /* Enter key */
    KEYCODE_CTRL,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'e',   /* 39 */
    'a', 'u',   KEYCODE_LEFT_SHIFT,     /* Left shift */
    'i', 'y', 'x', 'c', 'v', 'b', 'n',         /* 49 */
    'm', ',', '.', '/',   KEYCODE_RIGHT_SHIFT,                    /* Right shift */
    '*',
    KEYCODE_ALT,  /* Alt */
    ' ',    /* Space bar */
    KEYCODE_CAPS_LOCK,  /* Caps lock */
    KEYCODE_F1,  /* 59 - F1 key ... > */
    KEYCODE_F2,   KEYCODE_F3,   KEYCODE_F4,   KEYCODE_F5,   KEYCODE_F6,   KEYCODE_F7,   KEYCODE_F8,   KEYCODE_F9,
    KEYCODE_F10,  /* < ... F10 */
    KEYCODE_NUM_LOCK,  /* 69 - Num lock*/
    KEYCODE_SCROLL_LOCK,  /* Scroll Lock */
    KEYCODE_HOME,  /* Home key */
    KEYCODE_UP,  /* Up Arrow */
    KEYCODE_PAGE_UP,  /* Page Up */
    '-',
    KEYCODE_LEFT,  /* Left Arrow */
    0,
    KEYCODE_RIGHT,  /* Right Arrow */
    '+',
    KEYCODE_END,  /* 79 - End key*/
    KEYCODE_DOWN,  /* Down Arrow */
    KEYCODE_PAGE_DOWN,  /* Page Down */
    KEYCODE_INSERT,  /* Insert Key */
    KEYCODE_DEL,  /* Delete Key */
    0,   0,   0,
    KEYCODE_F11,  /* F11 Key */
    KEYCODE_F12,  /* F12 Key */
    0,  /* All other keys are undefined */
};

//XU french - the letters Q and A are swapped and the letters Z and W are swapped. The letter M is moved up a row.
const char kbdus_FR[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b',   /* Backspace */
    '\t',           /* Tab */
    'a', 'z', 'e', 'r', /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', KEYCODE_ENTER,       /* Enter key */
    KEYCODE_CTRL,          /* 29   - Control */
    'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   /* 39 */
    '\'', '`',   KEYCODE_LEFT_SHIFT,     /* Left shift */
    'm', 'w', 'x', 'c', 'v', 'b', 'n',         /* 49 */
    '\\', ',', '.', '/',   KEYCODE_RIGHT_SHIFT,                    /* Right shift */
    '*',
    KEYCODE_ALT,  /* Alt */
    ' ',    /* Space bar */
    KEYCODE_CAPS_LOCK,  /* Caps lock */
    KEYCODE_F1,  /* 59 - F1 key ... > */
    KEYCODE_F2,   KEYCODE_F3,   KEYCODE_F4,   KEYCODE_F5,   KEYCODE_F6,   KEYCODE_F7,   KEYCODE_F8,   KEYCODE_F9,
    KEYCODE_F10,  /* < ... F10 */
    KEYCODE_NUM_LOCK,  /* 69 - Num lock*/
    KEYCODE_SCROLL_LOCK,  /* Scroll Lock */
    KEYCODE_HOME,  /* Home key */
    KEYCODE_UP,  /* Up Arrow */
    KEYCODE_PAGE_UP,  /* Page Up */
    '-',
    KEYCODE_LEFT,  /* Left Arrow */
    0,
    KEYCODE_RIGHT,  /* Right Arrow */
    '+',
    KEYCODE_END,  /* 79 - End key*/
    KEYCODE_DOWN,  /* Down Arrow */
    KEYCODE_PAGE_DOWN,  /* Page Down */
    KEYCODE_INSERT,  /* Insert Key */
    KEYCODE_DEL,  /* Delete Key */
    0,   0,   0,
    KEYCODE_F11,  /* F11 Key */
    KEYCODE_F12,  /* F12 Key */
    0,  /* All other keys are undefined */
};

bool global_free_mode=false;
void keystroke_reg(Registers* regs){
	unsigned int scancode; //=x86_inb(0x60);
	//printf("%c",scancode);
	int i;
    //wait for scancode
    for(i = TIME_OUT_TICKS; i > 0; i++) {
		
        // Check if scan code is ready
        if((x86_inb(SCAN_IS_IN_PORT) & 1) == 0) continue;
        // Get the scan code
        scancode = x86_inb(SCAN_GET_PORT);
        break;
    }
	
    if(i > 0) {
        if(scancode & SCAN_KEY_UP) {
            //TODO key released
        }
        else {
			if(kbdus_EN[scancode]<0){
				switch(kbdus_EN[scancode]){
					case KEYCODE_ENTER:
						//TRY parse command
						int whatdo=try_parse_command();
						
						//success, 2 is freemode change, 3 is cmd mode change
						if(whatdo==2){
							global_free_mode=true;
						} else if(whatdo==3){
							global_free_mode=false;
						}
						printf("\n");
						break;
					//...
					case KEYCODE_CTRL:
						break;
					case KEYCODE_LEFT_SHIFT:
						break;
					case KEYCODE_RIGHT_SHIFT:
						break;
					case KEYCODE_ALT:
						break;
					case KEYCODE_CAPS_LOCK:
						if(caps_down==true){
							caps_down=false;
						} else {
							caps_down=true;
						}
						break;
					case KEYCODE_F1: //TODO map this system out (F keys)
						break;
					case KEYCODE_F2:
						break;
					case KEYCODE_F3:
						break;
					case KEYCODE_F4:
						break;
					case KEYCODE_F5:
						break;
					case KEYCODE_F6:
						break;
					case KEYCODE_F7:
						break;
					case KEYCODE_F8:
						break;
					case KEYCODE_F9:
						break;
					case KEYCODE_F10:
						break;
					case KEYCODE_F11:
						break;
					case KEYCODE_F12:
						break;
					case KEYCODE_NUM_LOCK:
						if(num_lock_on==true){
							num_lock_on=false;
						} else {
							num_lock_on=true;
						}
						break;
					case KEYCODE_SCROLL_LOCK:
						if(scroll_lock_on==true){
							scroll_lock_on=false;
						} else {
							scroll_lock_on=true;
						}
						break;
					case KEYCODE_HOME:
						break;
					case KEYCODE_PAGE_UP:
						break;
					case KEYCODE_PAGE_DOWN:
						break;
					case KEYCODE_END:
						break;
					case KEYCODE_UP: //arrows
						if(global_free_mode==true){
							move_cursor(0,-1);
						}
						break;
					case KEYCODE_DOWN: //arrows
						if(global_free_mode==true){
							move_cursor(0,1);
						}
						break;
					case KEYCODE_RIGHT: //arrows
						move_cursor(1,0);
						break;
					case KEYCODE_LEFT: //arrows
						move_cursor(-1,0);
						break;
					case KEYCODE_INSERT: //switch char placement mode
						break;
					case KEYCODE_DEL: //set cur char to 0;
						putc('\b');
						break;
		
				}
			} else {
				char cur_bet;
				switch(language){
					case 0:
						cur_bet=kbdus_EN[scancode];
						break;
					case 1:
						cur_bet=kbdus_DE[scancode];
						break;
					case 2:
						cur_bet=kbdus_HU[scancode];
						break;
					case 3:
						cur_bet=kbdus_FR[scancode];
						break;
				}
				printf("%c", cur_bet);
			}
            
        }
    }
}

void init_keyboard(){
	caps_down=false;
	num_lock_on=false;
	scroll_lock_on=false;
	language=0;
	
	register_commands();
	
	i686_IRQ_reg_handlers(1,keystroke_reg);
}

void change_language(int l_id){
	language=l_id; //set to lang id
}