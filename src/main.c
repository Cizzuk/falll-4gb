#include <gb/gb.h>
#include <gb/cgb.h>
#include <rand.h>
#include "main.h"
#include "background.h"
#include "map.h"
#include "sound.h"
#include "sprites.h"
#include "ui.h"
#include "utils.h"

// Game state
UINT8 rand_timer = 0U;
UINT8 rand_controller = 0U;
UINT8 prev_controller = 0U;
UINT8 scene_mode = 0U; // 0: title, 1: gameplay, 2: gameover
BOOLEAN dog_mode = FALSE;
UINT8 frame_counter = 0U; // counts from 0 to 179
BOOLEAN is_first_frame_count = TRUE;
UINT8 score[3] = {0U, 0U, 0U};

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

inline void init_vram(void) {
    VBK_REG = 0U;
    set_bkg_data(0U, 116U, Background);
    set_sprite_data(0U, 22U, Sprites);
    set_bkg_palette(0U, 8U, BackgroundPalette);
    set_sprite_palette(0U, 7U, SpritePalette);
    set_map_base();
    set_map_tree();
    init_map_attr_tree();
    init_map_attr_random();
}

void init_map_attr_tree(void) {
    if (_cpu != CGB_TYPE) {
        return;
    }

    VBK_REG = 1U;

    UINT8 column_palette[MapHeight];

    for (UINT8 i = 0U; i < MAP_ATTR_TREE_COLS_COUNT; i++) {
        for (UINT8 row = 0U; row < MapHeight; row++) {
            column_palette[row] = MapTreePalette[row];
        }

        set_bkg_tiles(MapAttrTreeCols[i], 0U, 1U, MapHeight, column_palette);
    }

    VBK_REG = 0U;
}

void init_map_attr_random(void) {
    if (_cpu != CGB_TYPE) {
        return;
    }

    VBK_REG = 1U;

    UINT8 column_palette[MapHeight];

    for (UINT8 i = 0U; i < MAP_ATTR_RANDOM_COLS_COUNT; i++) {
        for (UINT8 row = 0U; row < MapHeight; row++) {
            column_palette[row] = uint8_random(MAP_ATTR_RANDOM_PALETTE_MIN, MAP_ATTR_RANDOM_PALETTE_MAX);
        }

        set_bkg_tiles(MapAttrRandomCols[i], 0U, 1U, MapHeight, column_palette);
    }

    VBK_REG = 0U;
}

inline void set_map_base(void) {
    set_bkg_tiles(0U, MAP_BASE_HEIGHT, MAP_BASE_WIDTH, MAP_BASE_HEIGHT, MapBase);
    set_bkg_tiles(0U, MAP_BASE_HEIGHT * 2, MAP_BASE_WIDTH, MAP_BASE_HEIGHT, MapBase);
}

inline void set_map_tree(void) {
    set_bkg_tiles(MAP_TREE_X, MAP_TREE_Y, MAP_TREE_WIDTH, MAP_TREE_HEIGHT, MapObjTree);
}

inline void set_map_tree_curtain(void) {
    set_bkg_tiles(MAP_TREE_X, MAP_TREE_Y, MAP_TREE_WIDTH, MAP_TREE_HEIGHT, MapObjTreeCurtain);
}

