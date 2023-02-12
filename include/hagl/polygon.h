
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

#ifndef _HAGL_POLYGON_H
#define _HAGL_POLYGON_H

#include <stdint.h>

#include "hagl/color.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Draw a polygon
 *
 * Output will be clipped to the current clip window. Polygon does
 * not need to be convex. They can also be concave or complex.
 *
 * hagl_color_t color = hagl_color(0, 255, 0);
 * int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
 * hagl_draw_polygon(5, vertices, color);
 *
 * @param surface
 * @param amount number of vertices
 * @param vertices pointer to (an array) of vertices
 * @param color
 */
void
hagl_draw_polygon(void const *surface, int16_t amount, int16_t *vertices, hagl_color_t color);

/**
 * Draw a filled polygon
 *
 * Output will be clipped to the current clip window. Polygon does
 * not need to be convex. They can also be concave or complex.
 *
 * hagl_color_t color = hagl_color(0, 255, 0);
 * int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
 * hagl_draw_polygon(5, vertices, color);
 *
 * @param surface
 * @param amount number of vertices
 * @param vertices pointer to (an array) of vertices
 * @param color
 */
void
hagl_fill_polygon(void const *surface, int16_t amount, int16_t *vertices, hagl_color_t color);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAGL_POLYGON_H */