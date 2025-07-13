[bits 32]

extern i686_ISR_Handler
;register interrupts

%macro ISR_NOERROR 1
	global i686_ISR%1:
	i686_ISR%1:
		push 0	;push dummy error code
		push %1	;push interrupt number
		jmp isr_common

%endmacro

%macro ISR_ERRORCODE 1
global i686_ISR%1:
	i686_ISR%1:
		;8: double fault (error)
		push %1	;push interrupt number
		jmp isr_common

%endmacro

%include "arch/i686/isrs_gen.inc"


isr_common:
	;the processor pushes, the stack pointer, code segment, stack segment, data segment
	pusha ;save all
	
	xor eax,eax ;push ds
	mov ax,ds
	push eax
	
	mov ax,0x10 ;kernel data segment
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	
	push esp ;pass pointer to stack to C, so all pushed data can be accessed
	call i686_ISR_Handler
	add esp,4 ;cleanup
	
	pop eax ;restore old segment
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	
	popa ;restore general segment registers
	add esp, 8 ;remove error code
	iret ;will pop: code seg,eip,eflags,ss,esp
	