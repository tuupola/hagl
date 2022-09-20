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

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#include "rgb332.h"
#include "rgb565.h"
#include "fontx.h"
#include "hagl/bitmap.h"
#include "hagl/clip.h"
#include "hagl/window.h"

#include "hagl.h"
#include "hagl_hal.h"

void
hagl_set_clip_window(void *_surface, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    hagl_surface_t *surface = _surface;

    surface->clip.x0 = x0;
    surface->clip.y0 = y0;
    surface->clip.x1 = x1;
    surface->clip.y1 = y1;
}

uint8_t hagl_get_glyph(void const *surface, wchar_t code, color_t color, hagl_bitmap_t *bitmap, const uint8_t *font)
{
    uint8_t status, set;
    fontx_glyph_t glyph;

    status = fontx_glyph(&glyph, code, font);

    if (0 != status) {
        return status;
    }

    /* Initialise bitmap dimensions. */
    bitmap->depth = DISPLAY_DEPTH,
    bitmap->width = glyph.width,
    bitmap->height = glyph.height,
    bitmap->pitch = bitmap->width * (bitmap->depth / 8);
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

uint8_t hagl_put_char(void const *surface, wchar_t code, int16_t x0, int16_t y0, color_t color, const uint8_t *font)
{
    uint8_t set, status;
    color_t buffer[HAGL_CHAR_BUFFER_SIZE];
    hagl_bitmap_t bitmap;
    fontx_glyph_t glyph;

    status = fontx_glyph(&glyph, code, font);

    if (0 != status) {
        return 0;
    }

    bitmap.width = glyph.width,
    bitmap.height = glyph.height,
    bitmap.depth = DISPLAY_DEPTH,

    bitmap_init(&bitmap, (uint8_t *)buffer);

    color_t *ptr = (color_t *) bitmap.buffer;

    for (uint8_t y = 0; y < glyph.height; y++) {
        for (uint8_t x = 0; x < glyph.width; x++) {
            set = *(glyph.buffer + x / 8) & (0x80 >> (x % 8));
            if (set) {
                *(ptr++) = color;
            } else {
                *(ptr++) = 0x0000;
            }
        }
        glyph.buffer += glyph.pitch;
    }

    hagl_blit(surface, x0, y0, &bitmap);

    return bitmap.width;
}

/*
 * Write a string of text by calling hagl_put_char(backend, ) repeadetly. CR and LF
 * continue from the next line.
 */

uint16_t hagl_put_text(void const *surface, const wchar_t *str, int16_t x0, int16_t y0, color_t color, const unsigned char *font)
{
    wchar_t temp;
    uint8_t status;
    uint16_t original = x0;
    fontx_meta_t meta;

    status = fontx_meta(&meta, font);
    if (0 != status) {
        return 0;
    }

    do {
        temp = *str++;
        if (13 == temp || 10 == temp) {
            x0 = 0;
            y0 += meta.height;
        } else {
            x0 += hagl_put_char(surface, temp, x0, y0, color, font);
        }
    } while (*str != 0);

    return x0 - original;
}

void hagl_clear(void *_surface) {
    hagl_surface_t *surface = _surface;

    uint16_t x0 = surface->clip.x0;
    uint16_t y0 = surface->clip.y0;
    uint16_t x1 = surface->clip.x1;
    uint16_t y1 = surface->clip.y1;

    hagl_set_clip_window(surface, 0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT -1);
    hagl_fill_rectangle(surface, 0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT -1, 0x00);
    hagl_set_clip_window(surface, x0, y0, x1, y1);
}

void hagl_clear_clip_window(hagl_surface_t *surface) {
    hagl_fill_rectangle(
        surface,
        surface->clip.x0, surface->clip.y0, surface->clip.x1, surface->clip.y1,
        0x00
    );
}

hagl_backend_t *hagl_init(void) {
    static hagl_backend_t backend;
    memset(&backend, 0, sizeof(hagl_backend_t));

    hagl_hal_init(&backend);
    hagl_set_clip_window(&backend, 0, 0,  DISPLAY_WIDTH - 1,  DISPLAY_HEIGHT - 1);
    return &backend;
};

size_t hagl_flush(hagl_backend_t *backend) {
    if (backend->flush) {
        return backend->flush(backend);
    }
    return 0;
};

void hagl_close(hagl_backend_t *backend) {
    if (backend->close) {
        backend->close(backend);
    }
};