// Refresh game variables
void init_game(void) {
    // Game state
    frame_counter = 0U;
    is_first_frame_count = TRUE;
    score[0] = 0U;
    score[1] = 0U;
    score[2] = 0U;
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
    set_sprite_tile(SpriteNumLeaf1TopLeft, 6U);
    set_sprite_tile(SpriteNumLeaf1TopRight, 4U);
    set_sprite_tile(SpriteNumLeaf1BottomLeft, 7U);
    set_sprite_tile(SpriteNumLeaf1BottomRight, 5U);
    set_sprite_prop(SpriteNumLeaf1TopLeft, S_FLIPX | SpritesCGB4);
    set_sprite_prop(SpriteNumLeaf1TopRight, S_FLIPX | SpritesCGB5);
    set_sprite_prop(SpriteNumLeaf1BottomLeft, S_FLIPX | SpritesCGB6);
    set_sprite_prop(SpriteNumLeaf1BottomRight, S_FLIPX | SpritesCGB7);
    // Leaf 2
    set_sprite_tile(SpriteNumLeaf2TopLeft, 5U);
    set_sprite_tile(SpriteNumLeaf2TopRight, 7U);
    set_sprite_tile(SpriteNumLeaf2BottomLeft, 4U);
    set_sprite_tile(SpriteNumLeaf2BottomRight, 6U);
    set_sprite_prop(SpriteNumLeaf2TopLeft, S_FLIPY | SpritesCGB5);
    set_sprite_prop(SpriteNumLeaf2TopRight, S_FLIPY | SpritesCGB7);
    set_sprite_prop(SpriteNumLeaf2BottomLeft, S_FLIPY | SpritesCGB6);
    set_sprite_prop(SpriteNumLeaf2BottomRight, S_FLIPY | SpritesCGB4);
    // Leaf 3
    set_sprite_tile(SpriteNumLeaf3TopLeft, 4U);
    set_sprite_tile(SpriteNumLeaf3TopRight, 6U);
    set_sprite_tile(SpriteNumLeaf3BottomLeft, 5U);
    set_sprite_tile(SpriteNumLeaf3BottomRight, 7U);
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

        set_sprite_tile(SpriteNumPlayer0_0, 16U);
        set_sprite_tile(SpriteNumPlayer0_1, 19U);
        set_sprite_tile(SpriteNumPlayer0_2, 19U);
        set_sprite_tile(SpriteNumPlayer0_3, 16U);
        set_sprite_tile(SpriteNumPlayer1_0, 17U);
        set_sprite_tile(SpriteNumPlayer1_1, 20U);
        set_sprite_tile(SpriteNumPlayer1_2, 20U);
        set_sprite_tile(SpriteNumPlayer1_3, 17U);
        set_sprite_tile(SpriteNumPlayer2_0, 18U);
        set_sprite_tile(SpriteNumPlayer2_1, 21U);
        set_sprite_tile(SpriteNumPlayer2_2, 21U);
        set_sprite_tile(SpriteNumPlayer2_3, 18U);
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

        set_sprite_tile(SpriteNumPlayer0_0, 0U);
        set_sprite_tile(SpriteNumPlayer0_1, 2U);
        set_sprite_tile(SpriteNumPlayer0_2, 1U);
        set_sprite_tile(SpriteNumPlayer0_3, 3U);
        set_sprite_prop(SpriteNumPlayer0_0, SpritesCGB0);
        set_sprite_prop(SpriteNumPlayer0_1, SpritesCGB2);
        set_sprite_prop(SpriteNumPlayer0_2, SpritesCGB1);
        set_sprite_prop(SpriteNumPlayer0_3, SpritesCGB3);

        // Hide unused sprites
        move_sprite(SpriteNumPlayer1_0, 0U, 0U);
        move_sprite(SpriteNumPlayer1_1, 0U, 0U);
        move_sprite(SpriteNumPlayer1_2, 0U, 0U);
        move_sprite(SpriteNumPlayer1_3, 0U, 0U);
        move_sprite(SpriteNumPlayer2_0, 0U, 0U);
        move_sprite(SpriteNumPlayer2_1, 0U, 0U);
        move_sprite(SpriteNumPlayer2_2, 0U, 0U);
        move_sprite(SpriteNumPlayer2_3, 0U, 0U);
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
    player_pos[0] = player_move_min_x + ((player_move_max_x - player_move_min_x) >> 1U);

    render_player();
}

