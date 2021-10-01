/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <types.h>
#include <util.h>

extern info_t *info;

void tp()
{
    debug("\n[INFO]\n");
    debug("Base Adress of GDT : 0x%x\n", info->gdt_reg->addr);
    pmemory_zone(); // we need a memory zone we can write GDT OUTPUT
    debug("[0x000000 - 0x09fc00] Is Free for writing, dumping GDT on 0x000000\n");
    offset_t addr = 0x0;
    get_gdtr(addr); // on dump tout le contenu de la GDT en 0x0

    uint8_t *ptr = (uint8_t*) 0x0;
    uint32_t value = *ptr;
    debug("Value : %d at 0x%x\n", value, ptr);
    phex2bit(value);


}
