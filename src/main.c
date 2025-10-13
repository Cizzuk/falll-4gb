#include <gb/gb.h>
#include <gb/cgb.h>
#include <rand.h>
#include "main.h"
#include "background.h"
#include "sprites.h"
#include "world.h"
#include "utils.h"
#include "ui.h"

// Game state
UINT8 rand_timer = 0;
UINT8 rand_controller = 0;
UINT8 prev_controller = 0;
UINT8 scene_mode = 0; // 0: title, 1: gameplay, 2: gameover
BOOLEAN dog_mode = FALSE;
UINT8 frame_counter = 0; // counts from 0 to 179
BOOLEAN is_first_frame_count = TRUE;
UINT8 score[3] = {0, 0, 0};

// Title screen state
BOOLEAN cursor_pos = FALSE; // F: Start, T: Change

// Player state
UINT8 player_life = PLAYER_INITIAL_LIFE;
BOOLEAN player_flip = FALSE;
UINT8 player_pos[2] = {0U, PLAYER_START_Y};
UINT8 player_visual_width = PLAYER_PIXEL_WIDTH;
UINT8 player_visual_height = PLAYER_PIXEL_HEIGHT;
UINT8 player_visual_margin_left = PLAYER_VISUAL_MARGIN_LEFT;
UINT8 player_visual_margin_right = PLAYER_VISUAL_MARGIN_RIGHT;
UINT8 player_visual_margin_top = PLAYER_VISUAL_MARGIN_TOP;
UINT8 player_visual_margin_bottom = PLAYER_VISUAL_MARGIN_BOTTOM;
UINT8 player_hitbox_width = PLAYER_PIXEL_WIDTH;
UINT8 player_hitbox_height = PLAYER_PIXEL_HEIGHT;
UINT8 player_hitbox_margin_left = PLAYER_COLLISION_MARGIN_LEFT;
UINT8 player_hitbox_margin_right = PLAYER_COLLISION_MARGIN_RIGHT;
UINT8 player_hitbox_margin_top = PLAYER_COLLISION_MARGIN_TOP;
UINT8 player_hitbox_margin_bottom = PLAYER_COLLISION_MARGIN_BOTTOM;
UINT8 player_move_min_x = PLAY_BOUND_LEFT - PLAYER_VISUAL_MARGIN_LEFT;
UINT8 player_move_max_x = PLAY_BOUND_RIGHT - (PLAYER_PIXEL_WIDTH - PLAYER_VISUAL_MARGIN_RIGHT);

// Falling items state
UINT8 leaves_pos[3][2] = {
    {LEAF1_START_X, LEAF1_START_Y},
    {LEAF2_START_X, LEAF2_START_Y},
    {LEAF3_START_X, LEAF3_START_Y}
};
UINT8 apple_bomb_pos[2] = {APPLE_BOMB_START_X, APPLE_BOMB_START_Y};
UINT8 is_bomb = FALSE;
UINT8 background_scroll_y = 0U;

void init_vram(void) {
    VBK_REG = 0;
    set_bkg_data(0, 80, Background);
    set_sprite_data(0, 22, Sprites);
    set_bkg_palette(0, 8, BackgroundPalette);
    set_sprite_palette(0, 8, SpritePalette);
    set_bkg_tiles(0, 0, WorldWidth, WorldHeight, World);
    init_bkg_attr();
}

void init_bkg_attr(void) {
    // Set palettes if on CGB hardware
    if (_cpu == CGB_TYPE) {
        VBK_REG = 1;
        // Copy world palette
        UINT8 row_palette[WorldWidth];
        for (UINT8 col = 0; col < WorldWidth; col++) {
            row_palette[col] = WorldPalette[col];
        }

        // Apply palette per row
        for (UINT8 row = 0; row < WorldHeight; row++) {
            // Randomize specific columns
            for (UINT8 i = 0; i < WORLD_ATTR_RANDOM_COL_COUNT; i++) {
                UINT8 random_col = WorldAttrRandomColumns[i];
                row_palette[random_col] = uint8_random(WORLD_ATTR_RANDOM_PALETTE_MIN, WORLD_ATTR_RANDOM_PALETTE_MAX);
            }
            set_bkg_tiles(0, row, WorldWidth, 1, row_palette);
        }
        VBK_REG = 0;
    }
}

