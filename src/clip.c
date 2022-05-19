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

https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

*/

#include <stdint.h>
#include <stdbool.h>

#include "window.h"

static const uint8_t INSIDE = 0b0000;
static const uint8_t LEFT = 0b0001;
static const uint8_t RIGHT = 0b0010;
static const uint8_t BOTTOM = 0b0100;
static const uint8_t TOP = 0b1000;

static uint8_t code(int16_t x0, int16_t y0, window_t window)
{
    uint8_t code = INSIDE;

    if (x0 < window.x0) {
        code |= LEFT;
    } else if (x0 > window.x1) {
        code |= RIGHT;
    } if (y0 < window.y0) {
        code |= BOTTOM;
    } else if (y0 > window.y1) {
        code |= TOP;
    }

    return code;
}

bool clip_line(int16_t *x0, int16_t *y0, int16_t *x1, int16_t *y1, window_t window)
{
    uint8_t code0 = code(*x0, *y0, window);
    uint8_t code1 = code(*x1, *y1, window);

    bool accept = false;

    while (true) {
        if (!(code0 | code1)) {
            /* Both endpoints inside clipping window, trivial accept. */
            accept = true;
            break;
        } else if (code0 & code1) {
            /* Both endpoints outside clipping window, trivial reject. */
            break;
        } else {
            /* Part of line inside clipping window, nontrivial situation. */

            int16_t x = 0;
            int16_t y = 0;
            uint8_t code3 = code0 ? code0 : code1;

            /* Find intersection point. */
            /* slope = (y1 - y0) / (x1 - x0) */
            /* x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax */
            /* y = y0 + slope * (xm - x0), where xm is xmin or xmax */
            if (code3 & TOP) {
                x = *x0 + (*x1 - *x0) * (window.y1 - *y0) / (*y1 - *y0);
                y = window.y1;
            } else if (code3 & BOTTOM) {
                x = *x0 + (*x1 - *x0) * (window.y0 - *y0) / (*y1 - *y0);
                y = window.y0;
            } else if (code3 & RIGHT) {
                y = *y0 + (*y1 - *y0) * (window.x1 - *x0) / (*x1 - *x0);
                x = window.x1;
            }  else if (code3 & LEFT) {
                y = *y0 + (*y1 - *y0) * (window.x0 - *x0) / (*x1 - *x0);
                x = window.x0;
            }

            /* Replace the outside point with the intersection point. */
            if (code3 == code0) {
                *x0 = x;
                *y0 = y;
                code0 = code(*x0, *y0, window);
            } else {
                *x1 = x;
                *y1 = y;
                code1 = code(*x1, *y1, window);
            }
        }
    }

    return accept;
}