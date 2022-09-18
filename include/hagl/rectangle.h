
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

#ifndef _HAGL_RECTANGLE_H
#define _HAGL_RECTANGLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <hagl_hal_color.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/**
 * Draw a rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param color
 */
void
hagl_draw_rectangle(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color);

/**
 * Draw a filled rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param color
 */
void
hagl_fill_rectangle(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_RECTANGLE_H */