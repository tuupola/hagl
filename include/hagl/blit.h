
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

#ifndef _HAGL_BLIT_H
#define _HAGL_BLIT_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "hagl/bitmap.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Blit a bitmap to a _surface, with optional transparency
 *
 * Output will be clipped to the current clip window.
 *
 * @param _surface
 * @param x0
 * @param y0
 * @param source pointer to a bitmap
 * @param is_transparent
 * @param transparent_color
 */
void
hagl_blit_xy_extended(void const *_surface, uint16_t x0, uint16_t y0, hagl_bitmap_t *source, bool is_transparent, hagl_color_t transparent_color);

static void inline
hagl_blit(void const *_surface, int16_t x0, int16_t y0, hagl_bitmap_t *source)
{
    hagl_blit_xy_extended(_surface, x0, y0, source, false, 0);
};

static void inline
hagl_blit_xy(void const *_surface, int16_t x0, int16_t y0, hagl_bitmap_t *source)
{
    hagl_blit_xy_extended(_surface, x0, y0, source, false, 0);
};

static void inline
hagl_blit_transparent(void const *_surface, int16_t x0, int16_t y0, hagl_bitmap_t *source, hagl_color_t transparent_color)
{
    hagl_blit_xy_extended(_surface, x0, y0, source, true, transparent_color);
};

static void inline
hagl_blit_xy_transparent(void const *_surface, int16_t x0, int16_t y0, hagl_bitmap_t *source, hagl_color_t transparent_color)
{
    hagl_blit_xy_extended(_surface, x0, y0, source, true, transparent_color);
};

void
hagl_blit_xywh_extended(void const *_surface, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, hagl_bitmap_t *source, bool is_transparent, hagl_color_t transparent_color);

static void inline
hagl_blit_xywh(void const *_surface, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, hagl_bitmap_t *source)
{
    hagl_blit_xywh_extended(_surface, x0, y0, w, h, source, false, 0);
};

static void inline
hagl_blit_xyxy(void const *_surface, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, hagl_bitmap_t *source)
{
    hagl_blit_xywh_extended(_surface, x0, y0, abs(x1 - x0) + 1, abs(y1 - y0) + 1, source, false, 0);
};

static void inline
hagl_blit_xywh_transparent(void const *_surface, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, hagl_bitmap_t *source, hagl_color_t transparent_color)
{
    hagl_blit_xywh_extended(_surface, x0, y0, w, h, source, true, transparent_color);
};

static void inline
hagl_blit_xyxy_transparent(void const *_surface, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, hagl_bitmap_t *source, hagl_color_t transparent_color)
{
    hagl_blit_xywh_extended(_surface, x0, y0, abs(x1 - x0) + 1, abs(y1 - y0) + 1, source, true, transparent_color);
};

/**
 * Blit and scale a bitmap to a _surface with transparency
 *
 * Output will be clipped to the current clip window.
 *
 * @param _surface
 * @param x0
 * @param y0
 * @param w target width
 * @param h target height
 * @param source pointer to a bitmap
 * @param transparent_color
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAGL_BLIT_H */
