#include <libv1.h>

extern info_t *info;
uint8_t * eip = (uint8_t *) BASE_ADRESS_FREEZONE;

// affiche les zones mémoires
void pmemory_zone() {
   debug("%s", "\n[Memory Zone]\n");
   debug("mmap_addr = 0x%x, mmap_length = 0x%x\n",info->mbi->mmap_addr, info->mbi->mmap_length);
   multiboot_memory_map_t *mmap;

   for (mmap = (multiboot_memory_map_t *) info->mbi->mmap_addr;(size_t) mmap < info->mbi->mmap_addr + info->mbi->mmap_length;mmap = (multiboot_memory_map_t *) ((size_t) mmap + mmap->size + sizeof (mmap->size))) {
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
// permet de connaitre si l'adresse donnée en param peux être écrite en mémoire
uint32_t is_free_memory_zone_available(size_t addr_t8) {
   multiboot_memory_map_t *mmap;

   for (mmap = (multiboot_memory_map_t *) info->mbi->mmap_addr;(unsigned long) mmap < info->mbi->mmap_addr + info->mbi->mmap_length;mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
 	
    
    
    unsigned long addr_t7 = (unsigned long) (mmap->addr & 0xffffffff);
    unsigned long size = (unsigned long)(mmap->len & 0xffffffff);
    //debug("IF : %lu,%lu,%lu,%lu, %x\n", addr_t8, addr_t7, addr_t7 + size, size, mmap->type);
    if (addr_t8 >= addr_t7 && addr_t8 <= (addr_t7 + size)){
        if (mmap->type == 1) {
            debug("[IS_MEMORY_ZONE_FREE] 0x%x is free for writing\n", addr_t8);
            return 0; // available
        }
        else {
            debug("[IS_MEMORY_ZONE_FREE] 0x%x isn't free for writing\n", addr_t8);
            return 1; // not available
        }
    }
    // because unsigned long int is max 2 ^32 -1, let's add another verification if number > addr_t8 at the limit
    // of the unsigned long int, because addr_t7 + size will fail (=0)
    if (addr_t7 >= 4283691008) { // LAST MEMORY ZONE
        debug("[IS_MEMORY_ZONE_FREE] 0x%x isn't free for writing\n", addr_t8);
        return 1;
    }
   }

   return 2; // case not found(unknow)
   
}

// retourne une adresse disponible a l'écriture dans la mémoire, adresse de base : BASE_ADRESS_FREEZONE
size_t vmmap(uint32_t size) {
    debug("[VMMAP] Requesting memory , size : %d\n", size);
    for (uint8_t *i = eip; i < eip + size; i += 4) {
        if (*i & 1) {
            debug("[VMMAP] Memory used at 0x%x\n", i);
            return 1; // fail
        }
    }
    size_t addr_free = (size_t) eip;
    eip = eip + size;
    debug("[VMMAP] Memory from [0x%x, 0x%x] is now used\n", addr_free, eip);
    debug("[VMMAP] New BASE_ADRESS_FREEZONE : 0x%x\n", eip);
    return addr_free; // on fourni une adresse pour l'écriture
}

void exit() {
    __asm__("ret"); // sale, vraiment sale
}

void set_seg_regs(){

	set_cs(gdt_krn_seg_sel(1));
	set_ss(gdt_krn_seg_sel(2));
	set_ds(gdt_krn_seg_sel(2));
	set_es(gdt_krn_seg_sel(2));
	set_fs(gdt_krn_seg_sel(2));
	set_gs(gdt_krn_seg_sel(2));

}

void setsegdesc(seg_desc_t * seg_desc, uint32_t ringvalue , uint32_t registervalue) {
    seg_desc->limit_1 = 0xffff;  // 20 bit => 0xffff 16bit + 0xf (4 bit)
    seg_desc->limit_2 = 0xf;
    seg_desc->base_1 = 0;
    seg_desc->base_2 = 0;
    seg_desc->base_3 = 0;
    seg_desc->type = registervalue;
    seg_desc->dpl = ringvalue;
    seg_desc->p = 1; // on veut que ce segment soit present
    seg_desc->l = 0;
    seg_desc->d = 1; // 32 bit segment 
    seg_desc->s =1; // code/data
    seg_desc->g = 1; // 32bit segment, donc on veut un 4096 bytes page => g=1
}

void show_gdt(gdt_reg_t gdt_reg) {
    uint32_t i= 0;
    uint32_t n = (gdt_reg.limit +1)/sizeof(seg_desc_t); // nombre d'entrée dans la table GDT
    for (i = 0; i < n; i++) {
        seg_desc_t *addr_desc = &gdt_reg.desc[i];
        uint32_t addr_base = addr_desc->base_1 << 24 | addr_desc->base_2 << 16 | addr_desc->base_3; 
        // calculé de cette manière, car Base Address[31:24], base2 adress [23:16] , base4 adresse [15,0]
        uint32_t addr_limit = addr_desc->limit_1 << 16 | addr_desc->limit_2;
        debug("[GDT] descriptor segment %d -> adress : 0x%x, base : 0x%x, limit : 0x%x, type : %d\n" , i, addr_desc, addr_base, addr_limit, addr_desc->type);

    }
}

void show_idt(idt_reg_t idt) {
    uint32_t i= 0;
    uint32_t n = (idt.limit +1)/sizeof(int_desc_t); // nombre d'entrée dans la table IDT
    for (i = 0; i < n; i++) {
        int_desc_t *addr_desc = &idt.desc[i];
        uint32_t addr_offset = addr_desc->offset_1 << 16 | addr_desc->offset_2; 
        debug("[GDT] descriptor segment %d -> adress : 0x%x, base : 0x%x\n" , i, addr_desc, addr_offset);

    }
}