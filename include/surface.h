/*

MIT License

Copyright (c) 2022 Mika Tuupola

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

#ifndef _HAGL_SURFACE_H
#define _HAGL_SURFACE_H

#include <bitmap.h>
#include "hagl_hal_color.h"

typedef struct {
    int16_t width;
    int16_t height;
    uint8_t depth;
    void (*put_pixel)(void *self, int16_t x0, int16_t y0, color_t color);
    color_t (*get_pixel)(void *self, int16_t x0, int16_t y0);
    color_t (*color)(void *self, uint8_t r, uint8_t g, uint8_t b);
    void (*blit)(void *self, uint16_t x0, uint16_t y0, bitmap_t *src);
    void (*scale_blit)(void *self, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *src);
    void (*hline)(void *self, int16_t x0, int16_t y0, uint16_t width, color_t color);
    void (*vline)(void *self, int16_t x0, int16_t y0, uint16_t height, color_t color);
} hagl_surface_t;

#endif /* _HAGL_SURFACE_H */