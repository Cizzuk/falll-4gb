#include <gb/gb.h>
#include <gb/cgb.h>
#include "ui.h"

// FALLLL
static const UINT8 ui_title_text[6U] = {
    UI_TILE_TITLE_F,
    UI_TILE_TITLE_A,
    UI_TILE_TITLE_L,
    UI_TILE_TITLE_L,
    UI_TILE_TITLE_L,
    UI_TILE_TITLE_L
};

// START
static const UINT8 ui_menu_start_text[5U] = {
    UI_TILE_TEXT_S,
    UI_TILE_TEXT_T,
    UI_TILE_TEXT_A,
    UI_TILE_TEXT_R,
    UI_TILE_TEXT_T
};

// CHANGE
static const UINT8 ui_menu_change_text[6U] = {
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
static const UINT8 ui_gameover_top_text[4U] = {
    UI_TILE_TEXT_G,
    UI_TILE_TEXT_A,
    UI_TILE_TEXT_M,
    UI_TILE_TEXT_E
};

// OVER
static const UINT8 ui_gameover_bottom_text[5U] = {
    UI_TILE_BLANK,
    UI_TILE_DIGIT_START,
    UI_TILE_TEXT_V,
    UI_TILE_TEXT_E,
    UI_TILE_TEXT_R
};

static void set_ui_tile(UINT8 column, UINT8 row, UINT8 tile, UINT8 palette) {
    VBK_REG = 0;
    set_win_tile_xy(column, row, tile);

    // Set palette if on CGB hardware
    if (_cpu == CGB_TYPE) {
        VBK_REG = 1;
        set_win_tile_xy(column, row, palette & 0x07U);
        VBK_REG = 0;
    }
}

static void clear_row(UINT8 row) {
    for (UINT8 column = 0; column < UI_WINDOW_TILE_WIDTH; ++column) {
        set_ui_tile(column, row, UI_TILE_BLANK, UI_ATTR_DEFAULT);
    }
}

static void clear_all_rows(void) {
    for (UINT8 row = 0; row < 18U; ++row) {
        clear_row(row);
    }
}

static void set_ui_text(UINT8 start_column, UINT8 row, const UINT8 *tiles, UINT8 length, UINT8 palette) {
    for (UINT8 i = 0; i < length; ++i) {
        set_ui_tile(start_column + i, row, tiles[i], palette);
    }
}

inline static UINT8 get_first_digit(UINT8 num) {
    return num % 10U;
}

inline static UINT8 get_second_digit(UINT8 num) {
    return num / 10U;
}

void init_window(void) {
    fill_win_rect(0U, 0U, 20U, 18U, UI_TILE_BLANK);
    move_win(UI_WINDOW_X, UI_WINDOW_Y);
}

void init_ui_title(void) {
    clear_all_rows();
    set_ui_text(UI_TITLE_COLUMN, UI_TITLE_ROW, ui_title_text, (UINT8)sizeof(ui_title_text), UI_ATTR_DEFAULT);
    render_title_menu(FALSE);
}

void init_ui_gameplay(UINT8 score[3], UINT8 lives) {
    clear_all_rows();
    render_score(score);
    render_lives(lives);
}

void render_title_menu(BOOLEAN cursor_pos) {
    UINT8 start_cursor_tile = UI_TILE_BLANK;
    UINT8 change_cursor_tile = UI_TILE_BLANK;

    // Set cursor position
    if (cursor_pos) {
        change_cursor_tile = UI_TILE_CURSOR;
    } else {
        start_cursor_tile = UI_TILE_CURSOR;
    }

    // START section
    clear_row(UI_MENU_START_ROW);
    set_ui_tile(UI_MENU_CURSOR_COLUMN, UI_MENU_START_ROW, start_cursor_tile, UI_ATTR_DEFAULT);
    set_ui_text(UI_MENU_TEXT_COLUMN, UI_MENU_START_ROW, ui_menu_start_text, (UINT8)sizeof(ui_menu_start_text), UI_ATTR_DEFAULT);

    // CHANGE section
    clear_row(UI_MENU_CHANGE_ROW);
    set_ui_tile(UI_MENU_CURSOR_COLUMN, UI_MENU_CHANGE_ROW, change_cursor_tile, UI_ATTR_DEFAULT);
    set_ui_text(UI_MENU_TEXT_COLUMN, UI_MENU_CHANGE_ROW, ui_menu_change_text, (UINT8)sizeof(ui_menu_change_text), UI_ATTR_DEFAULT);
}

void render_score(UINT8 score[3]) {
    // If score is 1M, show NICE!!
    if (score[2] > 99U) {
        for (UINT8 i = 0; i < UI_SCORE_DIGITS; ++i) {
            UINT8 column = UI_SCORE_COLUMN + (UI_SCORE_DIGITS - 1U - i);
            UINT8 tile = ui_tile_overflow[UI_SCORE_DIGITS - 1U - i];
            set_ui_tile(column, UI_SCORE_ROW, tile, UI_ATTR_DEFAULT);
        }
        return;
    }

    // Score display per digit
    for (UINT8 i = 0; i < UI_SCORE_DIGITS; ++i) {
        UINT8 column = UI_SCORE_COLUMN + (UI_SCORE_DIGITS - 1U - i);
        UINT8 tile;
        if (i == 0U) {
            tile = get_first_digit(score[0]) + UI_TILE_DIGIT_START;
        } else if (i == 1U) {
            tile = get_second_digit(score[0]) + UI_TILE_DIGIT_START;
        } else if (i == 2U) {
            tile = get_first_digit(score[1]) + UI_TILE_DIGIT_START;
        } else if (i == 3U) {
            tile = get_second_digit(score[1]) + UI_TILE_DIGIT_START;
        } else if (i == 4U) {
            tile = get_first_digit(score[2]) + UI_TILE_DIGIT_START;
        } else { // i == 5U
            tile = get_second_digit(score[2]) + UI_TILE_DIGIT_START;
        }
        set_ui_tile(column, UI_SCORE_ROW, tile, UI_ATTR_DEFAULT);
    }
}

void render_lives(UINT8 lives) {
    for (UINT8 i = 0; i < UI_MAX_LIVES; ++i) {
        if (i < lives) {
            set_ui_tile(UI_LIVES_COLUMN + i, UI_LIVES_ROW, UI_TILE_HEART, UI_ATTR_LIFE);
        } else {
            set_ui_tile(UI_LIVES_COLUMN + i, UI_LIVES_ROW, UI_TILE_BLANK, UI_ATTR_DEFAULT);
        }
    }
}

void render_gameover(void) {
    // GAME
    set_ui_text(UI_GAMEOVER_COLUMN, UI_GAMEOVER_TOP_ROW, ui_gameover_top_text, (UINT8)sizeof(ui_gameover_top_text), UI_ATTR_DEFAULT);

    // OVER
    set_ui_text(UI_GAMEOVER_COLUMN, UI_GAMEOVER_BOTTOM_ROW, ui_gameover_bottom_text, (UINT8)sizeof(ui_gameover_bottom_text), UI_ATTR_DEFAULT);
}