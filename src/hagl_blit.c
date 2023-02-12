/*

MIT License

Copyright (c) 2018-2023 Mika Tuupola

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

#include <stdint.h>

#include "hagl/color.h"
#include "hagl/pixel.h"
#include "hagl/bitmap.h"
#include "hagl/surface.h"

void
hagl_blit_xy(void const *_surface, int16_t x0, int16_t y0, hagl_bitmap_t *source)
{
    const hagl_surface_t *surface = _surface;

    if (surface->blit) {
        /* Check if bitmap is inside clip windows bounds */
        if (
            (x0 < surface->clip.x0) ||
            (y0 < surface->clip.y0) ||
            (x0 + source->width > surface->clip.x1) ||
            (y0 + source->height > surface->clip.y1)
        ) {
            /* Out of bounds, use local putpixel fallback. */
            hagl_color_t color;
            hagl_color_t *ptr = (hagl_color_t *) source->buffer;

            for (uint16_t y = 0; y < source->height; y++) {
                for (uint16_t x = 0; x < source->width; x++) {
                    color = *(ptr++);
                    hagl_put_pixel(surface, x0 + x, y0 + y, color);
                }
            }
        } else {
            /* Inside of bounds, can use HAL provided blit. */
            surface->blit(&surface, x0, y0, source);
        }
    } else {
        hagl_color_t color;
        hagl_color_t *ptr = (hagl_color_t *) source->buffer;

        for (uint16_t y = 0; y < source->height; y++) {
            for (uint16_t x = 0; x < source->width; x++) {
                color = *(ptr++);
                hagl_put_pixel(surface, x0 + x, y0 + y, color);
            }
        }
    }
};

void
hagl_blit_xywh(void const *_surface, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, hagl_bitmap_t *source)
{
    const hagl_surface_t *surface = _surface;

    if (surface->scale_blit) {
        surface->scale_blit(&surface, x0, y0, w, h, source);
    } else {
        hagl_color_t color;
        hagl_color_t *ptr = (hagl_color_t *) source->buffer;
        uint32_t x_ratio = (uint32_t)((source->width << 16) / w);
        uint32_t y_ratio = (uint32_t)((source->height << 16) / h);

        for (uint16_t y = 0; y < h; y++) {
            for (uint16_t x = 0; x < w; x++) {
                uint16_t px = ((x * x_ratio) >> 16);
                uint16_t py = ((y * y_ratio) >> 16);
                color = *(ptr + (py * source->width) + px);
                hagl_put_pixel(surface, x0 + x, y0 + y, color);
            }
        }
    }
};
