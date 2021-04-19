#include <stdio.h>

#ifndef _KERNEL_KEYBRD_H
#define _KERNEL_KEYBRD_H

#define KBD_DATA_PORT   0x60

/* Handles the keyboard interrupt */
void keyboard_handler(void *r);

/** read_scan_code:
    *  Reads a scan code from the keyboard
    *
    *  @return The scan code (NOT an ASCII character!)
    */
unsigned char read_scan_code(void);

#endif