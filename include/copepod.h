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

This file is part of the Copepod graphics library:
https://github.com/tuupola/copepod

SPDX-License-Identifier: MIT

*/

#ifndef _COPEPOD_H
#define _COPEPOD_H

#include <stdint.h>
#include <stdbool.h>

#include "bitmap.h"

#define ABS(x)  ((x) > 0 ? (x) : -(x))

#define POD_OK                  (0)
#define POD_ERR_GENERAL         (1)
#define POD_ERR_FILE_IO         (2)
#define POD_ERR_TJPGD           (100)

/* This is the only mandatory function which HAL must provide. */
extern void pod_hal_put_pixel(int16_t x0, int16_t y0, uint16_t color);

extern void pod_hal_blit(uint16_t x0, uint16_t y0, bitmap_t *source);
extern void pod_hal_hline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
extern void pod_hal_vline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
extern void pod_hal_flush();
extern bitmap_t *pod_hal_init();

void pod_put_pixel(int16_t x0, int16_t y0, uint16_t color);
uint8_t pod_put_char(const char *code, int16_t x0, int16_t y0, uint16_t color, const unsigned char *font);
uint16_t pod_put_text(const char *str, int16_t x0, int16_t y0, uint16_t color, const unsigned char *font);
void pod_blit(int16_t x0, int16_t y0, bitmap_t *source);
void pod_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source);

void pod_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pod_draw_hline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
void pod_draw_vline(int16_t x0, int16_t y0, uint16_t height, uint16_t color);

void pod_draw_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pod_fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pod_draw_circle(int16_t xc, int16_t yc, int16_t r, uint16_t color);
void pod_fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void pod_draw_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, uint16_t color);
void pod_fill_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, uint16_t color);
void pod_draw_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void pod_fill_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void pod_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void pod_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void pod_draw_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);
void pod_fill_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);

uint32_t pod_load_image(int16_t x0, int16_t y0, const char *filename);

void pod_set_clip_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void pod_clear_screen();
bitmap_t *pod_init();
void pod_flush();
void pod_close();

#endif /* _COPEPOD_H */
