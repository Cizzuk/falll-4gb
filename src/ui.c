#include <gb/gb.h>
#include "ui.h"
#include "sprites.h"

#define UI_SCORE_POS_X 152U
#define UI_SCORE_POS_Y 32U
#define UI_LIVES_POS_X 152U
#define UI_LIVES_POS_Y 40U

UINT8 get_first_digit(UINT8 num) {
    return num % 10;
}
UINT8 get_second_digit(UINT8 num) {
    return num / 10;
}

void render_score(UINT8 score[3]) {
    // Max score is 999999
    if (score[2] > 99) {
        for (UINT8 i = 0; i < 6; i++) {
            set_sprite_tile(SpriteNumScoreDigit0 + i, SpriteTextNICE[5 - i]);
        }
        return;
    }
    
    // First digit
    set_sprite_tile(SpriteNumScoreDigit0, SpriteNumbers[get_first_digit(score[0])]);
    move_sprite(SpriteNumScoreDigit0, UI_SCORE_POS_X, UI_SCORE_POS_Y);
    // Second digit
    set_sprite_tile(SpriteNumScoreDigit1, SpriteNumbers[get_second_digit(score[0])]);
    move_sprite(SpriteNumScoreDigit1, UI_SCORE_POS_X - 8U, UI_SCORE_POS_Y);
    // Third digit
    set_sprite_tile(SpriteNumScoreDigit2, SpriteNumbers[get_first_digit(score[1])]);
    move_sprite(SpriteNumScoreDigit2, UI_SCORE_POS_X - 16U, UI_SCORE_POS_Y);
    // Fourth digit
    set_sprite_tile(SpriteNumScoreDigit3, SpriteNumbers[get_second_digit(score[1])]);
    move_sprite(SpriteNumScoreDigit3, UI_SCORE_POS_X - 24U, UI_SCORE_POS_Y);
    // Fifth digit
    set_sprite_tile(SpriteNumScoreDigit4, SpriteNumbers[get_first_digit(score[2])]);
    move_sprite(SpriteNumScoreDigit4, UI_SCORE_POS_X - 32U, UI_SCORE_POS_Y);
    // Sixth digit
    set_sprite_tile(SpriteNumScoreDigit5, SpriteNumbers[get_second_digit(score[2])]);
    move_sprite(SpriteNumScoreDigit5, UI_SCORE_POS_X - 40U, UI_SCORE_POS_Y);
}

void render_lives(UINT8 lives) {
    // Max lives is 3
    if (lives > 3) {
        lives = 3;
    }
    
    if (lives >= 1) {
        move_sprite(SpriteNumHeart1, UI_LIVES_POS_X, UI_LIVES_POS_Y);
        set_sprite_tile(SpriteNumHeart1, SpriteHeart);
        set_sprite_prop(SpriteNumHeart1, SpritesCGB16);
    } else {
        move_sprite(SpriteNumHeart1, 0, 0);
    }
    if (lives >= 2) {
        move_sprite(SpriteNumHeart2, UI_LIVES_POS_X - 8U, UI_LIVES_POS_Y);
        set_sprite_tile(SpriteNumHeart2, SpriteHeart);
        set_sprite_prop(SpriteNumHeart2, SpritesCGB16);
    } else {
        move_sprite(SpriteNumHeart2, 0, 0);
    }
    if (lives >= 3) {
        move_sprite(SpriteNumHeart3, UI_LIVES_POS_X - 16U, UI_LIVES_POS_Y);
        set_sprite_tile(SpriteNumHeart3, SpriteHeart);
        set_sprite_prop(SpriteNumHeart3, SpritesCGB16);
    } else {
        move_sprite(SpriteNumHeart3, 0, 0);
    }
}