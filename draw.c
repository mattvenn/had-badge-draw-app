#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "mach_defines.h"
#include "sdk.h"
#include "gfx_load.h"
#include "cache.h"
#include "draw.h"

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

void draw_h_line(int x1, int x2, int y, uint8_t colour)
{
    for(int x = x1; x < x2; x++)
        FB_PIX(x, y) = colour;
}

void draw_v_line(int y1, int y2, int x, uint8_t colour)
{
    for(int y = y1; y < y2; y++)
        FB_PIX(x, y) = colour;
}

void draw_rect(int x1, int y1, int x2, int y2, uint8_t colour)
{
    draw_h_line(x1, x2, y1, colour);
    draw_h_line(x1, x2, y2, colour);
    draw_v_line(y1, y2, x1, colour);
    draw_v_line(y1, y2, x2, colour);
}

void clear_fbuf(uint8_t colour)
{
	// Clear the framebuffer
	for (uint32_t i = 0; i < (FB_WIDTH * FB_HEIGHT); i++) {
		fbmem[i] = colour;
	}
}

void flush_fb()
{
	cache_flush(fbmem, fbmem+FB_WIDTH*FB_HEIGHT);
}

void init_text()
{
	// The IPL leaves us with a tileset that has tile 0 to 127 mapped to ASCII
	// characters, so we do not need to load anything specific for this. In
	// order to get some text out, we can use the /dev/console device that will
	// use these tiles to put text in a tilemap. It uses escape codes to do so,
	// see ipl/gloss/console_out.c for more info.
	f=fopen("/dev/console", "w");
	setvbuf(f, NULL, _IONBF, 0); //make console line unbuffered
	// Note that without the setvbuf command, no characters would be printed
	// until 1024 characters are buffered. You normally don't want this.
    clear_text();   
}

void clear_text()
{
	fprintf(f, "\033C"); //clear the console. Note '\033' is the escape character.
}

void set_text(int x, int y, char * string)
{
	fprintf(f, "\033%dX", x); //set Xpos to 5
	fprintf(f, "\033%dY", y); //set Ypos to 8
	fprintf(f, string); // Print a message
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

    init_text();

	// The user can still see nothing of this graphics goodness, so let's
	// re-enable the framebuffer and tile layer A (the default layer for the
	// console). Also indicate the framebuffer we have is 8-bit.
	GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB_8BIT|GFX_LAYEREN_FB|GFX_LAYEREN_TILEA;
}
