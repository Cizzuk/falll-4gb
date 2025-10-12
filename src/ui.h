#ifndef __UI_h_INCLUDE
#define __UI_h_INCLUDE

#include <gb/gb.h>

#define SCREEN_BOTTOM 160U
#define PLAY_AREA_WIDTH 54U
#define PLAY_AREA_MIN_X 24U
#define PLAY_AREA_MAX_X (PLAY_AREA_MIN_X + PLAY_AREA_WIDTH)

#define UI_SCORE_POS_X 152U // right aligned
#define UI_SCORE_POS_Y 32U
#define UI_LIVES_POS_X 152U // right aligned
#define UI_LIVES_POS_Y 40U
#define GAMEOVER_POS_X (PLAY_AREA_MIN_X + (PLAY_AREA_WIDTH / 2) - 32U)
#define GAMEOVER_POS_Y 48U

extern UINT8 get_first_digit(UINT8 num);
extern UINT8 get_second_digit(UINT8 num);
extern void render_score(UINT8 score[3]);
extern void render_lives(UINT8 lives);
extern void render_gameover(BOOLEAN is_gameover);

#endif
