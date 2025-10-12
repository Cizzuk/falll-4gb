#ifndef __Main_h_INCLUDE
#define __Main_h_INCLUDE

#include <gb/gb.h>

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