#include <gb/gb.h>
#include <gb/cgb.h>
#include "main.h"
#include "background.h"
#include "sprites.h"
#include "utils.h"
#include "ui.h"

#define SCREEN_BOTTOM 160U

#define PLAY_AREA_WIDTH 54U
#define PLAY_AREA_MIN_X 24U
#define PLAY_AREA_MAX_X (PLAY_AREA_MIN_X + PLAY_AREA_WIDTH)

#define PLAYER_START_X ((PLAY_AREA_MAX_X - PLAY_AREA_MIN_X) / 2 + PLAY_AREA_MIN_X)
#define PLAYER_START_Y 32U
#define PLAYER_INITIAL_LIFE 3U

#define LEAF1_START_X (PLAY_AREA_MIN_X + 8)
#define LEAF1_START_Y 160U
#define LEAF2_START_X ((PLAY_AREA_MIN_X + PLAY_AREA_MAX_X) / 2)
#define LEAF2_START_Y 0U
#define LEAF3_START_X (PLAY_AREA_MAX_X - 8)
#define LEAF3_START_Y 0U

// Game state
unsigned char scene_mode = 0; // 0: title, 1: gameplay, 2: gameover
BOOLEAN dog_mode = FALSE;
UINT8 frame_counter = 0; // counts from 0 to 179
BOOLEAN is_first_frame_count = TRUE;
UINT8 score[3] = {0, 0, 0};

// Title screen state
unsigned char cursor_pos = 0; // 0: Start, 1: Change

// Player state
UINT8 player_life = PLAYER_INITIAL_LIFE;
BOOLEAN player_flip = FALSE;

UINT8 player_pos[2] = {PLAYER_START_X, PLAYER_START_Y};
UINT8 leaves_pos[3][2] = {
    {LEAF1_START_X, LEAF1_START_Y},
    {LEAF2_START_X, LEAF2_START_Y},
    {LEAF3_START_X, LEAF3_START_Y}
};
UINT8 apple_bomb_pos[2] = {0, 0};

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
        if (player_pos[0] < PLAY_AREA_MAX_X) {
            player_pos[0]++;
            player_flip = TRUE;
        }
    }
    if (controller & J_LEFT) {
        if (player_pos[0] > PLAY_AREA_MIN_X) {
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
            leaves_pos[0][0] = uint8_random(PLAY_AREA_MIN_X, PLAY_AREA_MAX_X);
        }
        leaves_pos[0][1] = SCREEN_BOTTOM;
    } else if (frame_counter == 60) {
        if (!is_first_frame_count) {
            leaves_pos[1][0] = uint8_random(PLAY_AREA_MIN_X, PLAY_AREA_MAX_X);
        }
        leaves_pos[1][1] = SCREEN_BOTTOM;
    } else if (frame_counter == 120) {
        if (!is_first_frame_count) {
            leaves_pos[2][0] = uint8_random(PLAY_AREA_MIN_X, PLAY_AREA_MAX_X);
        }
        leaves_pos[2][1] = SCREEN_BOTTOM;
    }
}

void summon_apple_bomb(BOOLEAN is_bomb) {
    if (is_bomb) {
        // Bomb
        set_sprite_tile(SpriteNumAppleBombTopLeft, 12);
        set_sprite_tile(SpriteNumAppleBombTopRight, 14);
        set_sprite_tile(SpriteNumAppleBombBottomLeft, 13);
        set_sprite_tile(SpriteNumAppleBombBottomRight, 15);
        set_sprite_prop(SpriteNumAppleBombTopLeft, SpritesCGB12);
        set_sprite_prop(SpriteNumAppleBombTopRight, SpritesCGB14);
        set_sprite_prop(SpriteNumAppleBombBottomLeft, SpritesCGB13);
        set_sprite_prop(SpriteNumAppleBombBottomRight, SpritesCGB15);
    } else {
        // Apple
        set_sprite_tile(SpriteNumAppleBombTopLeft, 8);
        set_sprite_tile(SpriteNumAppleBombTopRight, 10);
        set_sprite_tile(SpriteNumAppleBombBottomLeft, 9);
        set_sprite_tile(SpriteNumAppleBombBottomRight, 11);
        set_sprite_prop(SpriteNumAppleBombTopLeft, SpritesCGB8);
        set_sprite_prop(SpriteNumAppleBombTopRight, SpritesCGB10);
        set_sprite_prop(SpriteNumAppleBombBottomLeft, SpritesCGB9);
        set_sprite_prop(SpriteNumAppleBombBottomRight, SpritesCGB11);
    }
    apple_bomb_pos[0] = uint8_random(PLAY_AREA_MIN_X, PLAY_AREA_MAX_X);
    apple_bomb_pos[1] = SCREEN_BOTTOM;
}

