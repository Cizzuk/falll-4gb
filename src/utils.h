#ifndef __Utils_h_INCLUDE
#define __Utils_h_INCLUDE

#include <rand.h>

// Screen constants
// #define SCREEN_TOP 16
// #define SCREEN_BOTTOM (SCREEN_TOP + SCREEN_HEIGHT)
// #define SCREEN_LEFT 8
// #define SCREEN_RIGHT (SCREEN_LEFT + SCREEN_WIDTH)
// #define SPRITE_SIZE 8

inline UINT8 uint8_mod3(UINT8 num) {
    // return num % 3U;
    if (num >= 192U) num -= 192U;
    if (num >= 96U) num -= 96U;
    if (num >= 48U) num -= 48U;
    if (num >= 24U) num -= 24U;
    if (num >= 12U) num -= 12U;
    if (num >= 6U) num -= 6U;
    if (num >= 3U) num -= 3U;
    return num;
}

inline UINT8 uint8_mod10(UINT8 num) {
    // return num % 10U;
    if (num >= 160U) num -= 160U;
    if (num >= 80U) num -= 80U;
    if (num >= 40U) num -= 40U;
    if (num >= 20U) num -= 20U;
    if (num >= 10U) num -= 10U;
    return num;
}

inline UINT8 uint8_random(UINT8 min, UINT8 max) {
    return (rand() % (max - min + 1)) + min;
}

inline BOOLEAN check_collision(UINT8 a_left, UINT8 a_top, UINT8 a_right, UINT8 a_bottom,
                        UINT8 b_left, UINT8 b_top, UINT8 b_right, UINT8 b_bottom) {
    return (a_left <= b_right) && (a_right >= b_left) && (a_top <= b_bottom) && (a_bottom >= b_top);
}

inline UINT8 get_first_digit(UINT8 num) {
    // return num % 10U;
    return uint8_mod10(num);
}

inline UINT8 get_second_digit(UINT8 num) {
    // return num / 10U;
    for (UINT8 i = 0; i < 10U; i++) {
        if (num < (i + 1U) * 10U) {
            return i;
        }
    }
    return 0U; // Fallback
}

#endif