void render_player(void) {
    if (dog_mode) {
        move_sprite(SpriteNumPlayer0_0, player_pos[0], player_pos[1]);
        move_sprite(SpriteNumPlayer0_1, player_pos[0] + 8U, player_pos[1]);
        move_sprite(SpriteNumPlayer0_2, player_pos[0] + 15U, player_pos[1]);
        move_sprite(SpriteNumPlayer0_3, player_pos[0] + 23U, player_pos[1]);
        move_sprite(SpriteNumPlayer1_0, player_pos[0], player_pos[1] + 8U);
        move_sprite(SpriteNumPlayer1_1, player_pos[0] + 8U, player_pos[1] + 8U);
        move_sprite(SpriteNumPlayer1_2, player_pos[0] + 15U, player_pos[1] + 8U);
        move_sprite(SpriteNumPlayer1_3, player_pos[0] + 23U, player_pos[1] + 8U);
        move_sprite(SpriteNumPlayer2_0, player_pos[0], player_pos[1] + 16U);
        move_sprite(SpriteNumPlayer2_1, player_pos[0] + 8U, player_pos[1] + 16U);
        move_sprite(SpriteNumPlayer2_2, player_pos[0] + 15U, player_pos[1] + 16U);
        move_sprite(SpriteNumPlayer2_3, player_pos[0] + 23U, player_pos[1] + 16U);
    } else {
        if (player_flip) {
            set_sprite_prop(SpriteNumPlayer0_0, S_FLIPX | SpritesCGB0);
            set_sprite_prop(SpriteNumPlayer0_1, S_FLIPX | SpritesCGB2);
            set_sprite_prop(SpriteNumPlayer0_2, S_FLIPX | SpritesCGB1);
            set_sprite_prop(SpriteNumPlayer0_3, S_FLIPX | SpritesCGB3);
            move_sprite(SpriteNumPlayer0_0, player_pos[0] + 8U, player_pos[1]);
            move_sprite(SpriteNumPlayer0_1, player_pos[0], player_pos[1]);
            move_sprite(SpriteNumPlayer0_2, player_pos[0] + 8U, player_pos[1] + 8U);
            move_sprite(SpriteNumPlayer0_3, player_pos[0], player_pos[1] + 8U);
        } else {
            set_sprite_prop(SpriteNumPlayer0_0, SpritesCGB0);
            set_sprite_prop(SpriteNumPlayer0_1, SpritesCGB2);
            set_sprite_prop(SpriteNumPlayer0_2, SpritesCGB1);
            set_sprite_prop(SpriteNumPlayer0_3, SpritesCGB3);
            move_sprite(SpriteNumPlayer0_0, player_pos[0], player_pos[1]);
            move_sprite(SpriteNumPlayer0_1, player_pos[0] + 8U, player_pos[1]);
            move_sprite(SpriteNumPlayer0_2, player_pos[0], player_pos[1] + 8U);
            move_sprite(SpriteNumPlayer0_3, player_pos[0] + 8U, player_pos[1] + 8U);
        }
    }
}

