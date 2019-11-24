#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "badgetime.h"

#include "mach_defines.h"
#include "sdk.h"
#include "gfx_load.h"
#include "cache.h"
#include "draw.h"

void render_lines()
{
    clear_fbuf(BLACK);
    char * text = malloc(32);
    int x = FB_WIDTH / 2;
    int y = FB_HEIGHT / 2;
    // if height increment is 3 then horizontal lines are 2 pix thick 
    // if height increment is 2 then horizontal lines are missing
    // vertical lines are always correct
    for(int height = 0; height < FB_HEIGHT/2; height += 2)
    {
        sprintf(text, "%03d,%03d %03d,%03d", x - height, y - height, x + height, y + height);
        set_text(0,0, text);
        draw_rect(x - height, y - height, x + height, y + height, GREEN);
    }

    flush_fb(); // this is required to avoid missing parts of the drawing
}
void main(int argc, char **argv) {

    MISC_REG(MISC_LED_REG) = 0x0;

    init();

	int run = 1;
    render_lines();
	while(run) {
		// Exit when the start button is pressed
		if (MISC_REG(MISC_BTN_REG) & BUTTON_START) {
			run = 0;
    	}
	}
	printf("app done. Bye!\n");
}
