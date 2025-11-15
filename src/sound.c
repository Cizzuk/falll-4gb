#include <gb/gb.h>
#include <gb/cgb.h>
#include "sound.h"

static UINT8 sound_timer = 0U; // 0 - 255, 255: idle
static UINT8 sound_id = 0U; // 0: none, 1: stick, 2: apple, 3: gameover, 4: gamestart, 5: bomb, 6: done, 8: select

inline void init_sound(void) {
    sound_timer = 0U;
    sound_id = 0U;
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;
}

void update_sound(void) {
    // No sound is playing
    if (sound_id == 0U || sound_timer == 255U) {
        return;
    }

    if (sound_id == 1U) { // stick
        if (sound_timer == 0U) {
            // C4
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x06;
            NR24_REG = 0x87;
        } else if (sound_timer == 4U) {
            // C3
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x0A;
            NR24_REG = 0x86;
        } else if (sound_timer == 8U) {
            // Stop
            stop_sound();
        }
    } else if (sound_id == 2U) { // apple
        if (sound_timer == 0U) {
            // C5
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x83;
            NR24_REG = 0x87;
        } else if (sound_timer == 4U) {
            // C6
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0xC1;
            NR24_REG = 0x87;
        } else if (sound_timer == 8U) {
            // Stop
            stop_sound();
        }
    } else if (sound_id == 3U) { // gameover
        if (sound_timer == 0U) {
            // C4
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x06;
            NR24_REG = 0x87;
        } else if (sound_timer == 5U) {
            // Mute
            NR22_REG = 0xF8;
        } else if (sound_timer == 6U) {
            // C4
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x06;
            NR24_REG = 0x87;
        } else if (sound_timer == 10U) {
            // Mute
            NR22_REG = 0xF8;
        } else if (sound_timer == 11U) {
            // C3
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x0A;
            NR24_REG = 0x86;
        } else if (sound_timer == 19U) {
            // Stop
            stop_sound();
        }
    } else if (sound_id == 4U) { // gamestart
        if (sound_timer == 0U) {
            // C4
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x06;
            NR24_REG = 0x87;
        } else if (sound_timer == 5U) {
            // Mute
            NR22_REG = 0xF8;
        } else if (sound_timer == 6U) {
            // C5
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x83;
            NR24_REG = 0x87;
        } else if (sound_timer == 10U) {
            // Mute
            NR22_REG = 0xF8;
        } else if (sound_timer == 11U) {
            // C6
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0xC1;
            NR24_REG = 0x87;
        } else if (sound_timer == 19U) {
            // Stop
            stop_sound();
        }
    } else if (sound_id == 5U) { // bomb
        if (sound_timer == 0U) {
            // Noise (like explosion)
            NR41_REG = 0x01;
            NR42_REG = 0xF0;
            NR43_REG = 0x6C;
            NR44_REG = 0x80;
        } else if (sound_timer == 16U) {
            // Stop
            stop_sound();
        }
    } else if (sound_id == 6U) { // done
        if (sound_timer == 0U) {
            // C4
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x06;
            NR24_REG = 0x87;
        } else if (sound_timer == 4U) {
            // G4
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x59;
            NR24_REG = 0x87;
        } else if (sound_timer == 8U) {
            // Stop
            stop_sound();
        }
    } else if (sound_id == 8U) { // select
        if (sound_timer == 0U) {
            // C4
            NR21_REG = 0x81;
            NR22_REG = 0xF0;
            NR23_REG = 0x06;
            NR24_REG = 0x87;
        } else if (sound_timer == 4U) {
            // Stop
            stop_sound();
        }
    }

    sound_timer++;
}

inline void stop_sound(void) {
    sound_timer = 255U;
    sound_id = 0U;
    NR22_REG = 0x00;
    NR42_REG = 0x00;
}

inline void play_sound_stick(void) {
    sound_timer = 0U;
    sound_id = 1U;
}

inline void play_sound_apple(void) {
    sound_timer = 0U;
    sound_id = 2U;
}

inline void play_sound_gameover(void) {
    sound_timer = 0U;
    sound_id = 3U;
}

inline void play_sound_gamestart(void) {
    sound_timer = 0U;
    sound_id = 4U;
}

inline void play_sound_bomb(void) {
    sound_timer = 0U;
    sound_id = 5U;
}

inline void play_sound_done(void) {
    sound_timer = 0U;
    sound_id = 6U;
}

inline void play_sound_select(void) {
    sound_timer = 0U;
    sound_id = 8U;
}