// Refresh game variables
void init_game(void) {
    // Game state
    frame_counter = 0;
    is_first_frame_count = TRUE;
    score[0] = 0;
    score[1] = 0;
    score[2] = 0;
    // Player state
    player_life = PLAYER_INITIAL_LIFE;
    init_player();
    // Leaves state
    leaves_pos[0][0] = LEAF1_START_X;
    leaves_pos[0][1] = LEAF1_START_Y;
    leaves_pos[1][0] = LEAF2_START_X;
    leaves_pos[1][1] = LEAF2_START_Y;
    leaves_pos[2][0] = LEAF3_START_X;
    leaves_pos[2][1] = LEAF3_START_Y;
    render_leaves();
    // Apple/Bomb state
    apple_bomb_pos[0] = APPLE_BOMB_START_X;
    apple_bomb_pos[1] = APPLE_BOMB_START_Y;
    is_bomb = FALSE;
    render_apple_bomb();
    // Background state
    background_scroll_y = 0U;
    move_bkg(0U, 0U);
}

void init_sprites(void) {
    init_player();

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
}

void init_player(void) {
    UINT8 field_left;
    UINT8 field_right;
    UINT8 visual_min;
    UINT8 collision_min;
    UINT8 visual_offset;
    UINT8 collision_offset;
    UINT8 visual_max;
    UINT8 collision_max;

    player_flip = FALSE;

    if (dog_mode) {
        player_pos[1] = DOG_START_Y;

        player_visual_width = DOG_PIXEL_WIDTH;
        player_visual_height = DOG_PIXEL_HEIGHT;
        player_visual_margin_left = DOG_VISUAL_MARGIN_LEFT;
        player_visual_margin_right = DOG_VISUAL_MARGIN_RIGHT;
        player_visual_margin_top = DOG_VISUAL_MARGIN_TOP;
        player_visual_margin_bottom = DOG_VISUAL_MARGIN_BOTTOM;

        player_hitbox_width = DOG_PIXEL_WIDTH;
        player_hitbox_height = DOG_PIXEL_HEIGHT;
        player_hitbox_margin_left = DOG_COLLISION_MARGIN_LEFT;
        player_hitbox_margin_right = DOG_COLLISION_MARGIN_RIGHT;
        player_hitbox_margin_top = DOG_COLLISION_MARGIN_TOP;
        player_hitbox_margin_bottom = DOG_COLLISION_MARGIN_BOTTOM;

        field_left = PLAY_BOUND_LEFT_DOG;
        field_right = PLAY_BOUND_RIGHT_DOG;

        set_sprite_tile(SpriteNumPlayer0_0, 16);
        set_sprite_tile(SpriteNumPlayer0_1, 19);
        set_sprite_tile(SpriteNumPlayer0_2, 19);
        set_sprite_tile(SpriteNumPlayer0_3, 16);
        set_sprite_tile(SpriteNumPlayer1_0, 17);
        set_sprite_tile(SpriteNumPlayer1_1, 20);
        set_sprite_tile(SpriteNumPlayer1_2, 20);
        set_sprite_tile(SpriteNumPlayer1_3, 17);
        set_sprite_tile(SpriteNumPlayer2_0, 18);
        set_sprite_tile(SpriteNumPlayer2_1, 21);
        set_sprite_tile(SpriteNumPlayer2_2, 21);
        set_sprite_tile(SpriteNumPlayer2_3, 18);
        set_sprite_prop(SpriteNumPlayer0_0, SpritesCGB16);
        set_sprite_prop(SpriteNumPlayer0_1, SpritesCGB19);
        set_sprite_prop(SpriteNumPlayer0_2, S_FLIPX | SpritesCGB19);
        set_sprite_prop(SpriteNumPlayer0_3, S_FLIPX | SpritesCGB16);
        set_sprite_prop(SpriteNumPlayer1_0, SpritesCGB17);
        set_sprite_prop(SpriteNumPlayer1_1, SpritesCGB20);
        set_sprite_prop(SpriteNumPlayer1_2, S_FLIPX | SpritesCGB20);
        set_sprite_prop(SpriteNumPlayer1_3, S_FLIPX | SpritesCGB17);
        set_sprite_prop(SpriteNumPlayer2_0, SpritesCGB18);
        set_sprite_prop(SpriteNumPlayer2_1, SpritesCGB21);
        set_sprite_prop(SpriteNumPlayer2_2, S_FLIPX | SpritesCGB21);
        set_sprite_prop(SpriteNumPlayer2_3, S_FLIPX | SpritesCGB18);
    } else {
        player_pos[1] = PLAYER_START_Y;

        player_visual_width = PLAYER_PIXEL_WIDTH;
        player_visual_height = PLAYER_PIXEL_HEIGHT;
        player_visual_margin_left = PLAYER_VISUAL_MARGIN_LEFT;
        player_visual_margin_right = PLAYER_VISUAL_MARGIN_RIGHT;
        player_visual_margin_top = PLAYER_VISUAL_MARGIN_TOP;
        player_visual_margin_bottom = PLAYER_VISUAL_MARGIN_BOTTOM;

        player_hitbox_width = PLAYER_PIXEL_WIDTH;
        player_hitbox_height = PLAYER_PIXEL_HEIGHT;
        player_hitbox_margin_left = PLAYER_COLLISION_MARGIN_LEFT;
        player_hitbox_margin_right = PLAYER_COLLISION_MARGIN_RIGHT;
        player_hitbox_margin_top = PLAYER_COLLISION_MARGIN_TOP;
        player_hitbox_margin_bottom = PLAYER_COLLISION_MARGIN_BOTTOM;

        field_left = PLAY_BOUND_LEFT;
        field_right = PLAY_BOUND_RIGHT;

        set_sprite_tile(SpriteNumPlayer0_0, 0);
        set_sprite_tile(SpriteNumPlayer0_1, 2);
        set_sprite_tile(SpriteNumPlayer0_2, 1);
        set_sprite_tile(SpriteNumPlayer0_3, 3);
        set_sprite_prop(SpriteNumPlayer0_0, SpritesCGB0);
        set_sprite_prop(SpriteNumPlayer0_1, SpritesCGB2);
        set_sprite_prop(SpriteNumPlayer0_2, SpritesCGB1);
        set_sprite_prop(SpriteNumPlayer0_3, SpritesCGB3);

        // Hide unused sprites
        move_sprite(SpriteNumPlayer1_0, 0, 0);
        move_sprite(SpriteNumPlayer1_1, 0, 0);
        move_sprite(SpriteNumPlayer1_2, 0, 0);
        move_sprite(SpriteNumPlayer1_3, 0, 0);
        move_sprite(SpriteNumPlayer2_0, 0, 0);
        move_sprite(SpriteNumPlayer2_1, 0, 0);
        move_sprite(SpriteNumPlayer2_2, 0, 0);
        move_sprite(SpriteNumPlayer2_3, 0, 0);
    }

    // Calculate movement bounds
    // Left bound
    visual_min = field_left - player_visual_margin_left;
    collision_min = field_left - player_hitbox_margin_left;
    player_move_min_x = (visual_min > collision_min) ? visual_min : collision_min;
    // Right bound
    visual_offset = player_visual_width - player_visual_margin_right;
    collision_offset = player_hitbox_width - player_hitbox_margin_right;
    visual_max = field_right - visual_offset;
    collision_max = field_right - collision_offset;
    player_move_max_x = (visual_max < collision_max) ? visual_max : collision_max;

    // Calculate starting position (centered)
    player_pos[0] = player_move_min_x + ((player_move_max_x - player_move_min_x) / 2U);

    render_player();
}

