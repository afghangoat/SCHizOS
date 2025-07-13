;org 0x7C00

%macro x86_EnterRealMode 0
	[bits 32]
	jmp dword 18h:.pmode16

.pmode16:
	[bits 16]
	;Load segment registers with 16 bit data segment
	mov eax, cr0
	and al, ~1
	mov cr0, eax
	
	;Jump to real mode
	jmp dword 00h:.rmode

.rmode:
	;Setup segments
	mov ax,0
	mov ds,ax
	mov ss,ax
	
	;Reenable interrupts
	sti
	;Voila, done! (16 bit real mode again)

%endmacro

%macro x86_EnterProtectedMode 0
	cli
	
	mov eax,cr0 ;set protected move flag, enable it
	mov al,1
	mov cr0,eax
	
	jmp dword 08h:.pmode
	
.pmode:
	;Now in protected mode
	[bits 32]
	
	;Initialize stack segment registers with 0x10
	mov ax,0x10
	mov ds,ax
	mov ss,ax
%endmacro

%macro LinearToSegOffset 4
	mov %3, %1 ;Access linear address, move it to eax
	shr %3, 4
	mov %2, %4
	
	mov %3, %1 ;linear address to eax
	and %3, 0xf
%endmacro

;bool _cdecl x86_Disk_GetDriveParams(uint8_t drive,uint8_t* driveTypeOut,uint16_t* cylindersOut,uint16_t* sectorsOut,uint16_t* headsOut);
global x86dgdp
x86dgdp:
    [bits 32]

    ; make new call frame
    push ebp             ; save old call frame
    mov ebp, esp         ; initialize new call frame

    x86_EnterRealMode

    [bits 16]

    ; save regs
    push es
    push bx
    push esi
    push di

    ; call int13h
    mov dl, [bp + 8]    ; dl - disk drive
    mov ah, 08h
    mov di, 0           ; es:di - 0000:0000
    mov es, di
    stc
    int 13h

    ; out params
    mov eax, 1
    sbb eax, 0

    ; drive type from bl
    LinearToSegOffset [bp + 12], es, esi, si
    mov [es:si], bl

    ; cylinders
    mov bl, ch          ; cylinders - lower bits in ch
    mov bh, cl          ; cylinders - upper bits in cl (6-7)
    shr bh, 6
    inc bx

    LinearToSegOffset [bp + 16], es, esi, si
    mov [es:si], bx

    ; sectors
    xor ch, ch          ; sectors - lower 5 bits in cl
    and cl, 3Fh
    
    LinearToSegOffset [bp + 20], es, esi, si
    mov [es:si], cx

    ; heads
    mov cl, dh          ; heads - dh
    inc cx

    LinearToSegOffset [bp + 24], es, esi, si
    mov [es:si], cx

    ; restore regs
    pop di
    pop esi
    pop bx
    pop es

    ; return

    push eax

    x86_EnterProtectedMode

    [bits 32]

    pop eax

    ; restore old call frame
    mov esp, ebp
    pop ebp
    ret

;bool _cdecl x86_Disk_Reset(uint8_t drive);
global x86dreset
x86dreset:
    [bits 32]

    ; make new call frame
    push ebp             ; save old call frame
    mov ebp, esp          ; initialize new call frame


    x86_EnterRealMode

    mov ah, 0
    mov dl, [bp + 8]    ; dl - drive
    stc
    int 13h

    mov eax, 1
    sbb eax, 0           ; 1 on success, 0 on fail   

    push eax

    x86_EnterProtectedMode

    pop eax

    ; restore old call frame
    mov esp, ebp
    pop ebp
    ret

;
;bool _cdecl x86_Disk_Read(uint8_t drive,uint16_t cylinder,uint16_t sector,uint16_t head,uint8_t count,void far * dataOut);
global x86dread
x86dread:

    ; make new call frame
    push ebp             ; save old call frame
    mov ebp, esp          ; initialize new call frame

    x86_EnterRealMode

    ; save modified regs
    push ebx
    push es

    ; setup args
    mov dl, [bp + 8]    ; dl - drive

    mov ch, [bp + 12]    ; ch - cylinder (lower 8 bits)
    mov cl, [bp + 13]    ; cl - cylinder to bits 6-7
    shl cl, 6
    
    mov al, [bp + 16]    ; cl - sector to bits 0-5
    and al, 3Fh
    or cl, al

    mov dh, [bp + 20]   ; dh - head

    mov al, [bp + 24]   ; al - count

    LinearToSegOffset [bp + 28], es, ebx, bx

    ; call int13h
    mov ah, 02h
    stc
    int 13h

    ; set return value
    mov eax, 1
    sbb eax, 0           ; 1 on success, 0 on fail   

    ; restore regs
    pop es
    pop ebx

    push eax

    x86_EnterProtectedMode

    pop eax

    ; restore old call frame
    mov esp, ebp
    pop ebp
    ret

;section .text:
;    org 0x7C00
