
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

#ifndef _HAGL_RECTANGLE_H
#define _HAGL_RECTANGLE_H

#include <stdint.h>

#include "hagl/color.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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
hagl_draw_rectangle_xyxy(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, hagl_color_t color);

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
static void inline
hagl_draw_rectangle(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, hagl_color_t color)
{
    hagl_draw_rectangle_xyxy(surface, x0, y0, x1, y1, color);
}

/**
 * Draw a rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param width
 * @param height
 * @param color
 */
static void inline
hagl_draw_rectangle_xywh(void const *surface, int16_t x0, int16_t y0, uint16_t width, uint16_t height, hagl_color_t color)
{
    hagl_draw_rectangle_xyxy(surface, x0, y0, x0 + width - 1, y0 + height - 1, color);
};

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
hagl_fill_rectangle_xyxy(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, hagl_color_t color);

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
static void inline
hagl_fill_rectangle(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, hagl_color_t color)
{
    hagl_fill_rectangle_xyxy(surface, x0, y0, x1, y1, color);
}

/**
 * Draw a filled rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param width
 * @param height
 * @param color
 */
static void inline
hagl_fill_rectangle_xywh(void const *surface, int16_t x0, int16_t y0, uint16_t width, uint16_t height, hagl_color_t color)
{
    hagl_fill_rectangle_xyxy(surface, x0, y0, x0 + width - 1, y0 + height - 1, color);
};

/**
 * Draw a rounded rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param x0
 * @param y0
 * @param r corner radius
 * @param color
 */
void
hagl_draw_rounded_rectangle_xyxy(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, hagl_color_t color);

/**
 * Draw a rounded rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param x0
 * @param y0
 * @param r corner radius
 * @param color
 */
static void inline
hagl_draw_rounded_rectangle(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, hagl_color_t color)
{
    hagl_draw_rounded_rectangle_xyxy(surface, x0, y0, x1, y1, r, color);
}

/**
 * Draw a rounded rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param width
 * @param height
 * @param r corner radius
 * @param color
 */
static void inline
hagl_draw_rounded_rectangle_xywh(void const *surface, int16_t x0, int16_t y0, uint16_t width, uint16_t height, int16_t r, hagl_color_t color)
{
    hagl_draw_rounded_rectangle_xyxy(surface, x0, y0, x0 + width - 1, y0 + height - 1, r, color);
}

/**
 * Draw a filled rounded rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param r corner radius
 * @param color
 */
void
hagl_fill_rounded_rectangle_xyxy(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, hagl_color_t color);

/**
 * Draw a filled rounded rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param x0
 * @param y0
 * @param r corner radius
 * @param color
 */
static void inline
hagl_fill_rounded_rectangle(void const *surface, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, hagl_color_t color)
{
    hagl_fill_rounded_rectangle_xyxy(surface, x0, y0, x1, y1, r, color);
}

/**
 * Draw a filled rounded rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param width
 * @param height
 * @param r corner radius
 * @param color
 */
static void inline
hagl_fill_rounded_rectangle_xywh(void const *surface, int16_t x0, int16_t y0, uint16_t width, uint16_t height, int16_t r, hagl_color_t color)
{
    hagl_fill_rounded_rectangle_xyxy(surface, x0, y0, x0 + width - 1, y0 + height - 1, r, color);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAGL_RECTANGLE_H */