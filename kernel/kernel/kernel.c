#include <stdio.h>

#include <kernel/tty.h>
#include <drivers/serial.h>

int intToASCII(int num){
	return '0'+num;
}

int kernel_main() {
	terminal_initialize();
	initialize_serial_port(SERIAL_COM1_BASE, 2);
	//char *address = "0xFF8564089045AAA";
	//unsigned int value = 12;
	//while (*address != '\0'){
	//	write_serial(SERIAL_COM1_BASE, *address);
	//	*address++;
	//}
	
	// char c = intToASCII(12);
	// write_serial(SERIAL_COM1_BASE, c);

	//write_serial(SERIAL_COM1_BASE, '345');
	//printf("%u",123);
	terminal_setCursorPos(5,5);
	printf("HELLO");
	return 0;

}
