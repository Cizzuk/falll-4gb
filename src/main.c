#include <gb/gb.h>
#include <gb/cgb.h>
#include "background.h"
#include "sprites.h"
#include "utils.h"
#include "ui.h"

// Game state
BOOLEAN is_gaming = TRUE;
UINT8 frame_counter = 0; // counts from 0 to 179
BOOLEAN is_first_frame_count = TRUE;
UINT8 score[3] = {0, 0, 0};

// Player state
UINT8 player_life = 3;
BOOLEAN player_flip = FALSE;

// screen has dead zones, left: 8px, top: 16px
UINT8 player_pos[2] = {63, 32}; // X: 36 - 90
UINT8 leaves_pos[3][2] = {{43, 160}, {63, 0}, {83, 0}};

void init_sprites(void) {
    // Player
    set_sprite_tile(SpriteNumPlayerTopLeft, 0);
    set_sprite_tile(SpriteNumPlayerTopRight, 2);
    set_sprite_tile(SpriteNumPlayerBottomLeft, 1);
    set_sprite_tile(SpriteNumPlayerBottomRight, 3);
    set_sprite_prop(SpriteNumPlayerTopLeft, SpritesCGB0);
    set_sprite_prop(SpriteNumPlayerTopRight, SpritesCGB2);
    set_sprite_prop(SpriteNumPlayerBottomLeft, SpritesCGB1);
    set_sprite_prop(SpriteNumPlayerBottomRight, SpritesCGB3);

    // Leaves
    // Leaf 1
    set_sprite_tile(SpriteNumLeaf1TopLeft, 6);
    set_sprite_tile(SpriteNumLeaf1TopRight, 4);
    set_sprite_tile(SpriteNumLeaf1BottomLeft, 7);
    set_sprite_tile(SpriteNumLeaf1BottomRight, 5);
    set_sprite_prop(SpriteNumLeaf1TopLeft, S_FLIPX | SpritesCGB4);
    set_sprite_prop(SpriteNumLeaf1TopRight, S_FLIPX | SpritesCGB5);
    set_sprite_prop(SpriteNumLeaf1BottomLeft, S_FLIPX | SpritesCGB6);
    set_sprite_prop(SpriteNumLeaf1BottomRight, S_FLIPX | SpritesCGB7);
    // Leaf 2
    set_sprite_tile(SpriteNumLeaf2TopLeft, 5);
    set_sprite_tile(SpriteNumLeaf2TopRight, 7);
    set_sprite_tile(SpriteNumLeaf2BottomLeft, 4);
    set_sprite_tile(SpriteNumLeaf2BottomRight, 6);
    set_sprite_prop(SpriteNumLeaf2TopLeft, S_FLIPY | SpritesCGB5);
    set_sprite_prop(SpriteNumLeaf2TopRight, S_FLIPY | SpritesCGB7);
    set_sprite_prop(SpriteNumLeaf2BottomLeft, S_FLIPY | SpritesCGB6);
    set_sprite_prop(SpriteNumLeaf2BottomRight, S_FLIPY | SpritesCGB4);
    // Leaf 3
    set_sprite_tile(SpriteNumLeaf3TopLeft, 4);
    set_sprite_tile(SpriteNumLeaf3TopRight, 6);
    set_sprite_tile(SpriteNumLeaf3BottomLeft, 5);
    set_sprite_tile(SpriteNumLeaf3BottomRight, 7);
    set_sprite_prop(SpriteNumLeaf3TopLeft, SpritesCGB4);
    set_sprite_prop(SpriteNumLeaf3TopRight, SpritesCGB6);
    set_sprite_prop(SpriteNumLeaf3BottomLeft, SpritesCGB5);
    set_sprite_prop(SpriteNumLeaf3BottomRight, SpritesCGB7);

    // UI
    render_score(score);
}

void render_player(void) {
    if (player_flip) {
        set_sprite_prop(SpriteNumPlayerTopLeft, S_FLIPX | SpritesCGB0);
        set_sprite_prop(SpriteNumPlayerTopRight, S_FLIPX | SpritesCGB2);
        set_sprite_prop(SpriteNumPlayerBottomLeft, S_FLIPX | SpritesCGB1);
        set_sprite_prop(SpriteNumPlayerBottomRight, S_FLIPX | SpritesCGB3);
        move_sprite(SpriteNumPlayerTopLeft, player_pos[0] + 8, player_pos[1]);
        move_sprite(SpriteNumPlayerTopRight, player_pos[0], player_pos[1]);
        move_sprite(SpriteNumPlayerBottomLeft, player_pos[0] + 8, player_pos[1] + 8);
        move_sprite(SpriteNumPlayerBottomRight, player_pos[0], player_pos[1] + 8);
    } else {
        set_sprite_prop(SpriteNumPlayerTopLeft, SpritesCGB0);
        set_sprite_prop(SpriteNumPlayerTopRight, SpritesCGB2);
        set_sprite_prop(SpriteNumPlayerBottomLeft, SpritesCGB1);
        set_sprite_prop(SpriteNumPlayerBottomRight, SpritesCGB3);
        move_sprite(SpriteNumPlayerTopLeft, player_pos[0], player_pos[1]);
        move_sprite(SpriteNumPlayerTopRight, player_pos[0] + 8, player_pos[1]);
        move_sprite(SpriteNumPlayerBottomLeft, player_pos[0], player_pos[1] + 8);
        move_sprite(SpriteNumPlayerBottomRight, player_pos[0] + 8, player_pos[1] + 8);
    }
}

