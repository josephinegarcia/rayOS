#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

/*
*	The I/O ports 
*	All the I/O ports are calculated relative to the data port. This is 
* 	because all serial ports (COM1, COM2, COM3, COM4) have their ports in
* 	the same order, but they  start at different values. 
*/ 

#define SERIAL_COM1_BASE	0x3F8	/* COM1 base port */
#define SERIAL_COM2_BASE	0x2F8	/* COM2 base port */
#define SERIAL_COM3_BASE	0x3E8	/* COM3 base port */ 
#define SERIAL_COM4_BASE	0x2E8	/* COM4 base port */ 

/** Function-like macros 
* #define indentifier( parameters ) replacement-list 
*/ 

#define SERIAL_DATA_PORT(base) 				(base)
#define SERIAL_FIFO_COMMAND_PORT(base)		(base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)		(base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base)		(base + 4)
#define SERIAL_LINE_STATUS_PORT(base)		(base + 5)

/* The I/O port commands */
/*
* 	SERIAL_LINE_ENABLE_DLAB: 
*	Tells the serial ort to expect first the highest 8 bits 
*	on the data port, then the lowest 8 bits will follow 
*/ 

#define SERIAL_LINE_ENABLE_DLAB		0x80 




/** initialize_serial_port:
*
*	Standard initialization for the a given port 
*
*	@param port - the serial port to initialize
*	@param divisor - the divisor used for the baud rate
*
*/

void initialize_serial_port(unsigned short, unsigned short);



/** serial_configure_baud_rate: 
*
*	Sets the speed of the data being sent. The default speed of a 
*	serial port is 115200 bits/s. The argument is a divisor of that 
* 	number, hence the resulting speed becomes (115200 / divisor) bits/s.
*
* 	@param com 	The COM port to configure 
* 	@param divisor	The divisor 
*
*/ 

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);

/** serial_configure_line: 
*	
*	Configures the line of the given serial port. The port is set to 
*	have a data length of 8 bits, no parity bits, one stop bit and 
*	break control disabled. 
*
*	@param com 	The serial port to configure
*
*	Bit: 		| 7 | 6 | 5 4 3 | 2 | 1 0 |
*	Content: 	| d | b | prty  | s | dl  |
*	Value : 	| 0 | 0 | 0 0 0 | 0 | 1 1 |
*
*/ 

void serial_configure_line(unsigned short com);


/** serial_received: 
*
*	checks whether a given port has received and data
*	
*	@param com - port to check 
*
*/

int serial_received(unsigned short);


/** read_serial: 
*
*	reads a character from a given serial port
*
*	@param com - port to read from 
*/

char read_serial(unsigned short);

/** serial_is_transmit_fifo_empty: 
*
*	Checks whether the tranmit FIFO queue is empty or not for the 
*	given COM port. 
*
*	@param  com The COM port 
*	@param  0 if the transmit FIFO queue is not empty 
*			1 if the transmit FIFO queue is empty
*/ 

int serial_is_transmit_fifo_empty(unsigned int);


/** write_serial: 
*
*	write a character to a given serial port
* 
*	@param com - port to write to 
*	@param c - character to write 
*/

void write_serial(unsigned short, char);

#endif /* SERIAL_H */