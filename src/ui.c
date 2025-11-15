#include <gb/gb.h>
#include <gb/cgb.h>

#include "main.h"
#include "ui.h"
#include "utils.h"

// Clear needed rows
static const UINT8 clear_rows_title[] = {
    UI_SCORE_ROW,
    UI_LIVES_ROW,
    UI_GAMEOVER_TOP_ROW,
    UI_GAMEOVER_BOTTOM_ROW
};

static const UINT8 clear_rows_gameplay[] = {
    UI_TITLE_ROW,
    UI_MENU_START_ROW,
    UI_MENU_CHANGE_ROW
};

// FALLLL
static const UINT8 ui_title_text[6] = {
    UI_TILE_TITLE_F,
    UI_TILE_TITLE_A,
    UI_TILE_TITLE_L,
    UI_TILE_TITLE_L,
    UI_TILE_TITLE_L,
    UI_TILE_TITLE_L
};

// START
static const UINT8 ui_menu_start_text[5] = {
    UI_TILE_TEXT_S,
    UI_TILE_TEXT_T,
    UI_TILE_TEXT_A,
    UI_TILE_TEXT_R,
    UI_TILE_TEXT_T
};

// CHANGE
static const UINT8 ui_menu_change_text[6] = {
    UI_TILE_TEXT_C,
    UI_TILE_TEXT_H,
    UI_TILE_TEXT_A,
    UI_TILE_TEXT_N,
    UI_TILE_TEXT_G,
    UI_TILE_TEXT_E
};

// NICE!!
static const UINT8 ui_tile_overflow[UI_SCORE_DIGITS] = {
    UI_TILE_TEXT_N,
    UI_TILE_TEXT_I,
    UI_TILE_TEXT_C,
    UI_TILE_TEXT_E,
    UI_TILE_TEXT_EXCL,
    UI_TILE_TEXT_EXCL
};

// GAME
static const UINT8 ui_gameover_top_text[4] = {
    UI_TILE_TEXT_G,
    UI_TILE_TEXT_A,
    UI_TILE_TEXT_M,
    UI_TILE_TEXT_E
};

// OVER
static const UINT8 ui_gameover_bottom_text[5] = {
    UI_TILE_BLANK,
    UI_TILE_DIGIT_START,
    UI_TILE_TEXT_V,
    UI_TILE_TEXT_E,
    UI_TILE_TEXT_R
};

// Row palettes
static const UINT8 ui_row_palettes[UI_WINDOW_TILE_HEIGHT] = {
    UI_ATTR_DEFAULT, // Row 0
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_LIFE,    // Row 3 (lives)
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT,
    UI_ATTR_DEFAULT
};

inline void set_ui_tile(UINT8 column, UINT8 row, UINT8 tile) {
    set_win_tile_xy(column, row, tile);
}

inline void set_ui_text(UINT8 start_column, UINT8 row, const UINT8 *tiles, UINT8 length) {
    for (UINT8 i = 0U; i < length; i++) {
        set_ui_tile(start_column + i, row, tiles[i]);
    }
}

inline void clear_row(UINT8 row) {
    for (UINT8 column = UI_PADDING_LEFT; column < UI_WINDOW_TILE_WIDTH; column++) {
        set_ui_tile(column, row, UI_TILE_BLANK);
    }
}

void init_window(void) {
    fill_win_rect(0U, 0U, UI_WINDOW_TILE_WIDTH, UI_WINDOW_TILE_HEIGHT, UI_TILE_BLANK);
    move_win(UI_WINDOW_X, UI_WINDOW_Y);
    init_window_attr();
}

void init_window_attr(void) {
    if (_cpu == CGB_TYPE) {
        VBK_REG = 1U;
        for (UINT8 row = 0U; row < UI_WINDOW_TILE_HEIGHT; row++) {
            const UINT8 palette = ui_row_palettes[row] & 0x07U;
            for (UINT8 column = 0U; column < UI_WINDOW_TILE_WIDTH; column++) {
                set_win_tile_xy(column, row, palette);
            }
        }
        VBK_REG = 0U;
    }
}

