/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <types.h>
#include <libv1.h>
#include <util.h>
#include <string.h>


extern info_t *info;

seg_desc_t gdt1[4];

void tp()
{

    pmemory_zone();     
    //uint32_t res = is_free_memory_zone_available(0xff53f000);
    // on reserve dans la mémoire avec notre fonction vmmap()

    // Q1
    gdt_reg_t gdt_reg;
    get_gdtr(gdt_reg);
    show_gdt(gdt_reg);

    //Q2    
    gdt_reg_t newgdt;
    gdt1[0].raw = 0ULL;   
    setsegdesc(&gdt1[1],0,SEG_DESC_CODE_XR); // indice 1, en ring 0, XR
    setsegdesc(&gdt1[2],0,SEG_DESC_DATA_RW); // indice 2, ring0, WR

    newgdt.desc = gdt1;
    newgdt.limit = sizeof(gdt1) - 1;
    set_gdtr(newgdt);
    debug("[INFO] GDT SET, added IDX1 & IDX2\n");

    set_seg_regs();
    debug("[INFO] Segmentation register set\n");

    //Q3

    char  src[64];
    char *dst = 0;
    memset(src, 0xff, 64);

    uint32_t base = 0x600000;

    gdt1[3].raw = 0ULL;
    gdt1[3].base_1 = base;
    gdt1[3].base_2 = base >> 16;
    gdt1[3].base_3 = base >> 24;

    gdt1[3].limit_1 = 32 - 1;

    gdt1[3].type = SEG_DESC_DATA_RW;
    gdt1[3].d   = 1;
    gdt1[3].s   = 1;
    gdt1[3].p   = 1;
    gdt1[3].dpl = 0;

    newgdt.limit = sizeof(gdt1) - 1;
    set_gdtr(newgdt);
    debug("[INFO] GDT2 SET, added IDX 3\n");
    
    set_es(gdt_krn_seg_sel(3));

    _memcpy8(dst, src, 32);
    //_memcpy8(dst, src, 64);


    









    
    


}
