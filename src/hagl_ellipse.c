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

#include "hagl/color.h"
#include "hagl/pixel.h"
#include "hagl/hline.h"

void
hagl_draw_ellipse(void const *surface, int16_t x0, int16_t y0, int16_t a, int16_t b, hagl_color_t color)
{
    int16_t wx, wy;
    int32_t xa, ya;
    int32_t t;
    int32_t asq = a * a;
    int32_t bsq = b * b;

    hagl_put_pixel(surface, x0, y0 + b, color);
    hagl_put_pixel(surface, x0, y0 - b, color);

    wx = 0;
    wy = b;
    xa = 0;
    ya = asq * 2 * b;
    t = asq / 4 - asq * b;

    while (1) {
        t += xa + bsq;

        if (t >= 0) {
            ya -= asq * 2;
            t -= ya;
            wy--;
        }

        xa += bsq * 2;
        wx++;

        if (xa >= ya) {
            break;
        }

        hagl_put_pixel(surface, x0 + wx, y0 - wy, color);
        hagl_put_pixel(surface, x0 - wx, y0 - wy, color);
        hagl_put_pixel(surface, x0 + wx, y0 + wy, color);
        hagl_put_pixel(surface, x0 - wx, y0 + wy, color);
    }

    hagl_put_pixel(surface, x0 + a, y0, color);
    hagl_put_pixel(surface, x0 - a, y0, color);

    wx = a;
    wy = 0;
    xa = bsq * 2 * a;

    ya = 0;
    t = bsq / 4 - bsq * a;

    while (1) {
        t += ya + asq;

        if (t >= 0) {
            xa -= bsq * 2;
            t = t - xa;
            wx--;
        }

        ya += asq * 2;
        wy++;

        if (ya > xa) {
            break;
        }

        hagl_put_pixel(surface, x0 + wx, y0 - wy, color);
        hagl_put_pixel(surface, x0 - wx, y0 - wy, color);
        hagl_put_pixel(surface, x0 + wx, y0 + wy, color);
        hagl_put_pixel(surface, x0 - wx, y0 + wy, color);
    }
}

void
hagl_fill_ellipse(void const *surface, int16_t x0, int16_t y0, int16_t a, int16_t b, hagl_color_t color)
{
    int16_t wx, wy;
    int32_t xa, ya;
    int32_t t;
    int32_t asq = a * a;
    int32_t bsq = b * b;

    hagl_put_pixel(surface, x0, y0 + b, color);
    hagl_put_pixel(surface, x0, y0 - b, color);

    wx = 0;
    wy = b;
    xa = 0;
    ya = asq * 2 * b;
    t = asq / 4 - asq * b;

    while (1) {
        t += xa + bsq;

        if (t >= 0) {
            ya -= asq * 2;
            t -= ya;
            wy--;
        }

        xa += bsq * 2;
        wx++;

        if (xa >= ya) {
            break;
        }

        hagl_draw_hline(surface, x0 - wx, y0 - wy, wx * 2, color);
        hagl_draw_hline(surface, x0 - wx, y0 + wy, wx * 2, color);
    }

    hagl_draw_hline(surface, x0 - a, y0, a * 2, color);

    wx = a;
    wy = 0;
    xa = bsq * 2 * a;

    ya = 0;
    t = bsq / 4 - bsq * a;

    while (1) {
        t += ya + asq;

        if (t >= 0) {
            xa -= bsq * 2;
            t = t - xa;
            wx--;
        }

        ya += asq * 2;
        wy++;

        if (ya > xa) {
            break;
        }

        hagl_draw_hline(surface, x0 - wx, y0 - wy, wx * 2, color);
        hagl_draw_hline(surface, x0 - wx, y0 + wy, wx * 2, color);
    }
}
