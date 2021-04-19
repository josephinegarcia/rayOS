#include <stdio.h>

#include <kernel/tty.h>

int kernel_main() {
	terminal_initialize();
	printf("Hello World!");
	terminal_setCursorPos(5,5);
	return 0;

}
