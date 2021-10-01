/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <util.h>
#include <string.h>
#include <types.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("%s","[Basic Info]\n");
   debug("kernel mem [0x%x - 0x%x]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);
   phex2bit(info->mbi->flags);

   // Printing Memory ZONE
   pmemory_zone();

   //writing to RAM
    debug("\n[Test RAM]\n");
    int data;
    uint8_t *ptr = (uint8_t*)0x1000;
    *ptr = 8; // work, because 0x1000 is available (CF Memory ZOne)
    data = *ptr;
    debug("%s!: Data %d value in 0x%x [SHOULD SUCCESS : WRITE OK]\n", (data == 8) ? "Sucess" : "Fail", data, ptr);
    uint8_t *ptr2 = (uint8_t*)0x0fffc0000;
    *ptr2 = 5; // don't work, because 0x09fc04 isn't available (CF Memory ZOne)
    data = *ptr2;
    debug("%s!: Data %d value in 0x%x [SHOULD FAIL : READONLY]\n", (data == 5) ? "Sucess" : "Fail", data, ptr2);    
    uint8_t *ptr3  = (uint8_t*)0x0f0000;
    *ptr3 = 15; // don't work, because 0x09fc04 isn't available (CF Memory ZOne)
    data = *ptr3;
    debug("%s!: Data %d value in 0x%x [SHOULD FAIL : READONLY]\n", (data == 15) ? "Sucess" : "Fail", data, ptr3);
    uint8_t *outOfRam = (uint8_t*)0x8000004;  // 128Mb + 4, out of VM RAM
    *outOfRam = 50;
    data = *outOfRam;
    debug("%s!: Data %d value in 0x%x [SHOULD FAIL : OUT OF MEMORY RAM]\n", (data == 50) ? "Sucess" : "Fail", data, outOfRam);
    debug("[End of Test RAM]\n");
}

