#ifndef MEMORY_H
#define MEMORY_H
#include<stdint.h>

// helper constants
#define PAGE_SIZE 0x1000
#define PAGE_ALIGN 0xfffff000
#define NOT_ALIGNED ~(PAGE_ALIGN)
#define PAGE_ALIGN_UP(addr) (((addr) & NOT_ALIGNED) ? (((addr) & PAGE_ALIGN) + PAGE_SIZE) \
                                                    : ((addr)))
// 1024 DWORD entries per PD and PT
#define PAGE_ENTRIES 1024
#define PAGE_TABLE_SIZE (sizeof(uint32_t)*PAGE_ENTRIES)

// SI units to number of pages
#define PAGES_PER_KB(kb) (PAGE_ALIGN_UP((kb)*1024) / PAGE_SIZE)
#define PAGES_PER_MB(mb) (PAGE_ALIGN_UP((mb)*1024*1024) / PAGE_SIZE)
#define PAGES_PER_GB(gb) (PAGE_ALIGN_UP((gb)*1024*1024*1024) / PAGE_SIZE)

// page flags
#define PAGE_ENTRY_PRESENT 0x1
#define PAGE_ENTRY_RW      0x2
#define PAGE_ENTRY_ACCESS  0x20

// Information about the Kernel from the linker
extern uint32_t KERNEL_START;
extern uint32_t KERNEL_END;
extern uint32_t EARLY_KMALLOC_START;
extern uint32_t EARLY_KMALLOC_END;

void paging_init();
uint32_t page_ident_map(uint32_t addr, uint32_t perm);
uint32_t page_map(uint32_t virt, uint32_t phys, uint32_t perm);
#endif