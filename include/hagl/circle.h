
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

#ifndef _HAGL_CIRCLE_H
#define _HAGL_CIRCLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <hagl_hal_color.h>

/**
 * Draw a circle
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0 center X
 * @param y0 center Y
 * @param r radius
 * @param color
 */
void
hagl_draw_circle(void const *surface, int16_t x0, int16_t y0, int16_t r, color_t color);

/**
 * Draw a filled circle
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0 center X
 * @param y0 center Y
 * @param r radius
 * @param color
 */
void
hagl_fill_circle(void const *surface, int16_t x0, int16_t y0, int16_t r, color_t color);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_CIRCLE_H */