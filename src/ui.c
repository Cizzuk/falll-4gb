#include <gb/gb.h>
#include "sprites.h"

const UINT8 score_pos[2] = {152, 32}; // X: right aligned
const UINT8 lives_pos[2] = {152, 40}; // X: right aligned

unsigned char get_first_digit(UINT8 num) {
    return num % 10;
}
unsigned char get_second_digit(UINT8 num) {
    return num / 10;
}

void render_score(UINT8 score[3]) {
    // score[0]: 0-99, score[1]: 100-9999, score[2]: 10000-999999
    if (score[2] > 99) {
        for (UINT8 i = 0; i < 6; i++) {
            set_sprite_tile(SpriteNumScoreDigit0 + i, SpriteTextNICE[i]);
            move_sprite(SpriteNumScoreDigit0 + i, score_pos[0] - 40 + (i * 8), score_pos[1]);
        }
        return;
    }
    // First digit
    set_sprite_tile(SpriteNumScoreDigit0, SpriteNumbers[get_first_digit(score[0])]);
    move_sprite(SpriteNumScoreDigit0, score_pos[0], score_pos[1]);
    // Second digit
    set_sprite_tile(SpriteNumScoreDigit1, SpriteNumbers[get_second_digit(score[0])]);
    move_sprite(SpriteNumScoreDigit1, score_pos[0] - 8, score_pos[1]);
    // Third digit
    set_sprite_tile(SpriteNumScoreDigit2, SpriteNumbers[get_first_digit(score[1])]);
    move_sprite(SpriteNumScoreDigit2, score_pos[0] - 16, score_pos[1]);
    // Fourth digit
    set_sprite_tile(SpriteNumScoreDigit3, SpriteNumbers[get_second_digit(score[1])]);
    move_sprite(SpriteNumScoreDigit3, score_pos[0] - 24, score_pos[1]);
    // Fifth digit
    set_sprite_tile(SpriteNumScoreDigit4, SpriteNumbers[get_first_digit(score[2])]);
    move_sprite(SpriteNumScoreDigit4, score_pos[0] - 32, score_pos[1]);
    // Sixth digit
    set_sprite_tile(SpriteNumScoreDigit5, SpriteNumbers[get_second_digit(score[2])]);
    move_sprite(SpriteNumScoreDigit5, score_pos[0] - 40, score_pos[1]);
}
