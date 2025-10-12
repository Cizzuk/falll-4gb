#include <gb/gb.h>
#include <rand.h>

UINT8 uint8_random(UINT8 min, UINT8 max) {
    return (rand() % (max - min + 1)) + min;
}

BOOLEAN check_collision(INT16 a_left, INT16 a_top, INT16 a_right, INT16 a_bottom,
                        INT16 b_left, INT16 b_top, INT16 b_right, INT16 b_bottom) {
    return (a_left <= b_right) && (a_right >= b_left) && (a_top <= b_bottom) && (a_bottom >= b_top);
}
