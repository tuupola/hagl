/*

MIT License

Copyright (c) 2018-2022 Mika Tuupola

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

#include "hagl/surface.h"
#include "hagl/hline.h"
#include "hagl/vline.h"
#include "hagl/rectangle.h"
#include "hagl/color.h"


void
hagl_draw_rectangle(void const *_surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color)
{
    const hagl_surface_t *surface = _surface;

    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < surface->clip.x0) || (y1 < surface->clip.y0))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > surface->clip.x1) || (y0 > surface->clip.y1)) {
        return;
    }

    uint16_t width = x1 - x0 + 1;
    uint16_t height = y1 - y0 + 1;

    hagl_draw_hline(surface, x0, y0, width, color);
    hagl_draw_hline(surface, x0, y1, width, color);
    hagl_draw_vline(surface, x0, y0, height, color);
    hagl_draw_vline(surface, x1, y0, height, color);
}

void
hagl_fill_rectangle(void const *_surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color)
{
    const hagl_surface_t *surface = _surface;

    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < surface->clip.x0) || (y1 < surface->clip.y0))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > surface->clip.x1) || (y0 > surface->clip.y1)) {
        return;
    }

    x0 = MAX(x0, surface->clip.x0);
    y0 = MAX(y0, surface->clip.y0);
    x1 = MIN(x1, surface->clip.x1);
    y1 = MIN(y1, surface->clip.y1);

    uint16_t width = x1 - x0 + 1;
    uint16_t height = y1 - y0 + 1;

    for (uint16_t i = 0; i < height; i++) {
        if (surface->hline) {
            /* Already clipped so can call HAL directly. */
            surface->hline(&surface, x0, y0 + i, width, color);
        } else {
            hagl_draw_hline(surface, x0, y0 + i, width, color);
        }
    }
}