inline void player_control(void) {
    const UINT8 controller = joypad();
    if (controller & J_RIGHT && controller & J_LEFT) {
        return;
    }

    const BOOLEAN is_slow = (controller & (J_A | J_B));

    if (dog_mode) {
        const BOOLEAN is_fast = (!is_slow && uint8_mod3(frame_counter) == 0U);

        if (controller & J_RIGHT) {
            if (player_pos[0] < player_move_max_x) {
                if (is_fast && player_pos[0] < (player_move_max_x - 1U)) {
                    player_pos[0] += 2U;
                } else {
                    player_pos[0]++;
                }
            }
        } else if (controller & J_LEFT) {
            if (player_pos[0] > player_move_min_x) {
                if (is_fast && player_pos[0] > (player_move_min_x + 1U)) {
                    player_pos[0] -= 2U;
                } else {
                    player_pos[0]--;
                }
            }
        }
    } else {
        // Slow down
        if (is_slow) {
            if (frame_counter & 1U) { // frame_counter % 2
                return;
            }
        }

        if (controller & J_RIGHT) {
            if (player_pos[0] < player_move_max_x) {
                player_pos[0]++;
                player_flip = TRUE;
            }
        } else if (controller & J_LEFT) {
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
    move_sprite(SpriteNumLeaf1TopRight, leaves_pos[0][0] + 8U, leaves_pos[0][1]);
    move_sprite(SpriteNumLeaf1BottomLeft, leaves_pos[0][0], leaves_pos[0][1] + 8U);
    move_sprite(SpriteNumLeaf1BottomRight, leaves_pos[0][0] + 8U, leaves_pos[0][1] + 8U);
    // Leaf 2
    move_sprite(SpriteNumLeaf2TopLeft, leaves_pos[1][0], leaves_pos[1][1]);
    move_sprite(SpriteNumLeaf2TopRight, leaves_pos[1][0] + 8U, leaves_pos[1][1]);
    move_sprite(SpriteNumLeaf2BottomLeft, leaves_pos[1][0], leaves_pos[1][1] + 8U);
    move_sprite(SpriteNumLeaf2BottomRight, leaves_pos[1][0] + 8U, leaves_pos[1][1] + 8U);
    // Leaf 3
    move_sprite(SpriteNumLeaf3TopLeft, leaves_pos[2][0], leaves_pos[2][1]);
    move_sprite(SpriteNumLeaf3TopRight, leaves_pos[2][0] + 8U, leaves_pos[2][1]);
    move_sprite(SpriteNumLeaf3BottomLeft, leaves_pos[2][0], leaves_pos[2][1] + 8U);
    move_sprite(SpriteNumLeaf3BottomRight, leaves_pos[2][0] + 8U, leaves_pos[2][1] + 8U);
}

UINT8 leaves_speed_calc(void) {
    // Calculate score for speed
    UINT8 real_score = score[0];
    
    if (score[2]) {
        real_score = UINT8_MAX;
    } else {
        if (score[1] > 2U) {
            real_score = UINT8_MAX;
        } else if (score[1] == 1U) {
            real_score += 100U;
        } else if (score[1] == 2U) {
            if (score[0] > 55U) {
                real_score = UINT8_MAX;
            } else {
                real_score += 200U;
            }
        }
    }

    // Max speed reached
    if (real_score >= LEAF_SPEED_ACCEL_LIMIT) {
        return LEAF_SPEED_MAX;
    }

    {
        const UINT8 stage = real_score / LEAF_SPEED_STAGE_INTERVAL;
        const UINT8 pattern = stage & 3U; // stage % 4
        const UINT8 modulo = frame_counter & 3U; // frame_counter % 4
        UINT8 speed = LEAF_SPEED_INITIAL + (stage >> 2U); // Increase speed every 4 stages

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
    const UINT8 speed = leaves_speed_calc();

    if (leaves_pos[0][1] > speed) {
        leaves_pos[0][1] -= speed;
    } else if (leaves_pos[0][1] > 0U) {
        leaves_pos[0][1] = 0U;
    }
    if (leaves_pos[1][1] > speed) {
        leaves_pos[1][1] -= speed;
    } else if (leaves_pos[1][1] > 0U) {
        leaves_pos[1][1] = 0U;
    }
    if (leaves_pos[2][1] > speed) {
        leaves_pos[2][1] -= speed;
    } else if (leaves_pos[2][1] > 0U) {
        leaves_pos[2][1] = 0U;
    }

    if (frame_counter == 59U) {
        if (!is_first_frame_count) {
            leaves_pos[0][0] = uint8_random(FALL_ITEM_RANDOM_MIN_X, FALL_ITEM_RANDOM_MAX_X);
        }
        leaves_pos[0][1] = SCREEN_BOTTOM;
    } else if (frame_counter == 119U) {
        if (!is_first_frame_count) {
            leaves_pos[1][0] = uint8_random(FALL_ITEM_RANDOM_MIN_X, FALL_ITEM_RANDOM_MAX_X);
        }
        leaves_pos[1][1] = SCREEN_BOTTOM;
    } else if (frame_counter == 179U) {
        if (!is_first_frame_count) {
            leaves_pos[2][0] = uint8_random(FALL_ITEM_RANDOM_MIN_X, FALL_ITEM_RANDOM_MAX_X);
        }
        leaves_pos[2][1] = SCREEN_BOTTOM;
    }
}

void summon_apple_bomb(void) {
    if (is_bomb) {
        // Bomb
        set_sprite_tile(SpriteNumAppleBombTopLeft, 12U);
        set_sprite_tile(SpriteNumAppleBombTopRight, 14U);
        set_sprite_tile(SpriteNumAppleBombBottomLeft, 13U);
        set_sprite_tile(SpriteNumAppleBombBottomRight, 15U);
        set_sprite_prop(SpriteNumAppleBombTopLeft, SpritesCGB12);
        set_sprite_prop(SpriteNumAppleBombTopRight, SpritesCGB14);
        set_sprite_prop(SpriteNumAppleBombBottomLeft, SpritesCGB13);
        set_sprite_prop(SpriteNumAppleBombBottomRight, SpritesCGB15);
    } else {
        // Apple
        set_sprite_tile(SpriteNumAppleBombTopLeft, 8U);
        set_sprite_tile(SpriteNumAppleBombTopRight, 10U);
        set_sprite_tile(SpriteNumAppleBombBottomLeft, 9U);
        set_sprite_tile(SpriteNumAppleBombBottomRight, 11U);
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
    move_sprite(SpriteNumAppleBombTopRight, apple_bomb_pos[0] + 8U, apple_bomb_pos[1]);
    move_sprite(SpriteNumAppleBombBottomLeft, apple_bomb_pos[0], apple_bomb_pos[1] + 8U);
    move_sprite(SpriteNumAppleBombBottomRight, apple_bomb_pos[0] + 8U, apple_bomb_pos[1] + 8U);
}

void apple_bomb_scroll(void) {
    // Ignore first 10 points
    if (score[0] < 10U && score[1] <= 0U && score[2] <= 0U) {
        return;
    }

    // Summon apple or bomb
    // frame_counter % 60 == 25 => 25, 85, 145
    if (frame_counter == 25U || frame_counter == 85U || frame_counter == 145U) {
        if (uint8_mod10(score[0]) == 0U) {
            is_bomb = FALSE;
            summon_apple_bomb();
        } else if (uint8_mod10(score[0]) == 5U) {
            is_bomb = TRUE;
            summon_apple_bomb();
        }
    }

    // Gravity
    if (apple_bomb_pos[1] > 0U) {
        if (uint8_mod3(frame_counter) && apple_bomb_pos[1] > 1U) {
            apple_bomb_pos[1] -= 2U;
        } else {
            apple_bomb_pos[1] -= 1U;
        }
    }
}

inline void background_scroll(void) {
    background_scroll_y++;
    move_bkg(0U, background_scroll_y);
}

void score_counter(void) {
    if (is_first_frame_count && frame_counter < 120U) {
        return;
    }

    // frame_counter % 60 == 40 => 40, 100, 160
    if (frame_counter == 40U || frame_counter == 100U || frame_counter == 160U) {
        score[0]++;
        if (score[0] > 99U) {
            score[0] = 0U;
            score[1]++;
            if (score[1] > 99U) {
                score[1] = 0U;
                score[2]++;
                if (score[2] > 99U) {
                    // Limit Score
                    score[2] = 100U;
                }
            }
        }
        render_score(score);
    }
}

// Colliders
inline void update_colliders(void) {
    const UINT8 player_left = (player_pos[0] + player_hitbox_margin_left);
    const UINT8 player_right = (player_pos[0] + player_hitbox_width - player_hitbox_margin_right);
    const UINT8 player_top = (player_pos[1] + player_hitbox_margin_top);
    const UINT8 player_bottom = (player_pos[1] + player_hitbox_height - player_hitbox_margin_bottom);

    // Check leaves
    for (UINT8 i = 0U; i < 3U; i++) {
        const UINT8 leaf_left = (leaves_pos[i][0] + LEAF_MARGIN);
        const UINT8 leaf_right = (leaves_pos[i][0] + LEAF_HITBOX_WIDTH - LEAF_MARGIN);
        const UINT8 leaf_top = (leaves_pos[i][1] + LEAF_MARGIN);
        const UINT8 leaf_bottom = (leaves_pos[i][1] + LEAF_HITBOX_HEIGHT - LEAF_MARGIN);

        if (check_collision(player_left, player_top, player_right, player_bottom,
                            leaf_left, leaf_top, leaf_right, leaf_bottom)) {
            leaves_pos[i][1] = 0U; // Hide leaf
            if (player_life > 0U) {
                player_life--;
                render_lives(player_life);
                if (player_life != 0U) {
                    play_sound_stick();
                } else {
                    play_sound_gameover();
                }
            }
        }
    }

    // Check apple/bomb
    {
        UINT8 apple_left;
        UINT8 apple_right;
        UINT8 apple_top;
        UINT8 apple_bottom;
        if (is_bomb) {
            apple_left = (apple_bomb_pos[0] + BOMB_MARGIN_LEFT);
            apple_right = (apple_bomb_pos[0] + APPLE_BOMB_HITBOX_WIDTH - BOMB_MARGIN_RIGHT);
            apple_top = (apple_bomb_pos[1] + BOMB_MARGIN_TOP);
            apple_bottom = (apple_bomb_pos[1] + APPLE_BOMB_HITBOX_HEIGHT - BOMB_MARGIN_BOTTOM);
        } else {
            apple_left = (apple_bomb_pos[0] + APPLE_MARGIN_LEFT);
            apple_right = (apple_bomb_pos[0] + APPLE_BOMB_HITBOX_WIDTH - APPLE_MARGIN_RIGHT);
            apple_top = (apple_bomb_pos[1] + APPLE_MARGIN_TOP);
            apple_bottom = (apple_bomb_pos[1] + APPLE_BOMB_HITBOX_HEIGHT - APPLE_MARGIN_BOTTOM);
        }

        if (check_collision(player_left, player_top, player_right, player_bottom,
                            apple_left, apple_top, apple_right, apple_bottom)) {
            apple_bomb_pos[1] = 0U; // Hide apple/bomb
            if (is_bomb) {
                player_life = 0U;
                play_sound_bomb();
            } else {
                player_life++;
                if (player_life > PLAYER_INITIAL_LIFE) {
                    player_life = PLAYER_INITIAL_LIFE;
                }
                play_sound_apple();
            }
            render_lives(player_life);
        }
    }
}

/* ====== Scene Manager ====== */

void show_title_screen(void) {
    scene_mode = 0U;
    cursor_pos = FALSE;
    init_game();
    play_sound_done();
    set_map_tree();
    init_ui_title();
    init_map_attr_random();
}

inline void update_title_screen(void) {
    const UINT8 controller = joypad();

    if (controller) { // Press button
        // Move cursor
        if ((controller & J_SELECT) && !(prev_controller & J_SELECT)) {
            cursor_pos = !cursor_pos;
            render_title_menu(cursor_pos);
            play_sound_select();
        } else if ((controller & J_UP) && !(prev_controller & J_UP)) {
            cursor_pos = FALSE;
            render_title_menu(cursor_pos);
            play_sound_select();
        } else if ((controller & J_DOWN) && !(prev_controller & J_DOWN)) {
            cursor_pos = TRUE;
            render_title_menu(cursor_pos);
            play_sound_select();
        }
    } else { // Release button
        // Select option
        if ((prev_controller & (J_START | J_A | J_B))) {
            if (cursor_pos) {
                dog_mode = !dog_mode;
                init_player();
                play_sound_done();
            } else {
                show_gameplay_screen();
                return;
            }
        }
    }
}

void show_gameplay_screen(void) {
    scene_mode = 1U;
    initrand((UINT16)rand_timer | (UINT16)rand_controller << 8U);
    init_game();
    play_sound_gamestart();
    set_map_tree_curtain();
    init_ui_gameplay(score, player_life);
}

inline void update_gameplay_screen(void) {
    // Check game over
    if (player_life <= 0U) {
        show_gameover_screen();
        return;
    }

    // Exit to title screen
    if ((prev_controller & J_START) && !joypad()) {
        show_title_screen();
        return;
    }

    if (frame_counter < 179U) {
        frame_counter++;
    } else {
        frame_counter = 0U;
        if (is_first_frame_count) {
            is_first_frame_count = FALSE;
        }
        initrand((UINT16)rand_timer | (UINT16)rand_controller << 8U);
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
    scene_mode = 2U;
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

inline void update_gameover_screen(void) {
    // First, fall down player
    if (player_pos[1] < SCREEN_BOTTOM) {
        player_pos[1] += 3U;
        render_player();
        return;
    }

    // Second, wait for input to restart
    if ((prev_controller & (J_START | J_SELECT | J_A | J_B)) && !joypad()) {
        show_title_screen();
    }
}

void main(void) {
    DISPLAY_OFF;

    // Initialize
    init_vram();
    init_window();
    init_sprites();
    init_sound();
    show_title_screen();
    prev_controller = joypad();

    // Show All
    SHOW_BKG;
    SHOW_WIN;
    SHOW_SPRITES;
    DISPLAY_ON;

    while (TRUE) {
        const UINT8 controller = joypad();

        // Create random seed
        rand_timer++;
        rand_controller += ~controller;

        // Update scene
        if (scene_mode == 1U) {
            // Gaming
            update_gameplay_screen();
        } else if (scene_mode == 0U) {
            // Title screen
            update_title_screen();
        } else if (scene_mode == 2U) {
            // Game over
            update_gameover_screen();
        }

        // Update sound
        update_sound();

        // Store previous controller state
        prev_controller = controller;

        // Wait for next frame
        wait_vbl_done();
    }
}
