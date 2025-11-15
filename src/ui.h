#ifndef __UI_h_INCLUDE
#define __UI_h_INCLUDE

#include <gb/gb.h>

#define SCREEN_BOTTOM 160U
#define PLAY_AREA_WIDTH 56U
#define PLAY_AREA_MIN_X 16U
#define PLAY_AREA_MAX_X (PLAY_AREA_MIN_X + PLAY_AREA_WIDTH)

#define UI_TILE_BLANK 0U
#define UI_TILE_DIGIT_START 2U
#define UI_TILE_CURSOR 25U
#define UI_TILE_HEART 26U

#define UI_TILE_TEXT_G 12U
#define UI_TILE_TEXT_A 13U
#define UI_TILE_TEXT_M 14U
#define UI_TILE_TEXT_E 15U
#define UI_TILE_TEXT_V 16U
#define UI_TILE_TEXT_R 17U
#define UI_TILE_TEXT_S 18U
#define UI_TILE_TEXT_T 19U
#define UI_TILE_TEXT_C 20U
#define UI_TILE_TEXT_H 21U
#define UI_TILE_TEXT_N 22U
#define UI_TILE_TEXT_I 23U
#define UI_TILE_TEXT_EXCL 24U

#define UI_TILE_TITLE_F 27U
#define UI_TILE_TITLE_A 28U
#define UI_TILE_TITLE_L 29U

#define UI_ATTR_DEFAULT 0U
#define UI_ATTR_LIFE 1U

#define UI_WINDOW_TILE_WIDTH 9U
#define UI_WINDOW_TILE_HEIGHT 18U
#define UI_WINDOW_X (7U + (20U - UI_WINDOW_TILE_WIDTH) * 8U)
#define UI_WINDOW_Y (18U - UI_WINDOW_TILE_HEIGHT) * 8U

#define UI_PADDING_LEFT 1U

#define UI_TITLE_COLUMN UI_PADDING_LEFT
#define UI_TITLE_ROW 5U

#define UI_MENU_CURSOR_COLUMN UI_PADDING_LEFT
#define UI_MENU_TEXT_COLUMN (UI_PADDING_LEFT + 1U)
#define UI_MENU_START_ROW 9U
#define UI_MENU_CHANGE_ROW 11U

#define UI_SCORE_DIGITS 6U
#define UI_SCORE_COLUMN UI_PADDING_LEFT
#define UI_SCORE_ROW 2U

#define UI_MAX_LIVES 3U
#define UI_LIVES_COLUMN UI_PADDING_LEFT
#define UI_LIVES_ROW 3U

#define UI_GAMEOVER_TOP_ROW 8U
#define UI_GAMEOVER_COLUMN (UI_PADDING_LEFT + 1U)
#define UI_GAMEOVER_BOTTOM_ROW 9U

extern void init_window(void);
extern void init_window_attr(void);
extern void init_ui_title(void);
extern void init_ui_gameplay(void);
extern void render_title_menu(void);
extern void render_score(void);
extern void render_lives(void);
extern void render_gameover(void);

#endif
