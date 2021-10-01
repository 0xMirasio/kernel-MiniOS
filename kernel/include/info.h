/* GPLv2 (c) Airbus */
#ifndef __INFO_DATA_H__
#define __INFO_DATA_H__

#include <types.h>
#include <segmem.h>
#include <mbi.h>

typedef struct information
{
   mbi_t *mbi;
   seg_desc_t *seg_desc;
   gdt_reg_t *gdt_reg;


} __attribute__((packed)) info_t;


#endif
