
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

#ifndef _HAGL_CHAR_H
#define _HAGL_CHAR_H

#include <stdint.h>

#include "hagl/color.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Draw a single character
 *
 * Output will be clipped to the current clip window. Library itself
 * includes only a couple of fonts. You can find more fonts at:
 *
 * https://github.com/tuupola/embedded-fonts
 *
 * @param surface
 * @param code  unicode code point
 * @param x0
 * @param y0
 * @param color
 * @param font  pointer to a FONTX font
 * @return width of the drawn character
 */
uint8_t
hagl_put_char(void const *surface, wchar_t code, int16_t x0, int16_t y0, hagl_color_t color, const unsigned char *font);

/**
 * Draw a string
 *
 * Output will be clipped to the current clip window. Library itself
 * includes only a couple of fonts. You can find more fonts at:
 *
 * https://github.com/tuupola/embedded-fonts
 *
 * @param surface
 * @param str pointer to an wide char string
 * @param x0
 * @param y0
 * @param color
 * @param font pointer to a FONTX font
 * @return width of the drawn string
 */
uint16_t
hagl_put_text(void const *surface, const wchar_t *str, int16_t x0, int16_t y0, hagl_color_t color, const unsigned char *font);

/**
 * Extract a glyph into a bitmap
 *
 * This can be used for example for extracting game sprites from fontsets
 * such as UNSCII.
 *
 * @param surface
 * @param code Unicode code point
 * @param color
 * @param bitmap Pointer to a bitmap
 * @param font Pointer to a FONTX font
 * @return Width of the drawn string
 */
uint8_t
hagl_get_glyph(void const *surface, wchar_t code, hagl_color_t color, hagl_bitmap_t *bitmap, const uint8_t *font);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAGL_CHAR_H */
