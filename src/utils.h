// Screen constants
// #define SCREEN_TOP 16
// #define SCREEN_BOTTOM (SCREEN_TOP + SCREEN_HEIGHT)
// #define SCREEN_LEFT 8
// #define SCREEN_RIGHT (SCREEN_LEFT + SCREEN_WIDTH)
// #define SPRITE_SIZE 8

extern UINT8 uint8_random(UINT8 min, UINT8 max);
extern BOOLEAN check_collision(INT16 a_left, INT16 a_top, INT16 a_right, INT16 a_bottom,
                                  INT16 b_left, INT16 b_top, INT16 b_right, INT16 b_bottom);
