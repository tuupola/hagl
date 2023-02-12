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
#include "hagl/polygon.h"

void
hagl_draw_triangle(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, hagl_color_t color)
{
    int16_t vertices[6] = {x0, y0, x1, y1, x2, y2};
    hagl_draw_polygon(surface, 3, vertices, color);
};

void
hagl_fill_triangle(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, hagl_color_t color)
{
    int16_t vertices[6] = {x0, y0, x1, y1, x2, y2};
    hagl_fill_polygon(surface, 3, vertices, color);
}