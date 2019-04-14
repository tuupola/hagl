
/*

Copyright (c) 2018 Mika Tuupola

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef _COPEPOD_H
#define _COPEPOD_H

#include <stdint.h>

#include "framebuffer.h"
#include "color.h"

#define ABS(x)  ((x) > 0 ? (x) : -(x))

#define swap(x, y) do \
    { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
        memcpy(swap_temp,&y,sizeof(x)); \
        memcpy(&y,&x,       sizeof(x)); \
        memcpy(&x,swap_temp,sizeof(x)); \
    } while (0)

extern char g_font;

/* This is the only mandatory function which HAL must provide. */
extern void pod_hal_putpixel(int16_t x0, int16_t y0, uint16_t color);

extern void pod_hal_blit(uint16_t x0, uint16_t y0, bitmap_t *source);
extern void pod_hal_hline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
extern void pod_hal_vline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
extern void pod_hal_init();
extern void pod_hal_flush();

void pod_put_pixel(int16_t x0, int16_t y0, uint16_t color);
void pod_draw_char(char ascii, int16_t x0, int16_t y0, uint16_t color, char font[128][8]);
void pod_puttext(char *str, int16_t x0, int16_t y0, uint16_t color, char font[128][8]);
void pod_blit(int16_t x0, int16_t y0, bitmap_t *source);
void pod_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source);

void pod_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pod_draw_hline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
void pod_draw_vline(int16_t x0, int16_t y0, uint16_t height, uint16_t color);

void pod_draw_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pod_fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pod_draw_circle(int16_t xc, int16_t yc, int16_t r, uint16_t color);
void pod_fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void pod_draw_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void pod_fill_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void pod_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void pod_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

void pod_set_clip_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void pod_clear_screen();
void pod_init();
void pod_flush();
void pod_destroy();

#endif /* _COPEPOD_H */
