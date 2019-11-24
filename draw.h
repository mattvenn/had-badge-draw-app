#define FB_WIDTH 480
#define FB_HEIGHT 320

#define BLACK 0
#define WHITE 1
#define RED   2
#define GREEN 3
#define BLUE  4


#define PAL_OFFSET 17

#define COMP_COLOR(A, R, G, B) ((((A) & 0xFF) << 24) | \
								(((B) & 0xFF) << 16) | \
								(((G) & 0xFF) <<  8) | \
								(((R) & 0xFF) <<  0))
#define FB_PIX(X, Y) fbmem[(X) + ((Y) * FB_WIDTH)]


uint8_t *fbmem;
FILE *f;
void init();
void draw_h_line(int x1, int x2, int y, uint8_t colour);
void draw_v_line(int y1, int y2, int x, uint8_t colour);
void draw_rect(int x1, int y1, int x2, int y2, uint8_t colour);
void flush_fb();
void set_text(int x, int y, char * string);
void clear_text();
void clear_fbuf(uint8_t colour);
void create_palette(void);
