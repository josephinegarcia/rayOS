#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/multiboot.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

int kernel_main(multiboot_info_t* mbd, unsigned int magic){
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	keyboard_install();
	terminal_initialize();

	if(MULTIBOOT_BOOTLOADER_MAGIC != magic)
		printf ("Invalid magic number: 0x%x\n", (unsigned) magic);

	 /* Are mem_* valid? */
  	if (CHECK_FLAG (mbd->flags, 0))
    	printf ("mem_lower = %uKB, mem_upper = %uKB\n", (unsigned) mbd->mem_lower, (unsigned) mbd->mem_upper);

	printf("MADE IT!");

	return 0;
}