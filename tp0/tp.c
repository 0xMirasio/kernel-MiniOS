/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <util.h>
#include <string.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("%s","[Basic Info]\n");
   debug("kernel mem [0x%x - 0x%x]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);
   phex2bit(info->mbi->flags);

   // Printing Memory ZONE
   debug("%s", "\n[Memory Zone]\n");
   debug("mmap_addr = 0x%x, mmap_length = 0x%x\n",info->mbi->mmap_addr, info->mbi->mmap_length);
   multiboot_memory_map_t *mmap;

   for (mmap = (multiboot_memory_map_t *) info->mbi->mmap_addr;(unsigned long) mmap < info->mbi->mmap_addr + info->mbi->mmap_length;mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
 	debug("size = 0x%x, base_addr = 0x%x%08x,"
        "length = 0x%x%08x, type = 0x%x [%s]\n",
        (unsigned) mmap->size,
       	(unsigned) (mmap->addr >> 32),
       	(unsigned) (mmap->addr & 0xffffffff),
       	(unsigned) (mmap->len >> 32),
       	(unsigned) (mmap->len & 0xffffffff),
       	(unsigned) mmap->type,
   	(0x01 & (unsigned) mmap->type) ? "Available" : "Not Available");
   }
   debug("[End of Memory Zone]\n");

   //writing to RAM
    debug("\n[Test RAM]\n");
    int data;
    unsigned char *ptr = (unsigned char*)0x1000;
    *ptr = 8; // work, because 0x1000 is available (CF Memory ZOne)
    data = *ptr;
    debug("%s!: Data %d value in 0x%x [SHOULD SUCCESS : WRITE OK]\n", (data == 8) ? "Sucess" : "Fail", data, ptr);
    unsigned char *ptr2 = (unsigned char*)0x0fffc0000;
    *ptr2 = 5; // don't work, because 0x09fc04 isn't available (CF Memory ZOne)
    data = *ptr2;
    debug("%s!: Data %d value in 0x%x [SHOULD FAIL : READONLY]\n", (data == 5) ? "Sucess" : "Fail", data, ptr2);    
    unsigned char *ptr3  = (unsigned char*)0x0f0000;
    *ptr3 = 15; // don't work, because 0x09fc04 isn't available (CF Memory ZOne)
    data = *ptr3;
    debug("%s!: Data %d value in 0x%x [SHOULD FAIL : READONLY]\n", (data == 15) ? "Sucess" : "Fail", data, ptr3);
    unsigned char *outOfRam = (unsigned char *)0x8000004;  // 128Mb + 4, out of VM RAM
    *outOfRam = 50;
    data = *outOfRam;
    debug("%s!: Data %d value in 0x%x [SHOULD FAIL : OUT OF MEMORY RAM]\n", (data == 50) ? "Sucess" : "Fail", data, outOfRam);
    debug("[End of Test RAM]\n");
}

