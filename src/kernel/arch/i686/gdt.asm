[bits 32]

;void __attribute__((cdecl)) i686_GDT_Load(GDTDescriptor* descriptor,uint16_t code_seg,uint16_t data_seg);
global i686_GDT_Load
i686_GDT_Load:
	;new call frame
	push ebp	;ebp-old call frame saved
	mov ebp,esp ;initialize new call frame
	
	;load gdt
	mov eax, [ebp+8]
	lgdt [eax]
	
	;reload code segment
	mov eax, [ebp+12]
	push eax
	push .reload_cs
	retf ;return far
	
.reload_cs:
	
	;reload data segs
	mov ax, [ebp+16]
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	
	
	;restore ebp, the old call frame
	mov esp,ebp
	pop ebp
	ret