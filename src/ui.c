#include <gb/gb.h>
#include <gb/cgb.h>
#include "ui.h"

static const UINT8 ui_tile_overflow[UI_SCORE_DIGITS] = {
    UI_TILE_TEXT_N,
    UI_TILE_TEXT_I,
    UI_TILE_TEXT_C,
    UI_TILE_TEXT_E,
    UI_TILE_TEXT_EXCL,
    UI_TILE_TEXT_EXCL
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

static UINT8 get_first_digit(UINT8 num) {
    return num % 10U;
}

static UINT8 get_second_digit(UINT8 num) {
    return num / 10U;
}

void init_ui(void) {
    fill_win_rect(0U, 0U, 20U, 18U, UI_TILE_BLANK);
    move_win(UI_WINDOW_X, UI_WINDOW_Y);
    clear_row(UI_SCORE_ROW);
    clear_row(UI_LIVES_ROW);
}

void render_score(UINT8 score[3]) {
    if (score[2] > 99U) {
        for (UINT8 i = 0; i < UI_SCORE_DIGITS; ++i) {
            UINT8 column = UI_SCORE_RIGHT_COLUMN - i;
            UINT8 tile = ui_tile_overflow[UI_SCORE_DIGITS - 1U - i];
            set_ui_tile(column, UI_SCORE_ROW, tile, UI_ATTR_DEFAULT);
        }
        return;
    }

    for (UINT8 i = 0; i < UI_SCORE_DIGITS; ++i) {
        UINT8 column = UI_SCORE_RIGHT_COLUMN - i;
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