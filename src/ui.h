#ifndef __UI_h_INCLUDE
#define __UI_h_INCLUDE

#include <gb/gb.h>

extern UINT8 get_first_digit(UINT8 num);
extern UINT8 get_second_digit(UINT8 num);
extern void render_score(UINT8 score[3]);
extern void render_lives(UINT8 lives);
extern void render_gameover(BOOLEAN is_gameover);

#endif
