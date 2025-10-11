#include <gb/gb.h>

void main(void) {
    DISPLAY_ON;
    SHOW_BKG;

    while (1) {
        wait_vbl_done();
    }
}
