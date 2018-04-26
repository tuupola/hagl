
#include <stdint.h>

#define RGB565(R, G, B) (uint16_t)(((R & 0xf8) << 8) | ((G & 0xfc) << 3) | ((B & 0xf8) >> 3))

#define ABS(x)          ((x) > 0 ? (x) : -(x))

extern void pln_ll_put_pixel(uint16_t x0, uint16_t y0, uint16_t color);
//extern void pln_ll_put_pixel(uint16_t x0, uint16_t y0, uint16_t color);
extern void pln_ll_put_bitmap(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t *bitmap);
extern void pln_ll_draw_hline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
extern void pln_ll_draw_vline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);


void pln_put_char(char ascii, uint16_t x0, uint16_t y0, uint16_t color);
void pln_put_text(char *str, uint16_t x0, uint16_t y0, uint16_t color);
void pln_put_pixel(uint16_t x0, uint16_t y0, uint16_t color);

void pln_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void pln_hline(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color);
void pln_vline(uint16_t x0, uint16_t y0, uint16_t height, uint16_t color);

void pln_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void pln_fillrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void pln_put_bitmap(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t *bitmap);