void render_apple_bomb(void) {
    move_sprite(SpriteNumAppleBombTopLeft, apple_bomb_pos[0], apple_bomb_pos[1]);
    move_sprite(SpriteNumAppleBombTopRight, apple_bomb_pos[0] + 8, apple_bomb_pos[1]);
    move_sprite(SpriteNumAppleBombBottomLeft, apple_bomb_pos[0], apple_bomb_pos[1] + 8);
    move_sprite(SpriteNumAppleBombBottomRight, apple_bomb_pos[0] + 8, apple_bomb_pos[1] + 8);
}

void apple_bomb_scroll(void) {
    // Ignore first 10 points
    if (score[0] < 10 && score[1] <= 0 && score[2] <= 0) {
        return;
    }

    // Summon apple or bomb
    if (score[0] % 10 == 0 ) {
        summon_apple_bomb(FALSE);
    } else if (score[0] % 10 == 5) {
        summon_apple_bomb(TRUE);
    }

    // Gravity
    if (apple_bomb_pos[1] > 0) {
        apple_bomb_pos[1]--;
    }
}

void score_counter(void) {
    if (is_first_frame_count && frame_counter < 150) {
        return;
    }

    if (frame_counter % 60 == 30) {
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

/* ====== Scene Manager ====== */

void show_title_screen(void) {
    scene_mode = 0;
    cursor_pos = 0;
}

void update_title_screen(void) {
    UINT8 controller = joypad();
    if (controller & J_UP && controller & J_DOWN) {
        return;
    }

    // Move cursor
    if ((controller & J_SELECT) || (controller & J_UP) || (controller & J_DOWN)) {
        cursor_pos = 1 - cursor_pos;
    }

    // Select option
    if (controller & J_START || controller & J_A || controller & J_B) {
        if (cursor_pos == 0) {
            show_gameplay_screen();
        } else {
            dog_mode = !dog_mode;
        }
    }
}

void show_gameplay_screen(void) {
    scene_mode = 1;
    frame_counter = 0;
    is_first_frame_count = TRUE;
    score[0] = 0;
    score[1] = 0;
    score[2] = 0;
    player_life = PLAYER_INITIAL_LIFE;
    player_flip = FALSE;
    player_pos[0] = PLAYER_START_X;
    player_pos[1] = PLAYER_START_Y;
    leaves_pos[0][0] = LEAF1_START_X;
    leaves_pos[0][1] = LEAF1_START_Y;
    leaves_pos[1][0] = LEAF2_START_X;
    leaves_pos[1][1] = LEAF2_START_Y;
    leaves_pos[2][0] = LEAF3_START_X;
    leaves_pos[2][1] = LEAF3_START_Y;
}

void update_gameplay_screen(void) {
    // Check game over
    if (player_life <= 0) {
        show_gameover_screen();
        return;
    }

    if (frame_counter < 179) {
        frame_counter++;
    } else {
        frame_counter = 0;
        is_first_frame_count = FALSE;
    }

    player_control();
    leaves_scroll();
    apple_bomb_scroll();
    score_counter();
}

void show_gameover_screen(void) {
    scene_mode = 2;
    render_score(score);
    // Hide leaves
    leaves_pos[0][1] = SCREEN_BOTTOM;
    leaves_pos[1][1] = SCREEN_BOTTOM;
    leaves_pos[2][1] = SCREEN_BOTTOM;
    render_leaves();
    // Hide apple/bomb
    apple_bomb_pos[1] = SCREEN_BOTTOM;
    render_apple_bomb();
}

void update_gameover_screen(void) {
    // Fall down player
    if (player_pos[1] < SCREEN_BOTTOM) {
        player_pos[1] += 2;
    } else {
        player_pos[1] = SCREEN_BOTTOM;
    }
    render_player();

    // Wait for input to restart
    if (joypad() & J_START || joypad() & J_A || joypad() & J_B) {
        show_title_screen();
    }
}

void main(void) {
    set_bkg_palette(0, 6, &BackgroundPalette[0]);
    set_sprite_data(0, 65, Sprites);
    set_sprite_palette(0, 8, SpritePalette);
    init_sprites();
    show_gameplay_screen();

    DISPLAY_ON;
    SHOW_SPRITES;
    SHOW_BKG;

    while (TRUE) {
        if (scene_mode == 0) {
            // Title screen
            update_title_screen();
        } else if (scene_mode == 1) {
            // Gaming
            update_gameplay_screen();
        } else if (scene_mode == 2) {
            // Game over
            update_gameover_screen();
        }

        render_player();
        render_leaves();
        render_apple_bomb();
        wait_vbl_done();
    }
}
