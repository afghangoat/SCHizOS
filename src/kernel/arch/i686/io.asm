;We are in 32 bit mode in kernel
global x86_outb
x86_outb:
    [bits 32]
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al ;places al out
    ret

global x86_inb
x86_inb:
    [bits 32]
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx ;places dx in al
    ret

global i686_panic
i686_panic:
	cli
	hlt

global crash_me
crash_me:
	mov eax,0
	div eax
	ret

;uint8_t __attribute__((cdecl)) i686_enable_interrupts();
global i686_enable_interrupts
i686_enable_interrupts:
	sti
	ret
	
;uint8_t __attribute__((cdecl)) i686_disable_interrupts();
global i686_disable_interrupts
i686_disable_interrupts:
	cli
	ret

global flush_irq
flush_irq:
	mov al, 0x20
	out 0x20, al
	ret

global start_pic
start_pic:
	in al, 0x21        ; Read current PIC mask
	and al, 0xFD       ; Clear bit 1 (unmask IRQ 1)
	out 0x21, al       ; Write back to PIC