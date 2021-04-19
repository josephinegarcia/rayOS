#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
int kernel_main() {
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	keyboard_install();
	terminal_initialize();
	// printf("Hello WORLD!!!");
	return 0;
}