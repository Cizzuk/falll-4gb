#include <gb/gb.h>
#include <gb/cgb.h>
#include "main.h"
#include "background.h"
#include "sprites.h"
#include "world.h"
#include "utils.h"
#include "ui.h"

// Game state
UINT8 scene_mode = 0; // 0: title, 1: gameplay, 2: gameover
BOOLEAN dog_mode = FALSE;
UINT8 frame_counter = 0; // counts from 0 to 179
BOOLEAN is_first_frame_count = TRUE;
UINT8 score[3] = {0, 0, 0};

// Title screen state
UINT8 cursor_pos = 0; // 0: Start, 1: Change

// Player state
UINT8 player_life = PLAYER_INITIAL_LIFE;
BOOLEAN player_flip = FALSE;

UINT8 player_pos[2] = {PLAYER_START_X, PLAYER_START_Y};
UINT8 leaves_pos[3][2] = {
    {LEAF1_START_X, LEAF1_START_Y},
    {LEAF2_START_X, LEAF2_START_Y},
    {LEAF3_START_X, LEAF3_START_Y}
};
UINT8 apple_bomb_pos[2] = {APPLE_BOMB_START_X, APPLE_BOMB_START_Y};
UINT8 is_bomb = FALSE;
UINT8 background_scroll_y = 0U;

void init_vram(void) {
    // Load background data
    set_bkg_data(0, 64, Background);
    set_bkg_palette(0, 8, BackgroundPalette);
    VBK_REG = 0;
    set_bkg_tiles(0, 0, WorldWidth, WorldHeight, World);
    VBK_REG = 1;
    for (UINT8 row = 0; row < WorldHeight; ++row) {
        set_bkg_tiles(0, row, WorldWidth, 1, WorldPalette);
    }
    VBK_REG = 0;

    // Load sprite data
    set_sprite_data(0, 22, Sprites);
    set_sprite_palette(0, 8, SpritePalette);
}

