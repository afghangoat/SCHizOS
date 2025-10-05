#include "stdlib.h"

static unsigned long int next = 1;  // NB: "unsigned long int" is assumed to be 32 bits wide
bool rnd_unsafe=true;
#define supress_error_rand 1

int seeded_rand(void){ // maximum random value assumed to be RAND_MAX
	if(rnd_unsafe==true&&supress_error_rand==false){
		printf("Warning: Seeded random has no new seed other than default seed. This may make the random generation predictable.\r\n");
	}
    next = next * 1103515245 + 78523;
    return (unsigned int) (next / 65536) % (RAND_MAX+1);
}

void set_rand(unsigned int seed){
    next = seed;
	rnd_unsafe=false;
}

//True random
//void __attribute__((cdecl)) x86_rand(uint32_t* rptr,uint8_t* success);
uint32_t rand_UNSTABLE(void){
	uint32_t a=0;
	uint8_t success=0;
	
	success=x86_rand(&a);
	if(success==0){
		printf("Warning: Generating true random value failed!");
	}
	return a;
}
//Basic system functions:
bool is_computer_on(){
	return true;
}

//Allocate a random, (hopefully available) pointer.
//Memory leaks can't occur, if you don't define memory leaks. ):K
//Assign RAM Randomly, as it was intended by God.
int* u_malloc(int size){
    long int index=MEMORY_MIN+seeded_rand()%MEMORY_MAX;
    int* ptr=(int*) index;
    int* tptr=ptr;
    for(int i=0;i<size;i++){
        tptr=NULL;
        tptr++;
    }
    return ptr;
}

//static uintptr_t next_free = MEMORY_MIN; //REDUNDANT

typedef struct MemBlock {
    uint32_t size;
    struct MemBlock* next;
} MemBlock;

static MemBlock* free_list = (MemBlock*)MEMORY_MIN;

static void k_malloc_init() {
    free_list->size = MEMORY_MAX - MEMORY_MIN - sizeof(MemBlock);
    free_list->next = NULL;
}

void* k_malloc(uint32_t size) {
    
    size = (size + 3) & ~3; //Align to 4B

    MemBlock* prev = NULL;
    MemBlock* curr = free_list;

    while (curr) { //traverse list
        if (curr->size >= size) {
            if (curr->size > size + sizeof(MemBlock)) {
				
                MemBlock* new_block = (MemBlock*)((uint8_t*)curr + sizeof(MemBlock) + size);
                new_block->size = curr->size - size - sizeof(MemBlock);
                new_block->next = curr->next;

                curr->size = size;
                curr->next = NULL;

                if (prev){
                    prev->next = new_block;
                }else{
                    free_list = new_block;
				}
				
            } else {
				
                //near-exact fit
                if (prev){
                    prev->next = curr->next;
                }else{
                    free_list = curr->next;
				}
                curr->next = NULL;
            }

            return (void*)((uint8_t*)curr + sizeof(MemBlock));
        }

        prev = curr;
        curr = curr->next;
    }

	//TODO outofmemory error
    return NULL; 
}

void k_free(void* ptr) {
    if (!ptr){ //NULL or nullptr
		return;
	}

    MemBlock* block = (MemBlock*)((uint8_t*)ptr - sizeof(MemBlock));
    block->next = free_list;
    free_list = block;
}

#define CANVAS_WIDTH 80
#define CANVAS_HEIGHT 25
void fill_color_rect(int x,int y,int width,int height,uint8_t color){
	for(int i=0;i<width;i++){
		for(int j=0;j<height;j++){
			putcolor(x+i,y+j,color);
		}
	}
}

const int motd_amount=5;
const char* motds[]={
	"Ha egy sorozat korla'tos e's monoton akkor konvergens.",
	"Ha a(n)->ve'gtelen e's n>=N esete'n b(n)>a(n) akkor b(n)->ve'gtelen.",
	"Ha a(n)->A, b(n)->A e's n>=N esete'n b(n)>c(n)>a(n) akkor c(n)->A.",
	"Nagysa'grendek: n^b << b^n << n! << n^n (b valo's)",
	"A hatarertek szamitasanal falhasznaltuk, hogy ha f derivalhato egy pontban, akkor ott folytonos is."
};
void display_startup(){
	clrscr();
	setcursor(0,0);
	fill_color_rect(0,0,CANVAS_WIDTH,7,80);
	fill_color_rect(0,0,42,6,60);
	//fill_color_rect(0,7,42,1,20);
	printf("	         _     _      ____   _____ \n");
	printf("          | |   (_)    / __ \\ / ____|\n");
	printf("  ___  ___| |__  _ ___| |  | | (___  \n");
	printf(" / __|/ __| '_ \\| |_  / |  | |\\___ \\ \n");
	printf(" \\__ \\ (__| | | | |/ /| |__| |____) |\n");
	printf(" |___/\\___|_| |_|_/___|\\____/|_____/ \n");
	printf(OS_NAME " %d.%d reporting for duty!\n",version_sup,version_inf);
	setcursor(0,8);
	
}
void display_motd(){
	int ax=0;
	int ay=0;
	getcursor(&ax,&ay);
	setcursor(0,ay);
	fill_color_rect(0,ay,CANVAS_WIDTH,3,26);
	//fill_color_rect(0,1,50,1,16);
	//fill_color_rect(0,2,50,1,20);
	printf("------------------------------Message of the day:------------------------------\n%s\n-------------------------------------------------------------------------------\n",motds[seeded_rand()%motd_amount]);
}

/* PORTED FROM JS
const hu_weaks=['m','c','k','f','g','h','j','l','n','p','r','s','v','gy','rcs'];
const hu_strongs=['t','n','g','d','b','k','m','h','gy','cs','ty','dzs'];
const hu_abts=['a','e','i','o','Ăś','u','ĂŠ','ĂĄ',];
function hungarian(tags=2){
    let word="";
    for(let i=0;i<tags;i++){
        word=word+hu_strongs[getRandomInt(hu_strongs.length)]+hu_abts[getRandomInt(hu_abts.length)]+hu_weaks[getRandomInt(hu_weaks.length)];
    }
    endcode.value=endcode.value+" "+word;
}
*/
const char* hu_weaks[]={"m","c","k","f","g","h","j","l","n","p","r","s","v","gy","rcs"};
#define SIZE_hu_weaks 15
const char* hu_strongs[]={"t","n","g","d","b","k","m","h","gy","cs","ty","dzs"};
#define SIZE_hu_strongs 12
const char* hu_abts[]={"a","e","i","o","u","u'","'u'","'o'"};
#define SIZE_hu_abts 8

#define MAX_TAGS_COUNT 4
void gen_hunglish(){
	int tags=seeded_rand()%MAX_TAGS_COUNT+1;
	for(int i=0;i<tags;i++){
		printf("%s%s%s",hu_strongs[seeded_rand()%SIZE_hu_strongs],hu_abts[seeded_rand()%SIZE_hu_abts],hu_weaks[seeded_rand()%SIZE_hu_weaks]);
    }
	//seeded_rand();
}