void render_player(void) {
    if (dog_mode) {
        move_sprite(SpriteNumPlayer0_0, player_pos[0], player_pos[1]);
        move_sprite(SpriteNumPlayer0_1, player_pos[0] + 8, player_pos[1]);
        move_sprite(SpriteNumPlayer0_2, player_pos[0] + 15, player_pos[1]);
        move_sprite(SpriteNumPlayer0_3, player_pos[0] + 23, player_pos[1]);
        move_sprite(SpriteNumPlayer1_0, player_pos[0], player_pos[1] + 8);
        move_sprite(SpriteNumPlayer1_1, player_pos[0] + 8, player_pos[1] + 8);
        move_sprite(SpriteNumPlayer1_2, player_pos[0] + 15, player_pos[1] + 8);
        move_sprite(SpriteNumPlayer1_3, player_pos[0] + 23, player_pos[1] + 8);
        move_sprite(SpriteNumPlayer2_0, player_pos[0], player_pos[1] + 16);
        move_sprite(SpriteNumPlayer2_1, player_pos[0] + 8, player_pos[1] + 16);
        move_sprite(SpriteNumPlayer2_2, player_pos[0] + 15, player_pos[1] + 16);
        move_sprite(SpriteNumPlayer2_3, player_pos[0] + 23, player_pos[1] + 16);
    } else {
        if (player_flip) {
            set_sprite_prop(SpriteNumPlayer0_0, S_FLIPX | SpritesCGB0);
            set_sprite_prop(SpriteNumPlayer0_1, S_FLIPX | SpritesCGB2);
            set_sprite_prop(SpriteNumPlayer0_2, S_FLIPX | SpritesCGB1);
            set_sprite_prop(SpriteNumPlayer0_3, S_FLIPX | SpritesCGB3);
            move_sprite(SpriteNumPlayer0_0, player_pos[0] + 8, player_pos[1]);
            move_sprite(SpriteNumPlayer0_1, player_pos[0], player_pos[1]);
            move_sprite(SpriteNumPlayer0_2, player_pos[0] + 8, player_pos[1] + 8);
            move_sprite(SpriteNumPlayer0_3, player_pos[0], player_pos[1] + 8);
        } else {
            set_sprite_prop(SpriteNumPlayer0_0, SpritesCGB0);
            set_sprite_prop(SpriteNumPlayer0_1, SpritesCGB2);
            set_sprite_prop(SpriteNumPlayer0_2, SpritesCGB1);
            set_sprite_prop(SpriteNumPlayer0_3, SpritesCGB3);
            move_sprite(SpriteNumPlayer0_0, player_pos[0], player_pos[1]);
            move_sprite(SpriteNumPlayer0_1, player_pos[0] + 8, player_pos[1]);
            move_sprite(SpriteNumPlayer0_2, player_pos[0], player_pos[1] + 8);
            move_sprite(SpriteNumPlayer0_3, player_pos[0] + 8, player_pos[1] + 8);
        }
    }
}

