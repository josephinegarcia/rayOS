#ifndef INCLUDE_IDT_H
#define INCLUDE_IDT_H

#include <stdint.h>
#include <drivers/io.h>

struct IDT_entry{
    uint16_t base_low;
    uint16_t selector;          
    uint8_t zero;
    uint8_t type_attr;
    uint16_t base_high;
}__attribute__((packed));
// base is a 32 bit value that represents the address of the entry point of the ISR

// struct IDT_ptr{
//     uint16_t limit;
//     uint32_t base;
// }__attribute__((packed));

struct IDT_entry IDT[256];
//struct IDT_ptr idtp;

void idt_set_gate(uint32_t num, uint32_t base, uint16_t sel, uint8_t flags){
    IDT[num].base_low = base & 0xFFFF;
    IDT[num].selector = sel;
    IDT[num].zero = 0;
    IDT[num].type_attr = flags;
    IDT[num].base_high = (base >> 16) & 0xFFFF;
}

void idt_init(){
    // idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    // idtp.base = (uint32_t)&IDT;

    //memset(&idt, 0, sizeof(struct idt_entry) * 256);


    extern int32_t load_idt();
    extern int32_t irq0();
    extern int32_t irq1();
    extern int32_t irq2();
    extern int32_t irq3();
    extern int32_t irq4();
    extern int32_t irq5();
    extern int32_t irq6();
    extern int32_t irq7();
    extern int32_t irq8();
    extern int32_t irq9();
    extern int32_t irq10();
    extern int32_t irq11();
    extern int32_t irq12();
    extern int32_t irq13();
    extern int32_t irq14();
    extern int32_t irq15();

    uint32_t irq0_address;
    uint32_t irq1_address;
    uint32_t irq2_address;
    uint32_t irq3_address;          
    uint32_t irq4_address; 
    uint32_t irq5_address;
    uint32_t irq6_address;
    uint32_t irq7_address;
    uint32_t irq8_address;
    uint32_t irq9_address;          
    uint32_t irq10_address;
    uint32_t irq11_address;
    uint32_t irq12_address;
    uint32_t irq13_address;
    uint32_t irq14_address;          
    uint32_t irq15_address;         
	uint32_t idt_address;
	uint32_t idt_ptr[2];

    /*        Ports                 */
    /*              PIC1    PIC2    */
    /*Command       0x20    0xA0    */
    /*Data          0x21    0xA1    */

    /* ICW1 - begin initialization */  
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    /* ICW2 - remap offset address of IDT */
    /* In x86 protected mode, we have to remap the PICs  */
    /* beyond 0x20 b/c Intel has designated the first 32 */
    /* interrupts as "reserved" for cpu exceptions       */
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    /*ICW4 - environment info */
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /* ICW3 - setup cascading */
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    irq0_address = (unsigned)irq0;
    idt_set_gate(32, irq0_address, 0x08, 0x8E);
    irq1_address = (unsigned)irq1;
    idt_set_gate(33, irq0_address, 0x08, 0x8E);
    irq2_address = (unsigned)irq2;
    idt_set_gate(34, irq0_address, 0x08, 0x8E);
    irq3_address = (unsigned)irq3;
    idt_set_gate(35, irq0_address, 0x08, 0x8E);
    irq4_address = (unsigned)irq4;
    idt_set_gate(36, irq0_address, 0x08, 0x8E);
    irq5_address = (unsigned)irq5;
    idt_set_gate(37, irq0_address, 0x08, 0x8E);
    irq6_address = (unsigned)irq6;
    idt_set_gate(38, irq0_address, 0x08, 0x8E);
    irq7_address = (unsigned)irq7;
    idt_set_gate(39, irq0_address, 0x08, 0x8E);
    irq8_address = (unsigned)irq8;
    idt_set_gate(40, irq0_address, 0x08, 0x8E);
    irq9_address = (unsigned)irq9;
    idt_set_gate(41, irq0_address, 0x08, 0x8E);
    irq10_address = (unsigned)irq10;
    idt_set_gate(42, irq0_address, 0x08, 0x8E);
    irq11_address = (unsigned)irq11;
    idt_set_gate(43, irq0_address, 0x08, 0x8E);
    irq12_address = (unsigned)irq12;
    idt_set_gate(44, irq0_address, 0x08, 0x8E);
    irq13_address = (unsigned)irq13;
    idt_set_gate(45, irq0_address, 0x08, 0x8E);
    irq14_address = (unsigned)irq14;
    idt_set_gate(46, irq0_address, 0x08, 0x8E);
    irq15_address = (unsigned)irq15;
    idt_set_gate(47, irq0_address, 0x08, 0x8E);

    idt_address = (unsigned) IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * 256) + ((idt_address & 0xFFFF) << 16);
    idt_ptr[1] = idt_address >> 16;

    load_idt(idt_ptr);
    
}

void irq0_handler(void) {
          outb(0x20, 0x20); //EOI
}
 
void irq1_handler(void) {
	  outb(0x20, 0x20); //EOI
}
 
void irq2_handler(void) {
          outb(0x20, 0x20); //EOI
}
 
void irq3_handler(void) {
          outb(0x20, 0x20); //EOI
}
 
void irq4_handler(void) {
          outb(0x20, 0x20); //EOI
}
 
void irq5_handler(void) {
          outb(0x20, 0x20); //EOI
}
 
void irq6_handler(void) {
          outb(0x20, 0x20); //EOI
}
 
void irq7_handler(void) {
          outb(0x20, 0x20); //EOI
}
 
void irq8_handler(void) {
          outb(0xA0, 0x20);
          outb(0x20, 0x20); //EOI          
}
 
void irq9_handler(void) {
          outb(0xA0, 0x20);
          outb(0x20, 0x20); //EOI
}
 
void irq10_handler(void) {
          outb(0xA0, 0x20);
          outb(0x20, 0x20); //EOI
}
 
void irq11_handler(void) {
          outb(0xA0, 0x20);
          outb(0x20, 0x20); //EOI
}
 
void irq12_handler(void) {
          outb(0xA0, 0x20);
          outb(0x20, 0x20); //EOI
}
 
void irq13_handler(void) {
          outb(0xA0, 0x20);
          outb(0x20, 0x20); //EOI
}
 
void irq14_handler(void) {
          outb(0xA0, 0x20);
          outb(0x20, 0x20); //EOI
}
 
void irq15_handler(void) {
          outb(0xA0, 0x20);
          outb(0x20, 0x20); //EOI
}

#endif