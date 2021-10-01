#include <util.h>
#include <debug.h>
#include <info.h>

extern info_t *info;

void phex2bit(int v) {
    debug("%s", "Binary (16bit): ");
    int z;
    int cpt = 0;
    for (z = 32768; z > 0; z >>= 1)
    {
        if (cpt % 4 == 0){
            debug("%c", ' ');
        }
        cpt++;
        debug(((v & z) == z) ? "1" : "0");
    }
    debug("%s", "\n");
}

void pmemory_zone() {
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
}