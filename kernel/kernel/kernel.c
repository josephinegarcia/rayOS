#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/multiboot.h>
#include <kernel/memmgr_phys.h>

//! format of a memory region
struct memory_region
{
	uint32_t startLo;
	uint32_t startHi;
	uint32_t sizeLo;
	uint32_t sizeHi;
	uint32_t type;
	uint32_t acpi_3_0;
};

//! different memory regions (in memory_region.type)
char *strMemoryTypes[] = {

	{"Available"},		//memory_region.type==0
	{"Reserved"},		//memory_region.type==1
	{"ACPI Reclaim"},	//memory_region.type==2
	{"ACPI NVS Memory"} //memory_region.type==3
};

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

int kernel_main(multiboot_info_t *mbd, unsigned int magic)
{
	printf("HELLO USER WORLD!!");
	// gdt_install();
	// idt_install();
	// isrs_install();
	// irq_install();
	// __asm__ __volatile__("sti");
	// initialize_serial_port(SERIAL_COM1_BASE, 2);
	// keyboard_install();
	// terminal_initialize();

	// if (MULTIBOOT_BOOTLOADER_MAGIC != magic)
	// 	printf("Invalid magic number: 0x%lx\n", (unsigned)magic);

	// //  /* Are mem_* valid? */
	// if (CHECK_FLAG(mbd->flags, 0))
	// 	printf("mem_lower = %uKB, mem_upper = %uKB\n", (unsigned)mbd->mem_lower, (unsigned)mbd->mem_upper);

	// if (CHECK_FLAG(mbd->flags, 1))
	// 	printf("boot_device = 0x%lx\n", (unsigned)mbd->boot_device);

	// /* Is the command line passed? */
	// if (CHECK_FLAG(mbd->flags, 2))
	// 	printf("cmdline = %s\n", (char *)mbd->cmdline);

	// /* Are mods_* valid? */
	// if (CHECK_FLAG(mbd->flags, 3))
	// {
	// 	multiboot_module_t *mod;
	// 	int i;

	// 	printf("mods_count = %d, mods_addr = 0x%lx\n",
	// 		   (int)mbd->mods_count, (int)mbd->mods_addr);
	// 	for (i = 0, mod = (multiboot_module_t *)mbd->mods_addr;
	// 		 i < mbd->mods_count;
	// 		 i++, mod++)
	// 		printf(" mod_start = 0x%lx, mod_end = 0x%lx, cmdline = %s\n",
	// 			   (unsigned)mod->mod_start,
	// 			   (unsigned)mod->mod_end,
	// 			   (char *)mod->cmdline);
	// }

	// /* Bits 4 and 5 are mutually exclusive! */
	// if (CHECK_FLAG(mbd->flags, 4) && CHECK_FLAG(mbd->flags, 5))
	// {
	// 	printf("Both bits 4 and 5 are set.\n");
	// 	return;
	// }

	// /* Is the symbol table of a.out valid? */
	// if (CHECK_FLAG(mbd->flags, 4))
	// {
	// 	multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbd->u.aout_sym);

	// 	printf("multiboot_aout_symbol_table: tabsize = 0x%0x, "
	// 		   "strsize = 0x%lx, addr = 0x%lx\n",
	// 		   (unsigned)multiboot_aout_sym->tabsize,
	// 		   (unsigned)multiboot_aout_sym->strsize,
	// 		   (unsigned)multiboot_aout_sym->addr);
	// }

	// /* Is the section header table of ELF valid? */
	// if (CHECK_FLAG(mbd->flags, 5))
	// {
	// 	multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbd->u.elf_sec);

	// 	printf("multiboot_elf_sec: num = %u, size = 0x%lx,"
	// 		   " addr = 0x%lx, shndx = 0x%lx\n",
	// 		   (unsigned)multiboot_elf_sec->num, (unsigned)multiboot_elf_sec->size,
	// 		   (unsigned)multiboot_elf_sec->addr, (unsigned)multiboot_elf_sec->shndx);
	// }

	// /* Are mmap_* valid? */
	// if (CHECK_FLAG(mbd->flags, 6))
	// {
	// 	multiboot_memory_map_t *mmap;

	// 	printf("mmap_addr = 0x%lx, mmap_length = 0x%lx\n",
	// 		   (unsigned)mbd->mmap_addr, (unsigned)mbd->mmap_length);
	// 	for (mmap = (multiboot_memory_map_t *)mbd->mmap_addr;
	// 		 (unsigned long)mmap < mbd->mmap_addr + mbd->mmap_length;
	// 		 mmap = (multiboot_memory_map_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size)))
	// 		printf(" size = 0x%lx, base_addr = 0x%lx%08lx,"
	// 			   " length = 0x%lx%08lx, type = %s\n",
	// 			   (unsigned)mmap->size,
	// 			   (unsigned)(mmap->addr >> 32),
	// 			   (unsigned)(mmap->addr & 0xffffffff),
	// 			   (unsigned)(mmap->len >> 32),
	// 			   (unsigned)(mmap->len & 0xffffffff),
	// 			   strMemoryTypes[(unsigned)mmap->type-1]);
	// }

	// //! get kernel size passed from boot loader
	// uint32_t kernelSize = mbd->mmap_length;

	// //Kernel Size should be the image size overall
	// if(kernelSize > 0)
	// 	printf("\nPassed the kernelSize %i\n", kernelSize);

	// //! get memory size in KB
	// uint32_t memSize = 1024 + mbd->mem_lower + mbd->mem_upper*64;

	// //! initialize the physical memory manager
	// //! we place the memory bit map used by the PMM at the end of the kernel in memory
	// pmmngr_init (memSize, 0x100000 + kernelSize*512);
	// printf("pmm initialized with %i KB physical memory; memLo: %i memHi: %i\n\n", memSize, mbd->mem_lower, mbd->mem_upper);

	// //! deinit the region the kernel is in as its in use
	// pmmngr_deinit_region (0x100000, kernelSize*512);

	// printf("\npmm regions initialized: %i allocation blocks; used or reserved blocks: %i\nfree blocks: %i\n",
	// 	pmmngr_get_block_count (),  pmmngr_get_use_block_count (), pmmngr_get_free_block_count () );

	// //! allocating and deallocating memory examples...
	// uint32_t* p = (uint32_t*)pmmngr_alloc_block ();
	// printf("\np allocated at 0x%x", p);

	// uint32_t* p2 = (uint32_t*)pmmngr_alloc_blocks (2);
	// printf("\nallocated 2 blocks for p2 at 0x%x", p2);

	// pmmngr_free_block (p);
	// p = (uint32_t*)pmmngr_alloc_block ();
	// printf("\nUnallocated p to free block 1. p is reallocated to 0x%x", p);

	// pmmngr_free_block (p);
	// pmmngr_free_blocks (p2, 2);

	return 0;
}