// Refresh game variables
void init_game(void) {
    frame_counter = 0;
    is_first_frame_count = TRUE;
    score[0] = 0;
    score[1] = 0;
    score[2] = 0;
    cursor_pos = 0;
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
    apple_bomb_pos[0] = APPLE_BOMB_START_X;
    apple_bomb_pos[1] = APPLE_BOMB_START_Y;
    is_bomb = FALSE;
    background_scroll_y = 0U;
    move_bkg(0U, 0U);
    render_player();
    render_leaves();
    render_apple_bomb();
    render_score(score);
    render_lives(player_life);
}

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

    render_player();
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
    UINT8 speed = 2;

    if (leaves_pos[0][1] > 0) {
        leaves_pos[0][1] -= speed;
    }
    if (leaves_pos[1][1] > 0) {
        leaves_pos[1][1] -= speed;
    }
    if (leaves_pos[2][1] > 0) {
        leaves_pos[2][1] -= speed;
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

void summon_apple_bomb(void) {
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
    if (frame_counter % 60 == 30) {
        if (score[0] % 10 == 0) {
            is_bomb = FALSE;
            summon_apple_bomb();
        } else if (score[0] % 10 == 5) {
            is_bomb = TRUE;
            summon_apple_bomb();
        }
    }

    // Gravity
    if (frame_counter % 2 == 0 && apple_bomb_pos[1] > 1) {
        apple_bomb_pos[1] -= 2;
    } else if (apple_bomb_pos[1] > 0) {
        apple_bomb_pos[1] -= 1;
    }
}

void background_scroll(void) {
    // Here it overflows and reset to 0 automatically
    background_scroll_y++;
    move_bkg(0U, background_scroll_y);
}

void score_counter(void) {
    if (is_first_frame_count && frame_counter < 60) {
        return;
    }

    if (frame_counter % 60 == 40) {
        score[0]++;
        if (score[0] > 99) {
            score[0] = 0;
            score[1]++;
            if (score[1] > 99) {
                score[1] = 0;
                score[2]++;
                if (score[2] > 99) {
                    // Limit Score
                    score[2] = 100;
                }
            }
        }
        render_score(score);
    }
}

// Colliders
void update_colliders(void) {
    UINT8 player_hitbox_width;
    UINT8 player_hitbox_height;
    UINT8 player_margin_left;
    UINT8 player_margin_right;
    UINT8 player_margin_top;
    UINT8 player_margin_bottom;
    UINT8 player_left;
    UINT8 player_right;
    UINT8 player_top;
    UINT8 player_bottom;

    // Update player Hitbox
    if (dog_mode) {
        player_hitbox_width = DOG_HITBOX_WIDTH;
        player_hitbox_height = DOG_HITBOX_HEIGHT;
        player_margin_left = DOG_MARGIN_LEFT;
        player_margin_right = DOG_MARGIN_RIGHT;
        player_margin_top = DOG_MARGIN_TOP;
        player_margin_bottom = DOG_MARGIN_BOTTOM;
    } else {
        player_hitbox_width = PLAYER_HITBOX_WIDTH;
        player_hitbox_height = PLAYER_HITBOX_HEIGHT;
        player_margin_left = PLAYER_MARGIN_LEFT;
        player_margin_right = PLAYER_MARGIN_RIGHT;
        player_margin_top = PLAYER_MARGIN_TOP;
        player_margin_bottom = PLAYER_MARGIN_BOTTOM;
    }

    player_left = (player_pos[0] + player_margin_left);
    player_right = (player_pos[0] + player_hitbox_width - 1 - player_margin_right);
    player_top = (player_pos[1] + player_margin_top);
    player_bottom = (player_pos[1] + player_hitbox_height - 1 - player_margin_bottom);

    // Check leaves
    for (UINT8 i = 0; i < 3; i++) {
    UINT8 leaf_left = (leaves_pos[i][0] + LEAF_MARGIN);
    UINT8 leaf_right = (leaves_pos[i][0] + LEAF_HITBOX_WIDTH - 1 - LEAF_MARGIN);
    UINT8 leaf_top = (leaves_pos[i][1] + LEAF_MARGIN);
    UINT8 leaf_bottom = (leaves_pos[i][1] + LEAF_HITBOX_HEIGHT - 1 - LEAF_MARGIN);

        if (check_collision(player_left, player_top, player_right, player_bottom,
                            leaf_left, leaf_top, leaf_right, leaf_bottom)) {
            leaves_pos[i][1] = 0; // Hide leaf
            if (player_life > 0) {
                player_life--;
                render_lives(player_life);
            }
        }
    }

    // Check apple/bomb
    {
    UINT8 apple_left = (apple_bomb_pos[0] + APPLE_BOMB_MARGIN_LEFT);
    UINT8 apple_right = (apple_bomb_pos[0] + APPLE_BOMB_HITBOX_WIDTH - 1 - APPLE_BOMB_MARGIN_RIGHT);
    UINT8 apple_top = (apple_bomb_pos[1] + APPLE_BOMB_MARGIN_TOP);
    UINT8 apple_bottom = (apple_bomb_pos[1] + APPLE_BOMB_HITBOX_HEIGHT - 1 - APPLE_BOMB_MARGIN_BOTTOM);

        if (check_collision(player_left, player_top, player_right, player_bottom,
                            apple_left, apple_top, apple_right, apple_bottom)) {
            apple_bomb_pos[1] = 0; // Hide apple/bomb
            if (is_bomb) {
                player_life = 0;
            } else {
                player_life++;
                if (player_life > PLAYER_INITIAL_LIFE) {
                    player_life = PLAYER_INITIAL_LIFE;
                }
            }
            render_lives(player_life);
        }
    }
}

/* ====== Scene Manager ====== */

void show_title_screen(void) {
    scene_mode = 0;
    cursor_pos = 0;
    init_game();
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
    init_game();
    render_score(score);
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
    render_leaves();
    render_apple_bomb();
    background_scroll();
    update_colliders();
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
    // Wait for input to restart
    if (joypad() & J_START || joypad() & J_A || joypad() & J_B) {
        show_title_screen();
        return;
    }

    // Fall down player
    if (player_pos[1] < SCREEN_BOTTOM) {
        player_pos[1] += 2;
    } else {
        player_pos[1] = SCREEN_BOTTOM;
    }
    render_player();
}

void main(void) {
    // Initialize
    init_vram();
    init_ui();
    init_sprites();
    show_gameplay_screen();

    // Show All
    SHOW_BKG;
    SHOW_WIN;
    SHOW_SPRITES;
    DISPLAY_ON;

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

        wait_vbl_done();
    }
}
