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

#include "hagl/surface.h"
#include "hagl/hline.h"
#include "hagl/vline.h"
#include "hagl/pixel.h"
#include "hagl/color.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void
hagl_draw_rectangle_xyxy(void const *_surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, hagl_color_t color)
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
hagl_fill_rectangle_xyxy(void const *_surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, hagl_color_t color)
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

void
hagl_draw_rounded_rectangle_xyxy(void const *_surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, hagl_color_t color)
{
    const hagl_surface_t *surface = _surface;

    uint16_t width, height;
    int16_t x, y, d;

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

    /* Max radius is half of shortest edge. */
    width = x1 - x0 + 1;
    height = y1 - y0 + 1;
    r = MIN(r, MIN(width / 2, height / 2));

    hagl_draw_hline(surface, x0 + r, y0, width - 2 * r, color);
    hagl_draw_hline(surface, x0 + r, y1, width - 2 * r, color);
    hagl_draw_vline(surface, x0, y0 + r, height - 2 * r, color);
    hagl_draw_vline(surface, x1, y0 + r, height - 2 * r, color);

    x = 0;
    y = r;
    d = 3 - 2 * r;

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        /* Top right */
        hagl_put_pixel(surface, x1 - r + x, y0 + r - y, color);
        hagl_put_pixel(surface, x1 - r + y, y0 + r - x, color);

        /* Top left */
        hagl_put_pixel(surface, x0 + r - x, y0 + r - y, color);
        hagl_put_pixel(surface, x0 + r - y, y0 + r - x, color);

        /* Bottom right */
        hagl_put_pixel(surface, x1 - r + x, y1 - r + y, color);
        hagl_put_pixel(surface, x1 - r + y, y1 - r + x, color);

        /* Bottom left */
        hagl_put_pixel(surface, x0 + r - x, y1 - r + y, color);
        hagl_put_pixel(surface, x0 + r - y, y1 - r + x, color);
    }
};

void
hagl_fill_rounded_rectangle_xyxy(void const *_surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, hagl_color_t color)
{
    const hagl_surface_t *surface = _surface;

    uint16_t width, height;
    int16_t rx0, ry0, rx1, x, y, d;

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

    /* Max radius is half of shortest edge. */
    width = x1 - x0 + 1;
    height = y1 - y0 + 1;
    r = MIN(r, MIN(width / 2, height / 2));

    x = 0;
    y = r;
    d = 3 - 2 * r;

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        /* Top  */
        ry0 = y0 + r - x;
        rx0 = x0 + r - y;
        rx1 = x1 - r + y;
        width = rx1 -  rx0;
        hagl_draw_hline(surface, rx0, ry0, width, color);

        ry0 = y0 + r - y;
        rx0 = x0 + r - x;
        rx1 = x1 - r + x;
        width = rx1 -  rx0;
        hagl_draw_hline(surface, rx0, ry0, width, color);

        /* Bottom */
        ry0 = y1 - r + y;
        rx0 = x0 + r - x;
        rx1 = x1 - r + x;
        width = rx1 -  rx0;
        hagl_draw_hline(surface, rx0, ry0, width, color);

        ry0 = y1 - r + x;
        rx0 = x0 + r - y;
        rx1 = x1 - r + y;
        width = rx1 -  rx0;
        hagl_draw_hline(surface, rx0, ry0, width, color);
    }

    /* Center */
    hagl_fill_rectangle_xyxy(surface, x0, y0 + r, x1, y1 - r, color);
};
