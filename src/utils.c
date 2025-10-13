#include <gb/gb.h>
#include <rand.h>

inline UINT8 uint8_random(UINT8 min, UINT8 max) {
    return (rand() % (max - min + 1)) + min;
}

inline BOOLEAN check_collision(UINT8 a_left, UINT8 a_top, UINT8 a_right, UINT8 a_bottom,
                        UINT8 b_left, UINT8 b_top, UINT8 b_right, UINT8 b_bottom) {
    return (a_left <= b_right) && (a_right >= b_left) && (a_top <= b_bottom) && (a_bottom >= b_top);
}
