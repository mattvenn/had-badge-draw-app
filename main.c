#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "badgetime.h"

#include "mach_defines.h"
#include "sdk.h"
#include "gfx_load.h"
#include "cache.h"
#include "draw.h"

void render_lines(int height_inc)
{
    clear_fbuf(BLACK);
    char * text = malloc(32);
    sprintf(text, "inc: %d", height_inc);
    set_text(0,0, text);

    int x = WIDTH / 2;
    int y = HEIGHT / 2;
    // if height increment is 3 then horizontal lines are 2 pix thick 
    // if height increment is 2 then horizontal lines are missing
    // vertical lines are always correct
    for(int height = 0; height < HEIGHT/2; height += height_inc)
        draw_rect(x - height, y - height, x + height, y + height, GREEN);

    flush_fb(); // this is required to flush graphics caches
}
void main(int argc, char **argv) {

    MISC_REG(MISC_LED_REG) = 0x0;

    init();

	int run = 1;
    render_lines(1);
	while(run) {
		// Exit when the start button is pressed
		if (MISC_REG(MISC_BTN_REG) & BUTTON_START)
			run = 0;
		if (MISC_REG(MISC_BTN_REG) & BUTTON_UP)
            render_lines(2);
		if (MISC_REG(MISC_BTN_REG) & BUTTON_DOWN)
            render_lines(3);
		if (MISC_REG(MISC_BTN_REG) & BUTTON_LEFT)
            render_lines(4);
		if (MISC_REG(MISC_BTN_REG) & BUTTON_LEFT)
            render_lines(5);
		if (MISC_REG(MISC_BTN_REG) & BUTTON_A)
            render_lines(1);
	}
	printf("app done. Bye!\n");
}
