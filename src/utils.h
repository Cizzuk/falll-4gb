#ifndef __Utils_h_INCLUDE
#define __Utils_h_INCLUDE

// Screen constants
// #define SCREEN_TOP 16
// #define SCREEN_BOTTOM (SCREEN_TOP + SCREEN_HEIGHT)
// #define SCREEN_LEFT 8
// #define SCREEN_RIGHT (SCREEN_LEFT + SCREEN_WIDTH)
// #define SPRITE_SIZE 8

extern UINT8 uint8_random(UINT8 min, UINT8 max);
extern BOOLEAN check_collision(UINT8 a_left, UINT8 a_top, UINT8 a_right, UINT8 a_bottom,
                               UINT8 b_left, UINT8 b_top, UINT8 b_right, UINT8 b_bottom);

#endif
