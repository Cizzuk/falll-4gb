#include <gb/gb.h>
#include "sprites.h"

// Screen constants
// #define SCREEN_WIDTH 160
// #define SCREEN_HEIGHT 144
// #define SCREEN_TOP 16
// #define SCREEN_BOTTOM (SCREEN_TOP + SCREEN_HEIGHT)
// #define SCREEN_LEFT 8
// #define SCREEN_RIGHT (SCREEN_LEFT + SCREEN_WIDTH)

BOOLEAN is_gaming = TRUE;
BOOLEAN player_flip = FALSE;
UINT8 leaf_frame = 0;

// screen has dead zones, left: 8px, top: 16px
UINT8 player_pos[2] = {63, 32}; // X: 36 - 90
UINT8 leaves_pos[3][2] = {{43, 160}, {63, 0}, {83, 0}};

void init_sprites(void) {
    // Player
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 2);
    set_sprite_tile(2, 1);
    set_sprite_tile(3, 3);

    // Leaves
    // Leaf 1
    set_sprite_tile(4, 6);
    set_sprite_tile(5, 4);
    set_sprite_tile(6, 7);
    set_sprite_tile(7, 5);
    set_sprite_prop(4, S_FLIPX);
    set_sprite_prop(5, S_FLIPX);
    set_sprite_prop(6, S_FLIPX);
    set_sprite_prop(7, S_FLIPX);
    // Leaf 2
    set_sprite_tile(8, 5);
    set_sprite_tile(9, 7);
    set_sprite_tile(10, 4);
    set_sprite_tile(11, 6);
    set_sprite_prop(8, S_FLIPY);
    set_sprite_prop(9, S_FLIPY);
    set_sprite_prop(10, S_FLIPY);
    set_sprite_prop(11, S_FLIPY);
    // Leaf 3
    set_sprite_tile(12, 4);
    set_sprite_tile(13, 6);
    set_sprite_tile(14, 5);
    set_sprite_tile(15, 7);
}

void render_player(void) {
    if (player_flip) {
        set_sprite_prop(0, S_FLIPX);
        set_sprite_prop(1, S_FLIPX);
        set_sprite_prop(2, S_FLIPX);
        set_sprite_prop(3, S_FLIPX);
        move_sprite(0, player_pos[0] + 8, player_pos[1]);
        move_sprite(1, player_pos[0], player_pos[1]);
        move_sprite(2, player_pos[0] + 8, player_pos[1] + 8);
        move_sprite(3, player_pos[0], player_pos[1] + 8);
    } else {
        set_sprite_prop(0, 0);
        set_sprite_prop(1, 0);
        set_sprite_prop(2, 0);
        set_sprite_prop(3, 0);
        move_sprite(0, player_pos[0], player_pos[1]);
        move_sprite(1, player_pos[0] + 8, player_pos[1]);
        move_sprite(2, player_pos[0], player_pos[1] + 8);
        move_sprite(3, player_pos[0] + 8, player_pos[1] + 8);
    }
}

void player_control(void) {
    UINT8 controller = joypad();
    if (controller & J_RIGHT && controller & J_LEFT) {
        return;
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
    if (leaves_pos[0][1] != 0) {
        move_sprite(4, leaves_pos[0][0], leaves_pos[0][1]);
        move_sprite(5, leaves_pos[0][0] + 8, leaves_pos[0][1]);
        move_sprite(6, leaves_pos[0][0], leaves_pos[0][1] + 8);
        move_sprite(7, leaves_pos[0][0] + 8, leaves_pos[0][1] + 8);
    }
    // Leaf 2
    if (leaves_pos[1][1] != 0) {
        move_sprite(8, leaves_pos[1][0], leaves_pos[1][1]);
        move_sprite(9, leaves_pos[1][0] + 8, leaves_pos[1][1]);
        move_sprite(10, leaves_pos[1][0], leaves_pos[1][1] + 8);
        move_sprite(11, leaves_pos[1][0] + 8, leaves_pos[1][1] + 8);
    }
    // Leaf 3
    if (leaves_pos[2][1] != 0) {
        move_sprite(12, leaves_pos[2][0], leaves_pos[2][1]);
        move_sprite(13, leaves_pos[2][0] + 8, leaves_pos[2][1]);
        move_sprite(14, leaves_pos[2][0], leaves_pos[2][1] + 8);
        move_sprite(15, leaves_pos[2][0] + 8, leaves_pos[2][1] + 8);
    }
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
    if (leaf_frame % 60 == 0) {
        if (leaves_pos[0][1] == 0) {
            leaves_pos[0][1] = 160;
        } else if (leaves_pos[1][1] == 0) {
            leaves_pos[1][1] = 160;
        } else if (leaves_pos[2][1] == 0) {
            leaves_pos[2][1] = 160;
        }
    }
}

void reset_game(void) {
    is_gaming = TRUE;
    player_flip = FALSE;
    leaf_frame = 0;
    player_pos[0] = 55;
    player_pos[1] = 32;
}

void main(void) {
    set_sprite_data(0, 15, Sprites);
    init_sprites();

    DISPLAY_ON;
    SHOW_SPRITES;
    SHOW_BKG;

    while (TRUE) {
        if (is_gaming) {
            player_control();

            if (leaf_frame < 179) {
                leaf_frame++;
            } else {
                leaf_frame = 0;
            }
        }

        leaves_scroll();
        render_player();
        render_leaves();
        wait_vbl_done();
    }
}
