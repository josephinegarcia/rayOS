global gdt_flush

extern gp				;The pointer to our GDT

gdt_flush:
	lgdt [gp]
	mov ax, 0x10		; 0x10 is the offset in the GDT to the data segment
	mov ds, ax 			; Load all data segment selectors
	mov es, ax
	mov fs, ax 
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2		; 0x08 is the offset of the code segemnt

flush2:
	ret