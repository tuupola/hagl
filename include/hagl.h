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

#ifndef _HAGL_H
#define _HAGL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#include "hagl_hal.h"
#include "bitmap.h"

#define ABS(x)  ((x) > 0 ? (x) : -(x))

#define HAGL_CHAR_BUFFER_SIZE    (16 * 16 * DISPLAY_DEPTH / 2)

#define HAGL_OK                  (0)
#define HAGL_ERR_GENERAL         (1)
#define HAGL_ERR_FILE_IO         (2)
#define HAGL_ERR_TJPGD           (100)

/**
 * Put a single pixel
 *
 * Output will be clipped to the current clip window.
 * @param x0
 * @param y0
 * @param color
 */
void hagl_put_pixel(int16_t x0, int16_t y0, color_t color);

/**
 * Get a single pixel
 *
 * Input will be clipped to the current clip window. In case of
 * error or if HAL does not support this feature returns black.
 * @param x0
 * @param y0
 * @return color at the given location
 */
color_t hagl_get_pixel(int16_t x0, int16_t y0);

/**
 * Draw a single character
 *
 * Output will be clipped to the current clip window. Library itself
 * includes only a couple of fonts. You can find more fonts at:
 *
 * https://github.com/tuupola/fonts
 *
 * @param code  unicode code point
 * @param x0
 * @param y0
 * @param color
 * @param font  pointer to a FONTX font
 * @return width of the drawn character
 */
uint8_t hagl_put_char(wchar_t code, int16_t x0, int16_t y0, color_t color, const unsigned char *font);

/**
 * Draw a string
 *
 * Output will be clipped to the current clip window. Library itself
 * includes only a couple of fonts. You can find more fonts at:
 *
 * https://github.com/tuupola/fonts
 *
 * @param str pointer to an wide char string
 * @param x0
 * @param y0
 * @param color
 * @param font pointer to a FONTX font
 * @return width of the drawn string
 */
uint16_t hagl_put_text(const wchar_t *str, int16_t x0, int16_t y0, color_t color, const unsigned char *font);

/**
 * Extract a glyph into a bitmap
 *
 * This can be used for example for extracting game sprites from fontsets
 * such as UNSCII.
 *
 * @param code Unicode code point
 * @param color
 * @param bitmap Pointer to a bitmap
 * @param font Pointer to a FONTX font
 * @return Width of the drawn string
 */
uint8_t hagl_get_glyph(wchar_t code, color_t color, bitmap_t *bitmap, const uint8_t *font);

/**
 * Blit a bitmap to the display
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0
 * @param y0
 * @param source pointer to a bitmap
 */
void hagl_blit(int16_t x0, int16_t y0, bitmap_t *source);

/**
 * Blit and scale a bitmap to the display
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0
 * @param y0
 * @param w target width
 * @param h target height
 * @param source pointer to a bitmap
 */
void hagl_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source);

/**
 * Draw a line
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param color
 */
void hagl_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color);

/**
 * Draw a vertical line
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0
 * @param y0
 * @param width
 * @param color
 */
void hagl_draw_hline(int16_t x0, int16_t y0, uint16_t width, color_t color);

/**
 * Draw a horizontal line
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0
 * @param y0
 * @param height
 * @param color
 */
void hagl_draw_vline(int16_t x0, int16_t y0, uint16_t height, color_t color);

/**
 * Draw a rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param color
 */
void hagl_draw_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color);

/**
 * Draw a filled rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param color
 */
void hagl_fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color);

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
void hagl_draw_circle(int16_t x0, int16_t y0, int16_t r, color_t color);

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
void hagl_fill_circle(int16_t x0, int16_t y0, int16_t r, color_t color);

/**
 * Draw an ellipse
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0 center X
 * @param y0 center Y
 * @param a vertical radius
 * @param b horizontal radius
 * @param color
 */
void hagl_draw_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, color_t color);

/**
 * Draw a filled ellipse
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0 center X
 * @param y0 center Y
 * @param a vertical radius
 * @param b horizontal radius
 * @param color
 */
void hagl_fill_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, color_t color);

/**
 * Draw a polygon
 *
 * Output will be clipped to the current clip window. Polygon does
 * not need to be convex. They can also be concave or complex.
 *
 * color_t color = hagl_color(0, 255, 0);
 * int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
 * hagl_draw_polygon(5, vertices, color);
 *
 * @param amount number of vertices
 * @param vertices pointer to (an array) of vertices
 * @param color
 */
void hagl_draw_polygon(int16_t amount, int16_t *vertices, color_t color);

/**
 * Draw a filled polygon
 *
 * Output will be clipped to the current clip window. Polygon does
 * not need to be convex. They can also be concave or complex.
 *
 * color_t color = hagl_color(0, 255, 0);
 * int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
 * hagl_draw_polygon(5, vertices, color);
 *
 * @param amount number of vertices
 * @param vertices pointer to (an array) of vertices
 * @param color
 */
void hagl_fill_polygon(int16_t amount, int16_t *vertices, color_t color);

/**
 * Draw a triangle
 *
 * Output will be clipped to the current clip window. Internally
 * uses hagl_draw_polygon() to draw the triangle.
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param x2
 * @param y3
 * @param color
 */
void hagl_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, color_t color);

/**
 * Draw a filled triangle
 *
 * Output will be clipped to the current clip window. Internally
 * uses hagl_fill_polygon() to draw the triangle.
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param x2
 * @param y3
 * @param color
 */
void hagl_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, color_t color);

/**
 * Draw a rounded rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0
 * @param y0
 * @param x0
 * @param y0
 * @param r corner radius
 * @param color
 */
void hagl_draw_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, color_t color);

/**
 * Draw a filled rounded rectangle
 *
 * Output will be clipped to the current clip window.
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param r corner radius
 * @param color
 */
void hagl_fill_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, color_t color);

/**
 * Load an image
 *
 * Output will be clipped to the current clip window. Does not do
 * any scaling. Currently supports only baseline jpg images.
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param r corner radius
 * @param color
 */
uint32_t hagl_load_image(int16_t x0, int16_t y0, const char *filename);

/**
 * Set the clip window
 *
 * Clip windows restricts the drawable area. It does not affect
 * the coordinates.
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 */
void hagl_set_clip_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
 * Convert RGB to color
 *
 * Returns color type  defined by the HAL. Most often it is an
 * uint16_t RGB565 color.
 *
 * @return color
 */
color_t hagl_color(uint8_t r, uint8_t g, uint8_t b);

/**
 * Clear area of the current clip window
 */
void hagl_clear_clip_window();

/**
 * Clear the display
 */
void hagl_clear_screen();
bitmap_t *hagl_init();
size_t hagl_flush();
void hagl_close();

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_H */