void player_control(void) {
    UINT8 controller = joypad();
    if (controller & J_RIGHT && controller & J_LEFT) {
        return;
    }

    BOOLEAN is_slow = (controller & (J_A | J_B));

    if (dog_mode) {
        BOOLEAN is_fast = (!is_slow && frame_counter % 3 == 0);

        if (controller & J_RIGHT) {
            if (player_pos[0] < player_move_max_x) {
                if (is_fast && player_pos[0] < (player_move_max_x - 1)) {
                    player_pos[0] += 2;
                } else {
                    player_pos[0]++;
                }
            }
        }
        if (controller & J_LEFT) {
            if (player_pos[0] > player_move_min_x) {
                if (is_fast && player_pos[0] > (player_move_min_x + 1)) {
                    player_pos[0] -= 2;
                } else {
                    player_pos[0]--;
                }
            }
        }
    } else {
        // Slow down
        if (is_slow) {
            if (frame_counter % 2) {
                return;
            }
        }

        if (controller & J_RIGHT) {
            if (player_pos[0] < player_move_max_x) {
                player_pos[0]++;
                player_flip = TRUE;
            }
        }
        if (controller & J_LEFT) {
            if (player_pos[0] > player_move_min_x) {
                player_pos[0]--;
                player_flip = FALSE;
            }
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

UINT8 leaves_speed_calc(void) {
    // Early returns
    if (score[2] > 0U) {
        return LEAF_SPEED_MAX;
    }
    if (score[1] > LEAF_SPEED_ACCEL_LIMIT / 100U) {
        return LEAF_SPEED_MAX;
    }

    // Calculate real score
    UINT8 real_score = score[0];
    if (score[1] > 0U) {
        real_score += (UINT8)(score[1] * 100U);
    }

    // Max speed reached
    if (real_score >= LEAF_SPEED_ACCEL_LIMIT) {
        return LEAF_SPEED_MAX;
    }

    {
        UINT8 stage = real_score / LEAF_SPEED_STAGE_INTERVAL;
        UINT8 speed = LEAF_SPEED_INITIAL + (stage >> 2); // Increase speed every 4 stages
        UINT8 pattern = stage & 3U; // 0, 1, 2, 3
        UINT8 modulo = frame_counter & 3U;

        // Boost speed based on pattern
        // if pattern is 0, no boost
        if (pattern == 1U) {
            // 1 boost per 4f
            if (modulo == 0U) {
                speed++;
            }
        } else if (pattern == 2U) {
            // 2 boosts per 4f
            if ((modulo == 0U) || (modulo == 2U)) {
                speed++;
            }
        } else if (pattern == 3U) {
            // 3 boosts per 4f
            if (modulo != 3U) {
                speed++;
            }
        }

        return speed;
    }
}

void leaves_scroll(void) {
    UINT8 speed = leaves_speed_calc();

    if (leaves_pos[0][1] > 0 + speed) {
        leaves_pos[0][1] -= speed;
    } else if (leaves_pos[0][1] > 0) {
        leaves_pos[0][1] = 0;
    }
    if (leaves_pos[1][1] > 0 + speed) {
        leaves_pos[1][1] -= speed;
    } else if (leaves_pos[1][1] > 0) {
        leaves_pos[1][1] = 0;
    }
    if (leaves_pos[2][1] > 0 + speed) {
        leaves_pos[2][1] -= speed;
    } else if (leaves_pos[2][1] > 0) {
        leaves_pos[2][1] = 0;
    }

    if (frame_counter == 0) {
        if (!is_first_frame_count) {
            leaves_pos[0][0] = uint8_random(FALL_ITEM_RANDOM_MIN_X, FALL_ITEM_RANDOM_MAX_X);
        }
        leaves_pos[0][1] = SCREEN_BOTTOM;
    } else if (frame_counter == 60) {
        if (!is_first_frame_count) {
            leaves_pos[1][0] = uint8_random(FALL_ITEM_RANDOM_MIN_X, FALL_ITEM_RANDOM_MAX_X);
        }
        leaves_pos[1][1] = SCREEN_BOTTOM;
    } else if (frame_counter == 120) {
        if (!is_first_frame_count) {
            leaves_pos[2][0] = uint8_random(FALL_ITEM_RANDOM_MIN_X, FALL_ITEM_RANDOM_MAX_X);
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
    apple_bomb_pos[0] = uint8_random(FALL_ITEM_RANDOM_MIN_X, FALL_ITEM_RANDOM_MAX_X);
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
    if (frame_counter % 2 && apple_bomb_pos[1] > 1) {
        apple_bomb_pos[1] -= 2;
    } else if (apple_bomb_pos[1] > 0) {
        apple_bomb_pos[1] -= 1;
    }
}

void background_scroll(void) {
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
    UINT8 player_left = (player_pos[0] + player_hitbox_margin_left);
    UINT8 player_right = (player_pos[0] + player_hitbox_width - player_hitbox_margin_right);
    UINT8 player_top = (player_pos[1] + player_hitbox_margin_top);
    UINT8 player_bottom = (player_pos[1] + player_hitbox_height - player_hitbox_margin_bottom);

    // Check leaves
    for (UINT8 i = 0; i < 3; i++) {
        UINT8 leaf_left = (leaves_pos[i][0] + LEAF_MARGIN);
        UINT8 leaf_right = (leaves_pos[i][0] + LEAF_HITBOX_WIDTH - LEAF_MARGIN);
        UINT8 leaf_top = (leaves_pos[i][1] + LEAF_MARGIN);
        UINT8 leaf_bottom = (leaves_pos[i][1] + LEAF_HITBOX_HEIGHT - LEAF_MARGIN);

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
        UINT8 apple_right = (apple_bomb_pos[0] + APPLE_BOMB_HITBOX_WIDTH - APPLE_BOMB_MARGIN_RIGHT);
        UINT8 apple_top = (apple_bomb_pos[1] + APPLE_BOMB_MARGIN_TOP);
        UINT8 apple_bottom = (apple_bomb_pos[1] + APPLE_BOMB_HITBOX_HEIGHT - APPLE_BOMB_MARGIN_BOTTOM);

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
    cursor_pos = FALSE;
    init_game();
    init_ui_title();
    init_bkg_attr();
}

void update_title_screen(void) {
    UINT8 controller = joypad();

    // Move cursor
    if (!(prev_controller & J_SELECT) && (controller & J_SELECT)) {
        cursor_pos = !cursor_pos;
        render_title_menu(cursor_pos);
    }
    if (!(prev_controller & J_UP) && (controller & J_UP)) {
        cursor_pos = FALSE;
        render_title_menu(cursor_pos);
    }
    if (!(prev_controller & J_DOWN) && (controller & J_DOWN)) {
        cursor_pos = TRUE;
        render_title_menu(cursor_pos);
    }

    // Select option
    if (!controller && (prev_controller & (J_START | J_A | J_B))) {
        if (cursor_pos) {
            dog_mode = !dog_mode;
            init_player();
        } else {
            show_gameplay_screen();
            return;
        }
    }

    // Create random seed
    rand_timer++;
    rand_controller += (controller * rand_timer);
}

void show_gameplay_screen(void) {
    scene_mode = 1;
    initrand((UINT16)rand_timer | (UINT16)rand_controller << 8);
    init_game();
    init_ui_gameplay(score, player_life);
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

    // Exit to title screen
    if (!prev_controller && (joypad() & J_START)) {
        show_title_screen();
        return;
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
    render_gameover();
}

void update_gameover_screen(void) {
    // First, fall down player
    if (player_pos[1] < SCREEN_BOTTOM) {
        player_pos[1] += 3;
        render_player();
        return;
    }

    // Second, wait for input to restart
    UINT8 controller = joypad();
    if (!controller && (prev_controller & (J_START | J_A | J_B))) {
        show_title_screen();
    }
}

void main(void) {
    DISPLAY_OFF;
    
    // Initialize
    init_vram();
    init_window();
    init_sprites();
    show_title_screen();
    prev_controller = joypad();

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

        prev_controller = joypad();
        wait_vbl_done();
    }
}
