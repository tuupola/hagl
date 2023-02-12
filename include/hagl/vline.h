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

#ifndef _HAGL_VLINE_H
#define _HAGL_VLINE_H

#include <stdint.h>
#include <stdlib.h>

#include "hagl/color.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Draw a vertical line
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param height
 * @param color
 */
void
hagl_draw_vline_xyh(void const *surface, int16_t x0, int16_t y0, uint16_t height, hagl_color_t color);

/**
 * Draw a vertical line
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param y1
 * @param color
 */
static inline void
hagl_draw_vline_xyy(void const *surface, int16_t x0, int16_t y0, int16_t y1, hagl_color_t color)
{
    hagl_draw_vline_xyh(surface, x0, y0, abs(y1 - y0) + 1, color);
}

/**
 * Draw a vertical line
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param height
 * @param color
 */
static inline void
hagl_draw_vline(void const *surface, int16_t x0, int16_t y0, uint16_t height, hagl_color_t color)
{
    hagl_draw_vline_xyh(surface, x0, y0, height, color);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAGL_VLINE_H */