#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
#ifdef __cplusplus
}
#endif

#endif
