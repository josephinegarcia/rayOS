#include <kernel/memory.h>
#include <kernel/kheap.h>
#include <stdio.h>
#include <string.h>

// initialize the KERNEL_* convience variables
// allows us forget about using & all the time
// uint32_t KERNEL_START = (uint32_t)&_KERNEL_START;
// uint32_t KERNEL_END = (uint32_t)&_KERNEL_END;
// uint32_t EARLY_KMALLOC_START = (uint32_t)&_EARLY_KMALLOC_START;
// uint32_t EARLY_KMALLOC_END = (uint32_t)&_EARLY_KMALLOC_END;

// stored Page Directory
static uint32_t * pageDirectory = NULL;
// A reverse mapping from physical to virtual
static uint32_t ** ptPhysToVirt = NULL;
// Guard against multiple enables of paging and for 
// virtual to physical translation
static int kernelPagingEnabled = 0;

static void page_map_pte(uint32_t * pt, uint32_t index, uint32_t phys, uint32_t perm);
static uint32_t * _native_get_page_directory();
static void _native_set_page_directory(uint32_t * phyDir);
static void _native_paging_enable();
static void _native_paging_disable();

uint32_t virt_to_phys(uint32_t virt_addr)
{
  uint32_t * pd = NULL;
  uint32_t * pt = NULL;
  uint32_t pde = 0;

  uint32_t index = virt_addr / PAGE_SIZE;
  uint32_t pageDirI = (index / 1024) % 1024;
  uint32_t pageTableI = index % 1024;

  // we are referencing our virtual memory alloc'd page directory
  if(kernelPagingEnabled) {
    pd = pageDirectory; // virtual address
    pde = (uint32_t)ptPhysToVirt[pageDirI] |
      (uint32_t)pd[pageDirI] & PAGE_ENTRY_PRESENT; // virtual address with flags
    pt = (uint32_t*)(pde & PAGE_ALIGN);
  } else {
    pd = _native_get_page_directory(); // identity mapped physical address
    pde = (uint32_t)pd[pageDirI]; // identity mapped physical address
    pt = (uint32_t*)(pde & PAGE_ALIGN);
  }

  // TODO: make this more robust
//   //ASSERT(pde & PAGE_ENTRY_PRESENT);
//   //ASSERT(pt[pageTableI] & PAGE_ENTRY_PRESENT);

  // return just physical address without flags
  return pt[pageTableI] & PAGE_ALIGN;
}

void paging_init()
{
  uint32_t i;
  uint32_t addr = 0;

  // 1024 page table entries (pointers)
  pageDirectory = (uint32_t *)kmalloc_early_align(PAGE_TABLE_SIZE);
  ptPhysToVirt = (uint32_t **)kmalloc_early_align(PAGE_TABLE_SIZE);

  // initialize all the page tables to not present, rw, supervisor
  memset(pageDirectory, 0, PAGE_TABLE_SIZE);
  // initialize reverse mappings
  memset(ptPhysToVirt, 0, PAGE_TABLE_SIZE);

  uint32_t * oldDir = _native_get_page_directory();
  printf("paging: PTD at %p (old %p)\n",
      virt_to_phys((uint32_t)pageDirectory), oldDir);

  // Identity map the 1 megabyte
  for(i = 0; i < PAGES_PER_MB(1); i++) {
    page_ident_map(addr, PAGE_ENTRY_RW);
    addr += PAGE_SIZE;
  }

  // move our kernel to the higher half
  uint32_t virtBase = 0xC0000000;
  uint32_t virtStart = KERNEL_START;
  uint32_t virtEnd = PAGE_ALIGN_UP(KERNEL_END);

  for(addr = virtStart-virtBase; virtStart < virtEnd; virtStart += PAGE_SIZE, addr += PAGE_SIZE) {
    page_map(virtStart, addr, PAGE_ENTRY_RW);
  }

  // set our directory and enable paging
  _native_set_page_directory(virt_to_phys(pageDirectory));
  _native_paging_enable();
  kernelPagingEnabled = 1;
}

/* A direct mapping between the virtual and physical realm 
 */
uint32_t page_ident_map(uint32_t addr, uint32_t perm)
{
  return page_map(addr, addr, perm);
}

void page_ident_map_range(uint32_t start, uint32_t end, uint32_t perm)
{
  //ASSERT(start < end);

  end = PAGE_ALIGN_UP(end);

  for(; start < end; start += PAGE_SIZE)
    page_map(start, start, perm);
}

/* Allocates a mapping between the requested virtual address
 * and the physical address, using the requested permissions.
 * Returns the address of the PTE
 */
uint32_t page_map(uint32_t virt, uint32_t phys, uint32_t perm)
{
  //ASSERT(!(virt & NOT_ALIGNED || phys & NOT_ALIGNED));
  //ASSERT(pageDirectory);

  //printf("page_map: %p -> %p\n", virt, phys);

  uint32_t index = virt / PAGE_SIZE;
  uint32_t pageDirI = (index / 1024) % 1024;
  uint32_t pageTableI = index % 1024;

  // if the page table isn't present, create it
  if(!(pageDirectory[pageDirI] & PAGE_ENTRY_PRESENT))
  {
    uint32_t * pageTable = (uint32_t *)kmalloc_early_align(PAGE_TABLE_SIZE);

    // Clear all physical addresses and flags
    memset(pageTable, 0, PAGE_TABLE_SIZE);

    // Add the page table to the directory and mark it as present
    // NOTE: PDE's MUST have a physical address
    pageDirectory[pageDirI] = virt_to_phys((uint32_t)pageTable)
      | PAGE_ENTRY_PRESENT | PAGE_ENTRY_RW;

    printf("page_map: table %u created at %p (%p)\n",
        pageDirI, pageTable, pageDirectory[pageDirI]);

    // store the virtual address of the PTE
    ptPhysToVirt[pageDirI] = pageTable;
  }

  // load our virtual PT address from our reverse mapping
  uint32_t * pageTable = ptPhysToVirt[pageDirI];
  page_map_pte(pageTable, pageTableI, phys, perm);

  return (uint32_t)&pageTable[pageTableI];
}

static void page_map_pte(uint32_t * pt, uint32_t index, uint32_t phys, uint32_t perm)
{
  //ASSERT(pt);
  //ASSERT(index < PAGE_ENTRIES);

  // clear out the page table entry, but keep the flags
  pt[index] &= ~PAGE_ALIGN;

  // OR on our physical address
  pt[index] |= phys;

  // Add permissions
  pt[index] |= perm;

  // mark the entry as present
  pt[index] |= PAGE_ENTRY_PRESENT;
}

static inline void _native_set_page_directory(uint32_t * phyDir)
{
  __asm__ volatile("mov %0, %%cr3":: "b"(phyDir));
}

static inline uint32_t * _native_get_page_directory()
{
  uint32_t ret;
  __asm__ volatile("mov %%cr3, %0": "=b" (ret));

  return (uint32_t *)ret;
}

static inline void _native_paging_enable()
{
  uint32_t cr0;

  __asm__ volatile("mov %%cr0, %0": "=b"(cr0));
  cr0 |= 0x80000000;
  __asm__ volatile("mov %0, %%cr0":: "b"(cr0)); // brace yourself
}

static inline void _native_paging_disable()
{
  uint32_t cr0;

  __asm__ volatile("mov %%cr0, %0": "=b"(cr0));
  cr0 &= ~(0x80000000U);
  __asm__ volatile("mov %0, %%cr0":: "b"(cr0));
}