#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "badgetime.h"

#include "mach_defines.h"
#include "sdk.h"
#include "gfx_load.h"
#include "cache.h"

#define FB_WIDTH 512
#define FB_HEIGHT 320

#define WIDTH 200
#define HEIGHT 320

#define BLACK 0
#define WHITE 1
#define RED   2
#define GREEN 3
#define BLUE  4

#define FB_PIX(X, Y) fbmem[(X) + ((Y) * FB_WIDTH)]
#define PAL_OFFSET 17
#define COMP_COLOR(A, R, G, B) ((((A) & 0xFF) << 24) | \
								(((B) & 0xFF) << 16) | \
								(((G) & 0xFF) <<  8) | \
								(((R) & 0xFF) <<  0))

uint8_t *fbmem;

void flush_fb()
{
	cache_flush(fbmem, fbmem+FB_WIDTH*FB_HEIGHT);
}

void clear_fbuf(uint8_t colour)
{
	// Clear the framebuffer
	for (uint32_t i = 0; i < (FB_WIDTH * FB_HEIGHT); i++) {
		fbmem[i] = colour;
	}
}

void draw_h_line(int x1, int x2, int y, uint8_t colour)
{
    clear_fbuf(BLACK);
    for(int x = x1; x < x2; x++)
        FB_PIX(x, y) = colour;
    flush_fb(); // this is required to flush graphics caches
}

void create_palette(void) 
{
    // wipe palette
    for(int i = PAL_OFFSET; i < 255; i ++)
        GFXPAL[i] = 0;
    GFXPAL[BLACK+PAL_OFFSET] = COMP_COLOR(255, 0,  0,  0);
    GFXPAL[WHITE+PAL_OFFSET] = COMP_COLOR(255, 255,255,255);
    GFXPAL[BLUE +PAL_OFFSET] = COMP_COLOR(255, 0,  0,  255);
    GFXPAL[GREEN+PAL_OFFSET] = COMP_COLOR(255, 0,  255,0);
    GFXPAL[RED  +PAL_OFFSET] = COMP_COLOR(255, 255,0,  0);
}

void init()
{
	// First, allocate some memory for the background framebuffer.
	// We will be using a custom 256 entry color palette, so we allocate 1 byte
	// per pixel.
	fbmem=calloc(FB_WIDTH,FB_HEIGHT);

	// Telling the GFX hardware what our framebuffer width is, and that it will
	// use palette entries starting from 17. The tiles left by the IPL will use
	// palette entries 0-16.
	GFX_REG(GFX_FBPITCH_REG)=(PAL_OFFSET<<GFX_FBPITCH_PAL_OFF)|(FB_WIDTH<<GFX_FBPITCH_PITCH_OFF);

	// Set up the framebuffer address
	GFX_REG(GFX_FBADDR_REG)=((uint32_t)fbmem);

    // background colour
	GFX_REG(GFX_BGNDCOL_REG)=0x000000; // black

    create_palette();
    
    clear_fbuf(BLACK);

	// The user can still see nothing of this graphics goodness, so let's
	// re-enable the framebuffer and tile layer A (the default layer for the
	// console). Also indicate the framebuffer we have is 8-bit.
	GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB_8BIT|GFX_LAYEREN_FB|GFX_LAYEREN_TILEA;
}

void main(int argc, char **argv) {

    MISC_REG(MISC_LED_REG) = 0x0;

    init();

	int run = 1;
	while(run) {
		// Exit when the start button is pressed
		if (MISC_REG(MISC_BTN_REG) & BUTTON_START)
			run = 0;
		if (MISC_REG(MISC_BTN_REG) & BUTTON_UP)
            draw_h_line(0, WIDTH, 2, GREEN);
		if (MISC_REG(MISC_BTN_REG) & BUTTON_RIGHT)
            draw_h_line(0, WIDTH, 3, GREEN);
		if (MISC_REG(MISC_BTN_REG) & BUTTON_DOWN)
            draw_h_line(0, WIDTH, 4, GREEN);
		if (MISC_REG(MISC_BTN_REG) & BUTTON_LEFT)
            draw_h_line(0, WIDTH, 5, GREEN);
	}
	printf("app done. Bye!\n");
}
