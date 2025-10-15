#ifndef __Main_h_INCLUDE
#define __Main_h_INCLUDE

#include <gb/gb.h>

#define TILE_SIZE 8U

#define PLAYER_START_Y 32U
#define DOG_START_Y 28U
#define PLAYER_INITIAL_LIFE 3U

#define PLAYER_PIXEL_WIDTH 16U
#define PLAYER_PIXEL_HEIGHT 16U
#define PLAYER_VISUAL_MARGIN_TOP 1U
#define PLAYER_VISUAL_MARGIN_LEFT 2U
#define PLAYER_VISUAL_MARGIN_RIGHT 2U
#define PLAYER_VISUAL_MARGIN_BOTTOM 0U

#define PLAYER_COLLISION_MARGIN_TOP (PLAYER_VISUAL_MARGIN_TOP + 5U)
#define PLAYER_COLLISION_MARGIN_LEFT (PLAYER_VISUAL_MARGIN_LEFT + 1U)
#define PLAYER_COLLISION_MARGIN_RIGHT (PLAYER_VISUAL_MARGIN_RIGHT + 1U)
#define PLAYER_COLLISION_MARGIN_BOTTOM (PLAYER_VISUAL_MARGIN_BOTTOM + 1U)

#define DOG_PIXEL_WIDTH 32U
#define DOG_PIXEL_HEIGHT 24U
#define DOG_VISUAL_MARGIN_LEFT 0U
#define DOG_VISUAL_MARGIN_RIGHT 1U
#define DOG_VISUAL_MARGIN_TOP 0U
#define DOG_VISUAL_MARGIN_BOTTOM 0U

#define DOG_COLLISION_MARGIN_TOP (DOG_VISUAL_MARGIN_TOP + 12U)
#define DOG_COLLISION_MARGIN_LEFT (DOG_VISUAL_MARGIN_LEFT + 2U)
#define DOG_COLLISION_MARGIN_RIGHT (DOG_VISUAL_MARGIN_RIGHT + 1U)
#define DOG_COLLISION_MARGIN_BOTTOM (DOG_VISUAL_MARGIN_BOTTOM + 4U)

#define PLAY_COLUMN_MIN 2U
#define PLAY_COLUMN_MAX 10U
#define PLAY_COLUMN_EXTRA_DOG 3U
#define PLAY_BOUND_LEFT (PLAY_COLUMN_MIN * TILE_SIZE)
#define PLAY_BOUND_RIGHT ((PLAY_COLUMN_MAX * TILE_SIZE) + TILE_SIZE)
#define PLAY_BOUND_LEFT_DOG (PLAY_BOUND_LEFT - PLAY_COLUMN_EXTRA_DOG)
#define PLAY_BOUND_RIGHT_DOG (PLAY_BOUND_RIGHT + PLAY_COLUMN_EXTRA_DOG)

#define LEAF1_START_X (PLAY_AREA_MIN_X + 8)
#define LEAF1_START_Y 0U
#define LEAF2_START_X ((PLAY_AREA_MIN_X + PLAY_AREA_MAX_X) / 2)
#define LEAF2_START_Y 0U
#define LEAF3_START_X (PLAY_AREA_MAX_X - 8)
#define LEAF3_START_Y 0U

#define FALL_ITEM_RANDOM_MIN_X PLAY_AREA_MIN_X + 4
#define FALL_ITEM_RANDOM_MAX_X PLAY_AREA_MAX_X - 4

#define LEAF_HITBOX_WIDTH 16U
#define LEAF_HITBOX_HEIGHT 16U
#define LEAF_MARGIN 2U

#define APPLE_BOMB_START_X 0U
#define APPLE_BOMB_START_Y 0U

#define APPLE_BOMB_HITBOX_WIDTH 16U
#define APPLE_BOMB_HITBOX_HEIGHT 16U
#define APPLE_BOMB_MARGIN_TOP 4U
#define APPLE_BOMB_MARGIN_LEFT 2U
#define APPLE_BOMB_MARGIN_RIGHT 4U
#define APPLE_BOMB_MARGIN_BOTTOM 1U

#define LEAF_SPEED_INITIAL 2U
#define LEAF_SPEED_STAGE_INTERVAL 16U
#define LEAF_SPEED_MAX 6U

// Calculate the score at which the leaf speed will reach its maximum
#define LEAF_SPEED_ACCEL_RAW ((LEAF_SPEED_MAX - LEAF_SPEED_INITIAL) * LEAF_SPEED_STAGE_INTERVAL * 3U)
// Cap to UINT8 max value if overflow
#define LEAF_SPEED_ACCEL_LIMIT ((LEAF_SPEED_ACCEL_RAW > 255U) ? 255U : (UINT8)LEAF_SPEED_ACCEL_RAW)

extern UINT8 rand_timer;
extern UINT8 rand_controller;
extern UINT8 prev_controller;
extern UINT8 scene_mode;
extern BOOLEAN dog_mode;
extern UINT8 frame_counter;
extern BOOLEAN is_first_frame_count;
extern UINT8 score[3];

extern BOOLEAN cursor_pos;

extern UINT8 player_life;
extern BOOLEAN player_flip;
extern UINT8 player_pos[2];
extern UINT8 player_visual_width;
extern UINT8 player_visual_height;
extern UINT8 player_visual_margin_left;
extern UINT8 player_visual_margin_right;
extern UINT8 player_visual_margin_top;
extern UINT8 player_visual_margin_bottom;
extern UINT8 player_hitbox_width;
extern UINT8 player_hitbox_height;
extern UINT8 player_hitbox_margin_left;
extern UINT8 player_hitbox_margin_right;
extern UINT8 player_hitbox_margin_top;
extern UINT8 player_hitbox_margin_bottom;
extern UINT8 player_move_min_x;
extern UINT8 player_move_max_x;
extern UINT8 leaves_pos[3][2];
extern UINT8 apple_bomb_pos[2];
extern UINT8 is_bomb;
extern UINT8 background_scroll_y;

void init_vram(void);
void init_bkg_attr(void);
void init_game(void);
void init_sprites(void);
void init_player(void);
void render_player(void);
void player_control(void);
void render_leaves(void);
UINT8 leaves_speed_calc(void);
void leaves_scroll(void);
void summon_apple_bomb(void);
void render_apple_bomb(void);
void apple_bomb_scroll(void);
void background_scroll(void);
void score_counter(void);
void update_colliders(void);

void show_title_screen(void);
void update_title_screen(void);
void show_gameplay_screen(void);
void update_gameplay_screen(void);
void show_gameover_screen(void);
void update_gameover_screen(void);

#endif