void player_control(void) {
    UINT8 controller = joypad();
    if (controller & J_RIGHT && controller & J_LEFT) {
        return;
    }

    // Slow down
    if (controller & J_A || controller & J_B) {
        if (frame_counter % 2 == 0) {
            return;
        }
    }

    if (controller & J_RIGHT) {
        if (player_pos[0] < 90) {
            player_pos[0]++;
            player_flip = TRUE;
        }
    }
    if (controller & J_LEFT) {
        if (player_pos[0] > 36) {
            player_pos[0]--;
            player_flip = FALSE;
        }
    }
}

void render_leaves(void) {
    // Leaf 1
    move_sprite(SpriteNumLeaf1TopLeft, leaves_pos[0][0], leaves_pos[0][1]);
    move_sprite(SpriteNumLeaf1TopRight, leaves_pos[0][0] + 8, leaves_pos[0][1]);
    move_sprite(SpriteNumLeaf1BottomLeft, leaves_pos[0][0], leaves_pos[0][1] + 8);
    move_sprite(SpriteNumLeaf1BottomRight, leaves_pos[0][0] + 8, leaves_pos[0][1] + 8);
    // Leaf 2
    move_sprite(SpriteNumLeaf2TopLeft, leaves_pos[1][0], leaves_pos[1][1]);
    move_sprite(SpriteNumLeaf2TopRight, leaves_pos[1][0] + 8, leaves_pos[1][1]);
    move_sprite(SpriteNumLeaf2BottomLeft, leaves_pos[1][0], leaves_pos[1][1] + 8);
    move_sprite(SpriteNumLeaf2BottomRight, leaves_pos[1][0] + 8, leaves_pos[1][1] + 8);
    // Leaf 3
    move_sprite(SpriteNumLeaf3TopLeft, leaves_pos[2][0], leaves_pos[2][1]);
    move_sprite(SpriteNumLeaf3TopRight, leaves_pos[2][0] + 8, leaves_pos[2][1]);
    move_sprite(SpriteNumLeaf3BottomLeft, leaves_pos[2][0], leaves_pos[2][1] + 8);
    move_sprite(SpriteNumLeaf3BottomRight, leaves_pos[2][0] + 8, leaves_pos[2][1] + 8);
}

void leaves_scroll(void) {
    if (leaves_pos[0][1] > 0) {
        leaves_pos[0][1]--;
    }
    if (leaves_pos[1][1] > 0) {
        leaves_pos[1][1]--;
    }
    if (leaves_pos[2][1] > 0) {
        leaves_pos[2][1]--;
    }

    if (frame_counter == 0) {
        if (!is_first_frame_count) {
            leaves_pos[0][0] = uint8_random(36, 90);
        }
        leaves_pos[0][1] = 160;
    } else if (frame_counter == 60) {
        if (!is_first_frame_count) {
            leaves_pos[1][0] = uint8_random(36, 90);
        }
        leaves_pos[1][1] = 160;
    } else if (frame_counter == 120) {
        if (!is_first_frame_count) {
            leaves_pos[2][0] = uint8_random(36, 90);
        }
        leaves_pos[2][1] = 160;
    }
}

void score_counter(void) {
    if (is_first_frame_count && frame_counter < 170) {
        return;
    }

    if (frame_counter % 60 == 50) {
        score[0]++;
        if (score[0] > 99) {
            score[0] = 0;
            score[1]++;
            if (score[1] > 99) {
                score[1] = 0;
                score[2]++;
                if (score[2] > 99) {
                    score[2] = 100;
                }
            }
        }
        render_score(score);
    }
}

void reset_game(void) {
    is_gaming = TRUE;
    frame_counter = 0;
    is_first_frame_count = TRUE;
    score[0] = 0;
    score[1] = 0;
    score[2] = 0;
    player_life = 3;
    player_flip = FALSE;
    player_pos[0] = 55;
    player_pos[1] = 32;
    leaves_pos[0][0] = 43;
    leaves_pos[0][1] = 160;
    leaves_pos[1][0] = 63;
    leaves_pos[1][1] = 0;
    leaves_pos[2][0] = 83;
    leaves_pos[2][1] = 0;
    init_sprites();
}

void main(void) {
    set_bkg_palette(0, 6, &BackgroundPalette[0]);
    set_sprite_data(0, 65, Sprites);
    set_sprite_palette(0, 8, SpritePalette);
    reset_game();

    DISPLAY_ON;
    SHOW_SPRITES;
    SHOW_BKG;

    while (TRUE) {
        if (is_gaming) {
            if (frame_counter < 179) {
                frame_counter++;
            } else {
                frame_counter = 0;
                is_first_frame_count = FALSE;
            }

            player_control();
            leaves_scroll();
            score_counter();
        }

        render_player();
        render_leaves();
        wait_vbl_done();
    }
}
