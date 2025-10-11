#include <gb/gb.h>

UINT8 uint8_random(UINT8 min, UINT8 max) {
    return (rand() % (max - min + 1)) + min;
}
