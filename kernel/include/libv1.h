#ifndef __LIBV1_H__
#define __LIBV1_H__

#include <types.h>
#include <debug.h>
#include <info.h>
#include <segmem.h>

#define EXIT_FAILURE 1
#define BASE_ADRESS_FREEZONE 0x0100000
#define END_ADRESS_FREEZONE 0x07fe0000 - 1


void pmemory_zone();
uint32_t is_free_memory_zone_available(size_t addr);
size_t vmmap(uint32_t size); 
void exit();
void set_seg_regs();
void setsegdesc(seg_desc_t * seg_desc, uint32_t ringvalue , uint32_t registervalue);
void show_gdt(gdt_reg_t gdt);
void show_idt(idt_reg_t idt);


#endif
