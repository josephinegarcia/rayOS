#include <drivers/keyboard/keyboard.h>
#include <drivers/io.h>

void kb_init(void){
    outb(0x21, 0xFD);
}