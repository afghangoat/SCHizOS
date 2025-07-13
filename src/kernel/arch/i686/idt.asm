[bits 32]

;void __attribute__((cdecl)) i686_IDT_Load(IDTDescriptor* idt_descriptor);
global i686_IDT_Load
i686_IDT_Load:
	;new call frame
	push ebp	;ebp-old call frame saved
	mov ebp,esp ;initialize new call frame
	
	;load idt
	mov eax, [ebp+8]
	lidt [eax]

	;restore ebp, the old call frame
	mov esp,ebp
	pop ebp
	ret
