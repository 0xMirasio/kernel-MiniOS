/* GPLv2 (c) Airbus */
#include <debug.h>
#include <intr.h>
#include <info.h>
#include <libv1.h>

extern info_t *info;

void handler() {
    debug("Interruption handler\n");

}
void bp_handler() 
{
    debug("Interruption bp_handler\n");
     asm(
        "pusha;"
        "call handler;"
        "popa;"
        "leave;"
        "iret;"
        );

}
void bp_trigger()
{    
    __asm__("int3");
    debug("Resumed from exception !\n");
}

void tp()
{
    
    intr_init();
    idt_reg_t idt;
    get_idtr(idt);
    offset_t idt_addr = idt.addr;
    debug("IDT Adress : 0x%x\n", idt_addr);
    //show_idt(idt);
    int_desc_t * idt_table = idt.desc;
    offset_t isr = (offset_t)bp_handler;
    int_desc(&idt_table[3],gdt_krn_seg_sel(1), isr);
    bp_trigger();

}
