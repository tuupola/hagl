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

#include <stdbool.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>

#include "hagl/color.h"
#include "hagl/pixel.h"
#include "hagl/bitmap.h"
#include "hagl/surface.h"

void
hagl_blit_xy_extended(void const *_surface, int16_t x0, int16_t y0, hagl_bitmap_t *source, bool is_transparent, hagl_color_t transparent_color)
{
    const hagl_surface_t *surface = _surface;
    bool done = false;

    /*
    static bool test = false;
    if (!test) {
        printf(
            "hagl_blit_xy_extended: width=%d, height=%d, depth=%d, pitch=%d, buffer=%p, size=%d, is_transparent=%d, transparent_color=%06x\n", 
            source->width, source->height, source->depth, 
            source->pitch, source->buffer, source->size, 
            is_transparent, transparent_color
        );
    }
    */

    if ((!is_transparent && surface->blit) || (is_transparent && surface->blit_alpha)) {
        /* Check if bitmap is inside clip windows bounds */
        if (
            (x0 >= surface->clip.x0) &&
            (y0 >= surface->clip.y0) &&
            (x0 + source->width <= surface->clip.x1) &&
            (y0 + source->height <= surface->clip.y1)
            (x0 >= surface->clip.x0) &&
            (y0 >= surface->clip.y0) &&
            (x0 + source->width <= surface->clip.x1) &&
            (y0 + source->height <= surface->clip.y1)
        ) {
            /* Inside of bounds, can use HAL provided blit. */
            if (is_transparent) {
                surface->blit_alpha(&surface, x0, y0, source, transparent_color);
            } else {
                surface->blit(&surface, x0, y0, source);
            }
            done = true;
        }
    }
    /* No blit function or out of bounds, use local put_pixel fallback */
    if (!done) {
        hagl_color_t color;
        hagl_color_t *ptr = (hagl_color_t *) source->buffer;
        for (uint16_t y = 0; y < source->height; y++) {
            for (uint16_t x = 0; x < source->width; x++) {
                color = *(ptr++);
                if ((!is_transparent) || (is_transparent && transparent_color!=color)) {
                    hagl_put_pixel(surface, x0 + x, y0 + y, color);
                }
            }
        }
    }
                if ((!is_transparent) || (is_transparent && transparent_color!=color)) {
                    hagl_put_pixel(surface, x0 + x, y0 + y, color);
                }
            }
        }
    }
};

void
hagl_blit_xywh_extended(void const *_surface, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, hagl_bitmap_t *source, bool is_transparent, hagl_color_t transparent_color)
{
    const hagl_surface_t *surface = _surface;

    if (!is_transparent && surface->scale_blit) {
        surface->scale_blit(&surface, x0, y0, w, h, source);
    } else if (is_transparent && surface->scale_blit_alpha) {
        surface->scale_blit_alpha(&surface, x0, y0, w, h, source, transparent_color);
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
                if ((!is_transparent) || (is_transparent && transparent_color!=color)) {
                    hagl_put_pixel(surface, x0 + x, y0 + y, color);
                }
            }
        }
    }
};
