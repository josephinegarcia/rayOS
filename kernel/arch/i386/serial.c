#include <kernel/serial.h>
#include <stdio.h>

void initialize_serial_port(unsigned short com, unsigned short divisor)
{
	serial_configure_baud_rate(com, divisor);
	serial_configure_line(com);
}

void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
	outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
	outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
	outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

void serial_configure_line(unsigned short com)
{
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}


int serial_received(unsigned short com)
{
	return SERIAL_LINE_STATUS_PORT(com) & 1;
}


char read_serial(unsigned short com)
{
	while(serial_received(com) == 0);
	return inb(SERIAL_DATA_PORT(com));
}


int serial_is_transmit_fifo_empty(unsigned int com)
{
	/* 0x20 = 0010 0000 */
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}


void write_serial(unsigned short com, char c)
{
	while(serial_is_transmit_fifo_empty(com) == 0);
	outb(SERIAL_DATA_PORT(com), c);
}