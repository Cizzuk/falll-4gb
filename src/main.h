#ifndef __Main_h_INCLUDE
#define __Main_h_INCLUDE

#include <gb/gb.h>

#define PLAYER_START_X ((PLAY_AREA_MAX_X - PLAY_AREA_MIN_X) / 2 + PLAY_AREA_MIN_X)
#define PLAYER_START_Y 32U
#define PLAYER_INITIAL_LIFE 3U

#define LEAF1_START_X (PLAY_AREA_MIN_X + 8)
#define LEAF1_START_Y 160U
#define LEAF2_START_X ((PLAY_AREA_MIN_X + PLAY_AREA_MAX_X) / 2)
#define LEAF2_START_Y 0U
#define LEAF3_START_X (PLAY_AREA_MAX_X - 8)
#define LEAF3_START_Y 0U

#define APPLE_BOMB_START_X 0U
#define APPLE_BOMB_START_Y 0U

#define PLAYER_HITBOX_WIDTH 16U
#define PLAYER_HITBOX_HEIGHT 16U
#define PLAYER_MARGIN_TOP 2U
#define PLAYER_MARGIN_LEFT 2U
#define PLAYER_MARGIN_RIGHT 2U
#define PLAYER_MARGIN_BOTTOM 0U

#define DOG_HITBOX_WIDTH 24U
#define DOG_HITBOX_HEIGHT 24U
#define DOG_MARGIN_TOP 6U
#define DOG_MARGIN_LEFT 0U
#define DOG_MARGIN_RIGHT 1U
#define DOG_MARGIN_BOTTOM 4U

#define LEAF_HITBOX_WIDTH 16U
#define LEAF_HITBOX_HEIGHT 16U
#define LEAF_MARGIN 2U

#define APPLE_BOMB_HITBOX_WIDTH 16U
#define APPLE_BOMB_HITBOX_HEIGHT 16U
#define APPLE_BOMB_MARGIN_TOP 4U
#define APPLE_BOMB_MARGIN_LEFT 2U
#define APPLE_BOMB_MARGIN_RIGHT 4U
#define APPLE_BOMB_MARGIN_BOTTOM 1U

extern UINT8 scene_mode;
extern BOOLEAN dog_mode;
extern UINT8 frame_counter;
extern BOOLEAN is_first_frame_count;
extern UINT8 score[3];

extern UINT8 cursor_pos;

extern UINT8 player_life;
extern BOOLEAN player_flip;
extern UINT8 player_pos[2];
extern UINT8 leaves_pos[3][2];
extern UINT8 apple_bomb_pos[2];

void init_game(void);
void init_sprites(void);
void render_player(void);
void player_control(void);
void render_leaves(void);
void leaves_scroll(void);
void summon_apple_bomb(void);
void render_apple_bomb(void);
void apple_bomb_scroll(void);
void score_counter(void);
void update_colliders(void);

void show_title_screen(void);
void update_title_screen(void);
void show_gameplay_screen(void);
void update_gameplay_screen(void);
void show_gameover_screen(void);
void update_gameover_screen(void);

#endif