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
#include "hagl/hline.h"

void
hagl_draw_circle(void const *surface, int16_t xc, int16_t yc, int16_t r, hagl_color_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    hagl_put_pixel(surface, xc + x, yc + y, color);
    hagl_put_pixel(surface, xc - x, yc + y, color);
    hagl_put_pixel(surface, xc + x, yc - y, color);
    hagl_put_pixel(surface, xc - x, yc - y, color);
    hagl_put_pixel(surface, xc + y, yc + x, color);
    hagl_put_pixel(surface, xc - y, yc + x, color);
    hagl_put_pixel(surface, xc + y, yc - x, color);
    hagl_put_pixel(surface, xc - y, yc - x, color);

    while (y >= x) {
        if (d > 0) {
            d = d + 4 * (x - y) + 10;
            y--;
            x++;
        } else {
            d = d + 4 * x + 6;
            x++;
        }

        hagl_put_pixel(surface, xc + x, yc + y, color);
        hagl_put_pixel(surface, xc - x, yc + y, color);
        hagl_put_pixel(surface, xc + x, yc - y, color);
        hagl_put_pixel(surface, xc - x, yc - y, color);
        hagl_put_pixel(surface, xc + y, yc + x, color);
        hagl_put_pixel(surface, xc - y, yc + x, color);
        hagl_put_pixel(surface, xc + y, yc - x, color);
        hagl_put_pixel(surface, xc - y, yc - x, color);
    }
}

void
hagl_fill_circle(void const *surface, int16_t x0, int16_t y0, int16_t r, hagl_color_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x) {
        hagl_draw_hline(surface, x0 - x, y0 + y, x * 2, color);
        hagl_draw_hline(surface, x0 - x, y0 - y, x * 2, color);
        hagl_draw_hline(surface, x0 - y, y0 + x, y * 2, color);
        hagl_draw_hline(surface, x0 - y, y0 - x, y * 2, color);

        if (d <= 0) {
            d = d + 4 * x + 6;
            x++;
        } else {
            d = d + 4 * (x - y) + 10;
            x++;
            y--;
        }
    }
}
