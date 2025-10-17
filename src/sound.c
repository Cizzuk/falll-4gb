#include <gb/gb.h>
#include <gb/cgb.h>
#include "sound.h"

void init_sound(void) {
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;
}
