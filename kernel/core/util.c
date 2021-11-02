#include <util.h>


// affiche un hex en binaire 16bit
void phex2bit(int v) {
    debug("%s", "[PHEX2BIT] Binary (16bit): ");
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
