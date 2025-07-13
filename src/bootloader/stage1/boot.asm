org 0x7C00
bits 16
;qemu-system-i386 -fda build/main_floppy.img

;MACROS:
%define ENDL 0x0D, 0x0A

;FAT12 header
jmp short start
nop

bdb_oem: DB 'MSWIN4.1' ;8 bytes
bdb_bytes_per_sector: 		DW 512
bdb_sectors_per_cluster: 	DB 1
bdb_reserved_sectors: 		DW 1
bdb_fat_count:  			DB 2
bdb_dir_entries_count:		DW 0E0h
bdb_total_sectors: 			DW 2880 ;From makefile: 2880*512=1.44MB
bdb_media_descriptor_type: 	DB 0F0h ;F0=3.5" floppy disk (????? WDYM)
bdb_sectors_per_fat:		DW 9	;9 sectors
bdb_sectors_per_track:		DW 18
bdb_heads:DW 2
bdb_hidden_sectors:			DD 0
bdb_large_sector_count: 	DD 0

;#extended boot sector
ebr_drive_number:			DB 0  	;0: floppy, 0x80: hdd
		DB 0	;reserved
ebr_signature:				DB 29h
ebr_volume_id:				DB 12h,34h,56h,78h ;serial number
ebr_volume_label:			DB 'SCHIZOS    ' ;11 bytes, os name
ebr_system_id:				DB 'FAT12   ' ;8 bytes

start:
	
	mov ax,0 ;Data->Regiser->Anything remotely useful
	mov ds,ax
	mov es, ax
	
	;stack init
	mov ss,ax
	mov sp, 0x7C00 				;Stack need to be offset from org, grows downwards, points to start of OS| mov sp, 0x7BFF for safe
	
	push es
	push word .after
	retf

.after:
	;read from floppy disk
	;set DL number
	mov [ebr_drive_number], dl
	;mov ax,1
	;mov cl,1
	;mov bx,0x7E00
	;call disk_read
	
	;Write loading...
	mov si,msg_loading
	call puts
	
	;Read drive params
	push es
	mov ah,08h
	int 13h
	jc floppy_error
	pop es
	
	and cl, 0x3F;rem 2 top bits
	xor ch,ch
	mov [bdb_sectors_per_track],cx 	;sector count
	
	inc dh
	mov [bdb_heads],dh 				;load bdb params, heads, count
	
	mov ax, [bdb_sectors_per_fat]	; LBA of root dir=reserved+fats*sectorsperfat
	mov bl, [bdb_fat_count]
	xor bh,bh
	mul bx
	add ax,[bdb_reserved_sectors]
	push ax
	
	;get size of root dir (32*num_of_entries/bytes_per_sector)
	mov ax,[bdb_sectors_per_fat]
	shl ax,5
	xor dx,dx 	;dx=0
	div word [bdb_bytes_per_sector] ;num of sectors
	
	test dx,dx 	;if dx!= 0 inc 1
	jz .root_dir_after
	inc ax 		;add 1

.root_dir_after:

	;read root dir:
	mov cl, al 	;cl=num of sectors to read
	pop ax 		;ax=LBA
	mov dl, [ebr_drive_number]
	mov bx, buffer ;es:bx=buffer
	call disk_read
	
	;search for stage2.bin
	xor bx,bx
	mov di, buffer
.search_kernel:
	mov si, file_kernel_bin
	mov cx, 11 	;size of kernel bin string
	push di
	repe cmpsb 	;compare string bytes, si length and cx, repe=repeat while equal
	pop di
	je .found_stage2 				;jump if equal
	add di,32
	inc bx
	cmp bx, [bdb_dir_entries_count]
	jl .search_kernel
	
	jmp kernel_not_found

.found_stage2:
	;di=address to entry
	mov ax, [di+26] 				; First cluster
	mov [stage2_cluster],ax
	
	;load FAT into memory
	mov ax, [bdb_reserved_sectors]
	mov bx,buffer
	mov cl, [bdb_sectors_per_fat]
	mov dl, [ebr_drive_number]
	call disk_read
	
	;read kernel and process FileAllocationTable chain
	mov bx, STAGE2_LOAD_SEGMENT
	mov es, bx
	mov bx, STAGE2_LOAD_OFFSET

.load_kernel_loop:
	mov ax, [stage2_cluster]
	add ax,31 	;first cluster, read all, hardcoded size
	mov cl,1
	mov dl, [ebr_drive_number]
	call disk_read
	add bx,[bdb_bytes_per_sector]
	
	;get location of next cluster
	mov ax, [stage2_cluster]
	mov cx,3
	mul cx
	mov cx,2
	div cx 		; ax=index of FAT entry dx=cluster%2
	
	mov si, buffer
	add si,ax
	mov ax, [ds:si] 				;read entry from fat at ax index
	
	or dx,dx
	jz .even

