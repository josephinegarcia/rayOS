#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/multiboot.h>
#include <kernel/memmgr_phys.h>
//#include <>

//! format of a memory region
struct memory_region {

	uint32_t	startLo;
	uint32_t	startHi;
	uint32_t	sizeLo;
	uint32_t	sizeHi;
	uint32_t	type;
	uint32_t	acpi_3_0;
};


#define CHECK_FLAG(flags,bit) ((flags) & (1 << (bit)))

int kernel_main(multiboot_info_t* mbd, unsigned int magic){
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	initialize_serial_port(SERIAL_COM1_BASE, 2);
	keyboard_install();
	terminal_initialize();
	
	printf("HELLO \n");
	// if(MULTIBOOT_BOOTLOADER_MAGIC != magic)
	// 	printf ("Invalid magic number: 0x%x\n", (unsigned) magic);

	//  /* Are mem_* valid? */
  	// if (CHECK_FLAG (mbd->flags, 0))
    // 	printf ("mem_lower = %uKB, mem_upper = %uKB\n", (unsigned) mbd->mem_lower, (unsigned) mbd->mem_upper);

	// //printf("0x%u\n", (unsigned) mbd->flags);
	// printf("CHECK COMPLETE!\n");

	// //! get kernel size passed from boot loader
	// uint32_t kernelSize=0;
	// asm(
	// 	"movl (%0), %%edx;"
	// 	: "=r"(kernelSize)
	// 	:
	// 	: "%edx");

	// if(kernelSize > 0){
	// 	char *address = "0x00FF23";
	// 	while (*address != '\0'){
	// 		write_serial(SERIAL_COM1_BASE, *address);
	// 		*address++;
	// 	}
	// }

	// //! get memory size in KB
	// uint32_t memSize = 1024 + mbd->mem_lower + mbd->mem_upper*64;
	// printf("Memory Size in KB:%u\n", memSize);

	// //! initialize the physical memory manager
	// //! we place the memory bit map used by the PMM at the end of the kernel in memory
	// pmmngr_init (memSize, 0x100000 + kernelSize*512);

	// //printf("pmm initialized with %i KB physical memory; memLo: %i memHi: %i\n\n",
	// //	memSize,mbd->mem_lower,mbd->mem_upper);

	// // DebugSetColor (0x19);
	// // DebugPrintf ("Physical Memory Map:\n");

	// struct memory_region*	region = (struct memory_region*)0x1000;

	// for (int i=0; i<15; ++i) {

	// 	//! sanity check; if type is > 4 mark it reserved
	// 	if (region[i].type>4)
	// 		region[i].type=1;

	// 	//! if start address is 0, there is no more entries, break out
	// 	if (i>0 && region[i].startLo==0)
	// 		break;

	// 	//! display entry

	// 	// DebugPrintf ("region %i: start: 0x%x%x length (bytes): 0x%x%x type: %i (%s)\n", i, 
	// 	// 	region[i].startHi, region[i].startLo,
	// 	// 	region[i].sizeHi,region[i].sizeLo,
	// 	// 	region[i].type, strMemoryTypes[region[i].type-1]);

	// 	//! if region is avilable memory, initialize the region for use
	// 	if (region[i].type==1)
	// 		pmmngr_init_region (region[i].startLo, region[i].sizeLo);
	// }

	// //! deinit the region the kernel is in as its in use
	// pmmngr_deinit_region (0x100000, kernelSize*512);

	// // DebugSetColor (0x17);

	// // DebugPrintf ("\npmm regions initialized: %i allocation blocks; used or reserved blocks: %i\nfree blocks: %i\n",
	// // 	pmmngr_get_block_count (),  pmmngr_get_use_block_count (), pmmngr_get_free_block_count () );

	// //! allocating and deallocating memory examples...

	// // DebugSetColor (0x12);

	// uint32_t* p = (uint32_t*)pmmngr_alloc_block ();
	// printf("p allocated at 0x%llx\n", p);
	// // DebugPrintf ("\np allocated at 0x%lx", p);

	// uint32_t* p2 = (uint32_t*)pmmngr_alloc_blocks (2);
	// printf("allocated 2 blocks for p2 at 0x%llx\n", p2);
	// // DebugPrintf ("\nallocated 2 blocks for p2 at 0x%x", p2);

	// pmmngr_free_block (p);
	// p = (uint32_t*)pmmngr_alloc_block ();
	// // DebugPrintf ("\nUnallocated p to free block 1. p is reallocated to 0x%x", p);

	// pmmngr_free_block (p);
	// pmmngr_free_blocks (p2, 2);

	return 0;
}