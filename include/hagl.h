/*

MIT License

Copyright (c) 2018-2020 Mika Tuupola

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

-cut-

This file is part of the HAGL graphics library:
https://github.com/tuupola/hagl


SPDX-License-Identifier: MIT

*/

#ifndef _HAGL_H
#define _HAGL_H

#include <stdint.h>

#include "bitmap.h"

#define ABS(x)  ((x) > 0 ? (x) : -(x))

#define HAGL_OK                  (0)
#define HAGL_ERR_GENERAL         (1)
#define HAGL_ERR_FILE_IO         (2)
#define HAGL_ERR_TJPGD           (100)

#ifndef char16_t
typedef uint16_t char16_t;
#endif

/* This is the only mandatory function which HAL must provide. */
extern void hagl_hal_put_pixel(int16_t x0, int16_t y0, uint16_t color);

extern void hagl_hal_blit(uint16_t x0, uint16_t y0, bitmap_t *source);
extern void hagl_hal_hline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
extern void hagl_hal_vline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
extern void hagl_hal_flush();
extern bitmap_t *hagl_hal_init();

void hagl_put_pixel(int16_t x0, int16_t y0, uint16_t color);

/**
 * Draw a single character
 *
 * @param code Unicode code point
 * @param x0 X coordinate
 * @param y0 Y coorginate
 * @param color RGB565 color
 * @param font Pointer to a FONTX2 font
 * @return Width of the drawn character
 */
uint8_t hagl_put_char(char16_t code, int16_t x0, int16_t y0, uint16_t color, const unsigned char *font);

/**
 * Draw a string
 *
 * @param str Pointer to an Unicode string
 * @param x0 X coordinate
 * @param y0 Y coorginate
 * @param color RGB565 color
 * @param font Pointer to a FONTX2 font
 * @return Width of the drawn string
 */
uint16_t hagl_put_text(const char16_t *str, int16_t x0, int16_t y0, uint16_t color, const unsigned char *font);
void hagl_blit(int16_t x0, int16_t y0, bitmap_t *source);
void hagl_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source);

void hagl_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void hagl_draw_hline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
void hagl_draw_vline(int16_t x0, int16_t y0, uint16_t height, uint16_t color);

void hagl_draw_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void hagl_fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void hagl_draw_circle(int16_t xc, int16_t yc, int16_t r, uint16_t color);
void hagl_fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void hagl_draw_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, uint16_t color);
void hagl_fill_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, uint16_t color);
void hagl_draw_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void hagl_fill_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void hagl_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void hagl_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void hagl_draw_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);
void hagl_fill_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);

uint32_t hagl_load_image(int16_t x0, int16_t y0, const char *filename);

void hagl_set_clip_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
 * Clear area of the current clip window
 *
 * @return void
 */
void hagl_clear_clip_window();

/**
 * Clear the whole display
 *
 * @return void
 */
void hagl_clear_screen();
bitmap_t *hagl_init();
void hagl_flush();
void hagl_close();

#endif /* _HAGL_H */
