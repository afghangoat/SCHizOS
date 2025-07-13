org 0x7C00 			;there, the bootsector is loaded by the BIOS
bits 16

;stack initialization
entry:
	mov ax,0
	mov dx,ax
	mov es,ax
	mov ss,ax
	mov sp,7C00h
	
	;PROTECTED MODE REQUIREMENTS:
	;1. Disable interrupts and NMI interrupts (hardware failure, watchdog error)
	cli
	;2. Enabled A20 gate
	call EnableA20
	
	;3. Load Global Descriptor Table
	call LoadGDT
	
	;4. Set enable protected mode flag in CR0
	mov eax,cr0
	or al,1
	mov cr0,eax
	
	;5. Perform a far jump into protected mode
	jmp dword 08h:.pmode

.pmode:
	[bits 32]
	;The is protected mode (32-bits)
	
	;Changed:
	;- All values in stack must be 32 bits wide
	;- No segmented offset scheme
	;- Flat memory mode
	;- Functions params and return values must be 32 bits wide
	;- Interrupts cause triple fold
	
	;6. setup segment registers
	mov ax,0x10
	mov ds,ax
	mov ss,ax
	
	
	;text can be drawn (at 16 colors) VGA interface
	mov esi, g_hello_world
	mov edi,ScreenBuffer
	cld
	
	mov bl,1

.loop:
	lodsb
	or al,al
	jz .done
	mov [edi],al
	inc edi
	
	mov [edi],bl ;0x2: green ,color byte, 0x47 is background color, upper bit data is upper color
	inc edi
	inc bl ;for colorful words
	jmp .loop

.done:
	;Now go back to real mode?? (if we want)
	jmp word 0x18h:.pmode16 ;Jump to 16 bit protected segment
	
.pmode16:
	[bits 16]
	;Load segment registers with 16 bit data segment
	mov eax, cr0
	and al, ~1
	mov cr0, eax
	
	;Jump to real mode
	jmp word 00h:.rmode

.rmode:
	;Setup segments
	mov ax,0
	mov ds,ax
	mov ss,ax
	
	;Reenable interrupts
	sti
	;Voila, done! (16 bit real mode again)

.halt:
	jmp .halt

EnableA20:
	[bits 16]
	call A20WaitInput
	;Disable keyboard
	mov al, KbdControllerDisableKeyboard
	out KbdControllerDataPort,al
	
	;Read control output port (flush buffer)
	call A20WaitInput
	mov al, KbdControllerReadCtrlOutputPort
	out KbdControllerCommandPort,al
	
	call, A20WaitOutput
	in al,KbdControllerDataPort ;call in al
	push eax
	
	;write control output port
	call A20WaitInput
	mov al,KbdControllerWriteCtrlOutputPort
	out KbdControllerCommandPort,al
	
	call A20WaitInput
	pop eax
	or al,2 					;bit 2 =A20 gate
	out KbdControllerDataPort,al
	
	;reenable keyboard
	call A20WaitInput
	mov al,KbdControllerEnableKeyboard
	out KbdControllerCommandPort, al
	
	call A20WaitInput
	ret

A20WaitInput:
	[bits 16]
	;Wait until input buffer is 0
	in al,KbdControllerCommandPort
	test al,2
	jnz A20WaitInput
	ret
A20WaitOutput:
	[bits 16]
	;Wait until status bit 1 is 1 so the output buffer can be read
	in al,KbdControllerCommandPort
	test al,1
	jz A20WaitOutput
	ret

LoadGDT:
	[bits 16]
	lgdt [global_GDT_descriptor]
	ret

KbdControllerDataPort equ 0x60
KbdControllerCommandPort equ 0x64
KbdControllerEnableKeyboard equ 0xAE
KbdControllerDisableKeyboard equ 0xAD
KbdControllerReadCtrlOutputPort equ 0xD0
KbdControllerWriteCtrlOutputPort equ 0xD1

ScreenBuffer equ 0xB8000

global_GDT:
	DQ 0;NULL descriptor
	
	;32bit code segment
	DW 0FFFFh 	;bits limit (32bit range)
	DW 0		;number base (bits 0-15)
	DB 0		;base (bits 16-23)
	DB 10011010b		;access flags (present,ring0,code segment,executable,direction,readable)
	DB 11001111b		;granularity (4Kb pages,32-bit protected mode) + limits (bits 16-19)
	DB 0				;base high
	
	;32bit data segment
	DW 0FFFFh 	;bits limit (32bit range)
	DW 0		;number base (bits 0-15)
	DB 0		;base (bits 16-23)
	DB 10010010b		;access flags (present,ring0,data segment,executable,direction 0,writeable)
	DB 11001111b		;granularity (4Kb pages,32-bit protected mode) + limits (bits 16-19)
	DB 0				;base high
	
	;16bit code segment
	DW 0FFFFh 	;bits limit (32bit range)
	DW 0		;number base (bits 0-15)
	DB 0		;base (bits 16-23)
	DB 10011010b		;access flags (present,ring0,code segment,executable,direction,readable)
	DB 00001111b		;granularity (1b pages,16-bit protected mode) + limits (bits 16-19)
	DB 0				;base high
	
	;16bit data segment
	DW 0FFFFh 	;bits limit (32bit range)
	DW 0		;number base (bits 0-15)
	DB 0		;base (bits 16-23)
	DB 10010010b		;access flags (present,ring0,data segment,executable,direction 0,writeable)
	DB 00001111b		;granularity (1b pages,16-bit protected mode) + limits (bits 16-19)
	DB 0				;base high

global_GDT_descriptor:
	DW global_GDT_descriptor - global_GDT - 1	;limit=size of GDT
	DD global_GDT								;Address of GDT

g_hello_world: DB "Hello World!",0

times 510-($-$$) DB 0	;pad file to become 512 bytes long
DW 0AA55h 				;required for the BIOS to load this

;bool test_A20(){
;	uint32_t* low_mem=(uint32_t*)0x500;
;	*low_mem = 0xC0DE1234 //Check later if this is written
;	uint32_t* high_mem=(uint32_t*)0x100500;
;	if(*high_mem==*low_mem){
;		return false; //A20 disabled
;	}
;	return true; //A20 enabled
;}
