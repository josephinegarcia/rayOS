#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

#include <stdint.h>

// struct gdt_entry{
//     unsigned short limit_low;
//     unsigned short base_low;
//     unsigned char base_middle;
//     unsigned char access;
//     unsigned char granularity;
//     unsigned char base_high;
// } __attribute__((packed));

struct gdt_entry{
    uint16_t limit_low;             //The lower 16 bits of the limit
    uint16_t base_low;              //The lower 16 bits of the base
    uint8_t base_middle;            //The next 8 bits of the base
    uint8_t access;                 //Access flag, determines the ring level this segment can be used in
    uint8_t granularity;
    uint8_t base_high;              //The last 8 bits of the base
} __attribute__((packed));

// struct gdt_ptr{
//     unsigned short limit;
//     unsigned int base;
// } __attribute__((packed));

struct gdt_ptr{
    uint16_t limit;                 //The upper 16 bits of all selector limits ; limit = size of table -1
    uint32_t base;                  //The address of the first gdt_entry struct
} __attribute__((packed));

struct gdt_entry gdt[5];
struct gdt_ptr gp;

extern void gdt_flush();

void gdt_install()
{
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                    //Null segment

    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);     //Kernel Code segment

    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);     //Kernel Data segment

    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);     //User Code segment

    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);     //User Data segment
    gdt_flush();
}

void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

#endif /* INCLUDE_GDT_H */ 