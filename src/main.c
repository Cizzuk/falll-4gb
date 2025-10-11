#include <gb/gb.h>
#include "sprites.h"

BOOLEAN is_gaming = TRUE;
BOOLEAN player_flip = FALSE;
UINT8 leaf_frame = 0;

// screen has dead zones, left: 8px, top: 16px
UINT8 player_pos[2] = {63, 32}; // X: 36 - 90

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
        if (player_pos[0] < 82) {
            player_pos[0]++;
            player_flip = TRUE;
        }
    }
    if (controller & J_LEFT) {
        if (player_pos[0] > 28) {
            player_pos[0]--;
            player_flip = FALSE;
        }
    }
}

void main(void) {
    set_sprite_data(0, 15, Sprites);

    // Player
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 2);
    set_sprite_tile(2, 1);
    set_sprite_tile(3, 3);

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

        render_player();
        wait_vbl_done();
    }
}
