#include <arch/i686/rnd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <memdefs.h>
#include <exclusive.h>

#define version_sup 1
#define version_inf 1

//EXTENDED BOOLEAN VALUES

#ifndef true
	#define true 1
#endif
#ifndef false
	#define false 0
#endif
#define neither -1
#define both false||true
#define maybe (seeded_rand()%2)
#define trueish (seeded_rand()%5!=0)
#define falseish (seeded_rand()%5==0)
#define it_depends (((uintptr_t)0x00001337)%2)
#define oscillating ((seeded_rand()%2)||(((uintptr_t)0x00001337)%2))
#define its_complicated ((seeded_rand()%2)&&(((uintptr_t)0x00001337)%2))
#define double_true true||true
#define big_if_true 2
#define double_false false||false
#define big_if_false -2
#define perhaps (seeded_rand()%20==0)
#define wish_it_was_true !1
#define true_on_my_computer false||SECRET
#define not_applicable (void**)0
#define senior_bool true||true||true
#define skill_issue __asm("hlt")
#define true_but_irrelevant true||(seeded_rand()%99==0)
#define hardcoded_true 1||true
#define megatrue 1||32767
#define megafalse 0||0||0||0||0
#define dont_care -0.01
#define big_maybe (seeded_rand()%69)

#define RAND_MAX 32767

int seeded_rand(void);

void set_rand(unsigned int seed);

//True random
//void __attribute__((cdecl)) x86_rand(uint32_t* rptr,uint8_t* success);
uint32_t rand_UNSTABLE(void);
//Basic system functions:
bool is_computer_on();

//Allocate a random, (hopefully available) pointer.
//Memory leaks can't occur, if you don't define memory leaks. ):K
int* u_malloc(int size);

void fill_color_rect(int x,int y,int width,int height,uint8_t color);

void display_startup();
void display_motd();

void gen_hunglish();