void init_ui_title(void) {
    for (UINT8 i = 0U; i < (UINT8)sizeof(clear_rows_title); i++) {
        clear_row(clear_rows_title[i]);
    }
    set_ui_text(UI_TITLE_COLUMN, UI_TITLE_ROW, ui_title_text, (UINT8)sizeof(ui_title_text));
    render_title_menu();
}

void init_ui_gameplay(void) {
    for (UINT8 i = 0U; i < (UINT8)sizeof(clear_rows_gameplay); i++) {
        clear_row(clear_rows_gameplay[i]);
    }
    render_score();
    render_lives();
}

void render_title_menu(void) {
    // Set cursor position
    const UINT8 start_cursor_tile = cursor_pos ? UI_TILE_BLANK : UI_TILE_CURSOR;
    const UINT8 change_cursor_tile = cursor_pos ? UI_TILE_CURSOR : UI_TILE_BLANK;

    // START section
    set_ui_tile(UI_MENU_CURSOR_COLUMN, UI_MENU_START_ROW, start_cursor_tile);
    set_ui_text(UI_MENU_TEXT_COLUMN, UI_MENU_START_ROW, ui_menu_start_text, (UINT8)sizeof(ui_menu_start_text));

    // CHANGE section
    set_ui_tile(UI_MENU_CURSOR_COLUMN, UI_MENU_CHANGE_ROW, change_cursor_tile);
    set_ui_text(UI_MENU_TEXT_COLUMN, UI_MENU_CHANGE_ROW, ui_menu_change_text, (UINT8)sizeof(ui_menu_change_text));
}

void render_score(void) {
    // If score is 1M, show NICE!!
    if (score[2] > 99U) {
        for (UINT8 i = 0U; i < UI_SCORE_DIGITS; i++) {
            const UINT8 column = UI_SCORE_COLUMN + (UI_SCORE_DIGITS - 1U - i);
            const UINT8 tile = ui_tile_overflow[UI_SCORE_DIGITS - 1U - i];
            set_ui_tile(column, UI_SCORE_ROW, tile);
        }
        return;
    }

    UINT8 score_text[UI_SCORE_DIGITS];
    score_text[5] = get_first_digit(score[0]) + UI_TILE_DIGIT_START;
    score_text[4] = get_second_digit(score[0]) + UI_TILE_DIGIT_START;
    score_text[3] = get_first_digit(score[1]) + UI_TILE_DIGIT_START;
    score_text[2] = get_second_digit(score[1]) + UI_TILE_DIGIT_START;
    score_text[1] = get_first_digit(score[2]) + UI_TILE_DIGIT_START;
    score_text[0] = get_second_digit(score[2]) + UI_TILE_DIGIT_START;
    for (UINT8 i = 0U; i < UI_SCORE_DIGITS; i++) {
        const UINT8 column = UI_SCORE_COLUMN + (UI_SCORE_DIGITS - 1U - i);
        set_ui_tile(column, UI_SCORE_ROW, score_text[UI_SCORE_DIGITS - 1U - i]);
    }
}

void render_lives(void) {
    for (UINT8 i = 0U; i < UI_MAX_LIVES; i++) {
        if (i < player_life) {
            set_ui_tile(UI_LIVES_COLUMN + i, UI_LIVES_ROW, UI_TILE_HEART);
        } else {
            set_ui_tile(UI_LIVES_COLUMN + i, UI_LIVES_ROW, UI_TILE_BLANK);
        }
    }
}

void render_gameover(void) {
    // GAME
    set_ui_text(UI_GAMEOVER_COLUMN, UI_GAMEOVER_TOP_ROW, ui_gameover_top_text, (UINT8)sizeof(ui_gameover_top_text));

    // OVER
    set_ui_text(UI_GAMEOVER_COLUMN, UI_GAMEOVER_BOTTOM_ROW, ui_gameover_bottom_text, (UINT8)sizeof(ui_gameover_bottom_text));
}