.odd:
	shr ax,4
	jmp .next_cluster_after

.even:
	and ax, 0x0FFF
	jmp .next_cluster_after

.next_cluster_after:
	cmp ax,0x0FF8 ;end of chain
	jae .read_finish
	mov [stage2_cluster],ax
	jmp .load_kernel_loop

.read_finish:

	;jump out from kernel
	mov dl, [ebr_drive_number]
	;set segmental registers
	mov ax, STAGE2_LOAD_SEGMENT
	mov ds, ax
	mov es, ax
	jmp STAGE2_LOAD_SEGMENT:STAGE2_LOAD_OFFSET
	
	jmp wait_key_and_reboot 		; should never happen
	cli
	hlt

;In case of error
floppy_error:
	mov si,msg_readfail
	call puts
	jmp wait_key_and_reboot
	
kernel_not_found:
	mov si,msg_stage2_not_found
	call puts
	jmp wait_key_and_reboot
	
wait_key_and_reboot:
	mov ah,0
	int 16h 	;wait for keypress
	jmp 0FFFFh:0 ;jump the beginning of BIOS, rebooting...
	
	
.halt:
	cli 		;No interrupts can be called, to prevent leaving halt
	hlt
;Receives a string and prints it out, (ds:si pointer)
puts:
	push si
	push ax
	push bx
	
.loop:
	lodsb 		;loads next char
	or al,al    ;Verify if next is null? 0or0 is 0, anything else is 1
	jz .done
	
	mov ah, 0x0E
	mov bh,0
	int 0x10 	;Interrupts to write
	
	jmp .loop

.done:
	;jump outside loop
	pop bx
	pop ax
	pop si
	ret

;
; Disk layout/routines:
; Rings, slices=sectors
; each side has a head
; blocks on the disks has ids (indexes, as one might say)



;
; LBA to CHS conversion (LogicalBlockAddress)
; sectors per track/cylinder (on side)
; heads per cylinder
; sector= (LBA%sectors per track)+1
; head=(LBA/sectors per track)%heads
; cylinder=(LBA/sectors per track)/heads
;
;Params: ax:lba_address, returns:
; cx[0:5] sector number
; cx[6:15] cylinder number
; dh: head number
lba_to_chs:

	push ax
	push dx 		; push register values to stack
	
	xor dx,dx  ;dx=0
	div word [bdb_sectors_per_track] 	;ax=LBA/sectors per track
	
	inc dx 			;dx=lba%(sectors per track+1)
	mov cx,dx 		; sector
	
	xor dx,dx  		;dx=0
	div word [bdb_heads]
	
	mov dh,dl 		;head
	mov ch,al 		;cylinder: lower 8 bits
	shl ah,6
	or cl,ah 		;cylinder: higher 2 bits
	
	pop ax
	mov dl, al
	pop ax
	
	ret

disk_read:

	push ax 		;save need-to modified registers
	push bx
	push cx
	push dx
	push di

	push cx			;number of sectors to read to stack
	call lba_to_chs
	pop ax
	
	mov ah,02h
	mov di, 3		;retry count

.retry:
	pusha 			;save all registers
	stc 			;set carry flag
	
	int 13h 		;we read so : disk IO
	jnc .done 		;if no carry: done
	
	;fail
	popa
	call disk_reset
	
	dec di
	test di, di
	jnz .retry

.fail:
	jmp floppy_error
.done:
	popa
	
	pop di 			;restore back used registers
	pop dx
	pop cx
	pop bx
	pop ax
	ret

;dr:drive number
disk_reset:
	pusha
	mov ah,0
	stc
	int 13h
	jc floppy_error
	popa
	ret

;DB NEEDED FOR CONSTANTS
msg_loading: 				DB 'Loading...',ENDL,0 ;0 is /0
;ERRORS
msg_readfail:				DB 'File/r failed!',ENDL,0
msg_stage2_not_found:		DB 'Stage2 not found!',ENDL,0
;GENERAL CONSTANTS
file_kernel_bin: 			DB 'STAGE2  BIN'
stage2_cluster: 			DW 0

STAGE2_LOAD_SEGMENT			equ 0x0
STAGE2_LOAD_OFFSET			equ 0x500
;PADDING
times 510-($-$$) DB 0
DW 0AA55h ;Ensure padding

;BUFFER
buffer: