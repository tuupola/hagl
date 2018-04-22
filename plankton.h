
#include <stdint.h>

#define ABS(x) ((x) > 0 ? (x) : -(x))

extern void pln_ll_put_pixel(uint16_t x1, uint16_t y1, uint16_t color);
extern void pln_ll_draw_hline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
extern void pln_ll_draw_vline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void pln_put_pixel(uint16_t x1, uint16_t y1, uint16_t colour);
void pln_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
void pln_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
