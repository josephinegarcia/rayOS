; setting up entry point for linker
global _start

extern KERNEL_START
extern KERNEL_END
extern BSS_START
extern BSS_SIZE

extern EARLY_KERNEL_START
extern EARLY_KERNEL_END
extern EARLY_BSS_START
extern EARLY_BSS_SIZE

KERNEL_BASE equ 0xC0000000
LOWMEM_END 	equ EARLY_KERNEL_END
PAGE_SIZE 	equ 4096
PAGE_SHIFT 	equ 12 ; 2^12 = 4096 = PAGE_SIZE
PAGE_PERM 	equ 3  ; default page permissions: present, read/write
STACK_SIZE  equ 4*PAGE_SIZE ; initial kernel stack space of 16k

;Global MultiBoot Kernel Recongnzation
MODULEALIGN equ 1<<0
MEMINFO 	equ 1<<1
FLAGS 		equ MODULEALIGN | MEMINFO
MAGIC 		equ 0x1BADB002
CHECKSUM 	equ -(MAGIC + FLAGS)


;Putting in object file
[section .multiboot_header]
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

[section .early_text progbits]
_start:
	;Save multiboot info from grub before clobbering registers
	;Stack will be zeroed out so we can't save it on stack
	mov [multiboot_magic], eax
	mov [multiboot_info],  ebx

	;zero early BSS
	mov ecx, EARLY_BSS_SIZE
	xor al, al
	mov edi, EARLY_BSS_START
	rep stosb ;instruction to copy data from AL (for bytes STOSB) to a destination string pointed by ES:DI in memory

	; identity map from 0x0000000 -> EARLY_KERNEL_END
	; code assumes that kernel won't be greater than 3MB
	mov eax, lowmem_pt
	mov [page_directory], eax
	or dword [page_directory], PAGE_PERM ;mark the PT as present

	xor eax, eax ;eax is our starting physical address (0x0000000)
.lowmem:
	mov ecx, eax
	shr ecx, PAGE_SHIFT ;divide our current address by PAGE_SIZE
	and ecx, 0x3ff		;mask of higher bits to create an index (mod 1024)
	mov [lowmem_pt + ecx*4], eax ;copy our physical address to page entry
	or dword [lowmem_pt + ecx * 4], PAGE_PERM ;write our permissions (present, etc)
	add eax, PAGE_SIZE ; move on to next page
	cmp eax, LOWMEM_END ; are we done with lowmem
	jl .lowmem

	;create virtual mappings for the kernel in higher half
	mov edx, KERNEL_BASE
	shr edx, 22		;find which page table we need to use

	mov eax, kernel_pt
	mov [page_directory + edx * 4], eax
	or dword [page_directory + edx * 4], PAGE_PERM ; mark PT as present
	
	mov eax, KERNEL_START ;kernel current virtual start
.higher:
	mov ecx, eax
	shr ecx, PAGE_SHIFT
	and ecx, 0x3ff ; generate kernel PTE index
	
	mov ebx, eax
	sub ebx, KERNEL_BASE ; convert virt->physical mem
	mov [kernel_pt+ecx*4], ebx
	or dword [kernel_pt+ecx*4], PAGE_PERM

	add eax, PAGE_SIZE ; move to next page
	cmp eax, KERNEL_END ; done mapping kernel?
	jl .higher

	mov eax, page_directory
	mov cr3, eax ; load CR3 with our page directory

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax ; enable paging make sure next instruction fetch doesnt page fault!

	;zero kernel bss
	mov ecx, BSS_SIZE
	xor al, al
	mov edi, BSS_START
	rep stosb
	
	;adjust stack into virtual area
	;setup and adjust stack
	mov esp, stack + STACK_SIZE

	extern KERNEL_SIZE
	push KERNEL_SIZE
	push dword [multiboot_magic] ; multiboot magic number
	push dword [multiboot_info] ; multiboot info
	extern kernel_main
	call kernel_main
	
.eof:
	hlt 		  ;halt machine if kernel returns
	jmp short .eof

[section .early_bss nobits]
alignb 4096 ;page align the page directory and tables
page_directory: ;this should be alinged as well
	resd 1024	;reserve 1024 DWORDS for our page table pointers
lowmem_pt:
	resd 1024 	;lowmem identity mapping
kernel_pt:
	resd 1024	;kernel page table mappings

[section .early_data]
multiboot_magic:
	dd 0
multiboot_info:
	dd 0

;bss is in the higher-half
[section .bss]
align 4
stack:
	resb STACK_SIZE ; 16k stack on DWORD boundary