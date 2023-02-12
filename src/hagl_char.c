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

#include "hagl/color.h"
#include "hagl/bitmap.h"
#include "hagl/blit.h"
#include "hagl.h"
#include "fontx.h"

uint8_t
hagl_get_glyph(void const *_surface, wchar_t code, color_t color, hagl_bitmap_t *bitmap, const uint8_t *font)
{
    const hagl_surface_t *surface = _surface;
    uint8_t status, set;
    fontx_glyph_t glyph;

    status = fontx_glyph(&glyph, code, font);
    if (0 != status) {
        return status;
    }

    /* Initialise bitmap dimensions. */
    bitmap->depth = surface->depth;
    bitmap->width = glyph.width;
    bitmap->height = glyph.height;
    bitmap->pitch = bitmap->width * bitmap->depth / 8;
    bitmap->size = bitmap->pitch * bitmap->height;

    color_t *ptr = (color_t *) bitmap->buffer;

    for (uint8_t y = 0; y < glyph.height; y++) {
        for (uint8_t x = 0; x < glyph.width; x++) {
            set = *(glyph.buffer) & (0x80 >> (x % 8));
            if (set) {
                *(ptr++) = color;
            } else {
                *(ptr++) = 0x0000;
            }
        }
        glyph.buffer += glyph.pitch;
    }

    return 0;
}

uint8_t
hagl_put_char_styled(void const *_surface, wchar_t code, int16_t x0, int16_t y0, const hagl_char_style_t *style)
{
    const hagl_surface_t *surface = _surface;
    uint8_t set, status;
    color_t buffer[HAGL_CHAR_BUFFER_SIZE];
    hagl_bitmap_t bitmap;
    fontx_glyph_t glyph;
    bool reverse;
    color_t background_color;
    color_t foreground_color;

    status = fontx_glyph(&glyph, code, style->font);
    if (0 != status) {
        return 0;
    }

    bitmap.width  = glyph.width;
    bitmap.height = glyph.height;
    bitmap.depth  = surface->depth;
    bitmap_init(&bitmap, (uint8_t *)buffer);

    reverse          = style->mode & HAGL_CHAR_MODE_REVERSE     ? true : false;
    foreground_color = reverse ? style->background_color : style->foreground_color;
    background_color = reverse ? style->foreground_color : style->background_color;

    color_t *ptr = (color_t *) bitmap.buffer;
    for (uint8_t y = 0; y < glyph.height; y++) {
        for (uint8_t x = 0; x < glyph.width; x++) {
            set = *(glyph.buffer + x / 8) & (0x80 >> (x % 8));
            if (style->mode & HAGL_CHAR_MODE_TRANSPARENT)
            {
                if (set) {
                    *ptr = foreground_color;
                }
            } else {
                if (set) {
                    *ptr = foreground_color;
                } else {
                    *ptr = background_color;
                }
            }
            ptr++;
        }
        glyph.buffer += glyph.pitch;
    }

    if (
        style->scale_x_numerator <= 1 && style->scale_x_denominator <= 1 
        && style->scale_y_numerator <= 1 && style->scale_y_denominator <= 1
    ) {
        hagl_blit(_surface, x0, y0, &bitmap);
        return bitmap.width;
    } else {
        hagl_blit_xywh(
            _surface, 
            x0, y0, 
            bitmap.width  * style->scale_x_numerator / style->scale_x_denominator, 
            bitmap.height * style->scale_y_numerator / style->scale_y_denominator, 
            &bitmap
        );
        return bitmap.width * style->scale_x_numerator / style->scale_x_denominator;
    }
}

uint8_t
hagl_put_char(void const *_surface, wchar_t code, int16_t x0, int16_t y0, color_t color, const uint8_t *font)
{
    hagl_char_style_t style = {
        .font = font,
        .background_color = 0x0000,
        .foreground_color = color,
        .mode = HAGL_CHAR_MODE_OPAQUE
    };
    return hagl_put_char_styled(_surface, code, x0, y0, &style);
}

uint16_t
hagl_put_text_styled(void const *_surface, const wchar_t *str, int16_t x0, int16_t y0, const hagl_char_style_t *style)
{
    wchar_t temp;
    uint8_t status;
    uint16_t original = x0;
    fontx_meta_t meta;

    status = fontx_meta(&meta, style->font);
    if (0 != status) {
        return 0;
    }

    while (*str != 0) {
        temp = *str++;
        if (13 == temp || 10 == temp) {
            x0 = 0;
            y0 += meta.height;
        } else {
            x0 += hagl_put_char_styled(_surface, temp, x0, y0, style);
        }
    }

    /* NB: result is junk if text wrapped through CR or LF */
    return x0 - original;
}

/*
 * Write a string of text by calling hagl_put_char() repeadetly. CR and LF
 * continue from the next line.
 */
uint16_t
hagl_put_text(void const *_surface, const wchar_t *str, int16_t x0, int16_t y0, color_t color, const unsigned char *font)
{
    hagl_char_style_t style = {
        .font = font,
        .background_color = 0x0000,
        .foreground_color = color,
        .mode = HAGL_CHAR_MODE_OPAQUE
    };
    return hagl_put_text_styled(_surface, str, x0, y0, &style);
}
