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

#include "bitmap.h"
#include "rgb332.h"
#include "rgb565.h"
#include "fontx.h"
#include "clip.h"
#include "tjpgd.h"
#include "window.h"
#include "hagl.h"
#include "hagl_hal.h"

typedef struct {
    FILE *fp;
    int16_t x0;
    int16_t y0;
} tjpgd_iodev_t;

static window_t clip_window = {
    .x0 = 0,
    .y0 = 0,
    .x1 = DISPLAY_WIDTH - 1,
    .y1 = DISPLAY_HEIGHT - 1,
};

void hagl_set_clip_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    clip_window.x0 = x0;
    clip_window.y0 = y0;
    clip_window.x1 = x1;
    clip_window.y1 = y1;
}

void hagl_put_pixel(int16_t x0, int16_t y0, color_t color)
{
    /* x0 or y0 is before the edge, nothing to do. */
    if ((x0 < clip_window.x0) || (y0 < clip_window.y0))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > clip_window.x1) || (y0 > clip_window.y1)) {
        return;
    }

    /* If still in bounds set the pixel. */
    hagl_hal_put_pixel(x0, y0, color);
}

color_t hagl_get_pixel(int16_t x0, int16_t y0)
{
    /* x0 or y0 is before the edge, nothing to do. */
    if ((x0 < clip_window.x0) || (y0 < clip_window.y0))  {
        return hagl_color(0, 0, 0);
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > clip_window.x1) || (y0 > clip_window.y1)) {
        return hagl_color(0, 0, 0);
    }

#ifdef HAGL_HAS_HAL_GET_PIXEL
    return hagl_hal_get_pixel(x0, y0);
#else
    return hagl_color(0, 0, 0);
#endif /* HAGL_HAS_HAL_GET_PIXEL */
}

void hagl_draw_hline(int16_t x0, int16_t y0, uint16_t w, color_t color) {
#ifdef HAGL_HAS_HAL_HLINE
    int16_t width = w;

    /* x0 or y0 is over the edge, nothing to do. */
    if ((x0 > clip_window.x1) || (y0 > clip_window.y1) || (y0 < clip_window.y0))  {
        return;
    }

    /* x0 is left of clip window, ignore start part. */
    if (x0 < clip_window.x0) {
        width = width + x0;
        x0 = clip_window.x0;
    }

    /* Everything outside clip window, nothing to do. */
    if (width < 0)  {
        return;
    }

    /* Cut anything going over right edge of clip window. */
    if (((x0 + width) > clip_window.x1)) {
        width = width - (x0 + width - clip_window.x1);
    }

    hagl_hal_hline(x0, y0, width, color);
#else
    hagl_draw_line(x0, y0, x0 + w, y0, color);
#endif
}

/*
 * Draw a vertical line with given color. If HAL supports it uses
 * hardware vline drawing. If not falls back to vanilla line drawing.
 */
void hagl_draw_vline(int16_t x0, int16_t y0, uint16_t h, color_t color) {
#ifdef HAGL_HAS_HAL_VLINE
    int16_t height = h;

    /* x0 or y0 is over the edge, nothing to do. */
    if ((x0 > clip_window.x1) || (x0 < clip_window.x0) || (y0 > clip_window.y1))  {
        return;
    }

    /* y0 is top of clip window, ignore start part. */
    if (y0 < clip_window.y0) {
        height = height + y0;
        y0 = clip_window.y0;
    }

    /* Everything outside clip window, nothing to do. */
    if (height < 0)  {
        return;
    }

    /* Cut anything going over right edge. */
    if (((y0 + height) > clip_window.y1))  {
        height = height - (y0 + height - clip_window.y1);
    }

    hagl_hal_vline(x0, y0, height, color);
#else
    hagl_draw_line(x0, y0, x0, y0 + h, color);
#endif
}

/*
 * Draw a line using Bresenham's algorithm with given color.
 */
void hagl_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color)
{
    /* Clip coordinates to fit clip window. */
    if (false == clip_line(&x0, &y0, &x1, &y1, clip_window)) {
        return;
    }

    int16_t dx;
    int16_t sx;
    int16_t dy;
    int16_t sy;
    int16_t err;
    int16_t e2;

    dx = ABS(x1 - x0);
    sx = x0 < x1 ? 1 : -1;
    dy = ABS(y1 - y0);
    sy = y0 < y1 ? 1 : -1;
    err = (dx > dy ? dx : -dy) / 2;

    while (1) {
        hagl_put_pixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) {
            break;
        };

        e2 = err + err;

        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
}

/*
 * Draw a rectangle with given color.
 */
void hagl_draw_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color)
{
    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < clip_window.x0) || (y1 < clip_window.y0))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > clip_window.x1) || (y0 > clip_window.y1)) {
        return;
    }

    uint16_t width = x1 - x0 + 1;
    uint16_t height = y1 - y0 + 1;

    hagl_draw_hline(x0, y0, width, color);
    hagl_draw_hline(x0, y1, width, color);
    hagl_draw_vline(x0, y0, height, color);
    hagl_draw_vline(x1, y0, height, color);
}

/*
 * Draw a filled rectangle with given color.
 */
void hagl_fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color)
{
    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < clip_window.x0) || (y1 < clip_window.y0))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > clip_window.x1) || (y0 > clip_window.y1)) {
        return;
    }

    x0 = max(x0, clip_window.x0);
    y0 = max(y0, clip_window.y0);
    x1 = min(x1, clip_window.x1);
    y1 = min(y1, clip_window.y1);

    uint16_t width = x1 - x0 + 1;
    uint16_t height = y1 - y0 + 1;

    for (uint16_t i = 0; i < height; i++) {
#ifdef HAGL_HAS_HAL_HLINE
        /* Already clipped so can call HAL directly. */
        hagl_hal_hline(x0, y0 + i, width, color);
#else
        hagl_draw_hline(x0, y0 + i, width, color);
#endif
    }
}

uint8_t hagl_get_glyph(wchar_t code, color_t color, bitmap_t *bitmap, const uint8_t *font)
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

uint8_t hagl_put_char(wchar_t code, int16_t x0, int16_t y0, color_t color, const uint8_t *font)
{
    uint8_t set, status;
    color_t buffer[HAGL_CHAR_BUFFER_SIZE];
    bitmap_t bitmap;
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

    hagl_blit(x0, y0, &bitmap);

    return bitmap.width;
}

/*
 * Write a string of text by calling hagl_put_char() repeadetly. CR and LF
 * continue from the next line.
 */

uint16_t hagl_put_text(const wchar_t *str, int16_t x0, int16_t y0, color_t color, const unsigned char *font)
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
            x0 += hagl_put_char(temp, x0, y0, color, font);
        }
    } while (*str != 0);

    return x0 - original;
}

/*
 * Blits a bitmap to a destination hardcoded in the HAL driver. Destination
 * parameter is left out intentionally to keep the API simpler. If you need
 * configurable source and destination see the file blit.c.
 *
 * TODO: Handle transparency.
 */

void hagl_blit(int16_t x0, int16_t y0, bitmap_t *source) {
#ifdef HAGL_HAS_HAL_BLIT
    /* Check if bitmap is inside clip windows bounds */
    if (
        (x0 < clip_window.x0) ||
        (y0 < clip_window.y0) ||
        (x0 + source->width > clip_window.x1) ||
        (y0 + source->height > clip_window.y1)
    ) {
        /* Out of bounds, use local pixel fallback. */
        color_t color;
        color_t *ptr = (color_t *) source->buffer;

        for (uint16_t y = 0; y < source->height; y++) {
            for (uint16_t x = 0; x < source->width; x++) {
                color = *(ptr++);
                hagl_put_pixel(x0 + x, y0 + y, color);
            }
        }
    } else {
        /* Inside of bounds, can use HAL provided blit. */
        hagl_hal_blit(x0, y0, source);
    }
#else
    color_t color;
    color_t *ptr = (color_t *) source->buffer;

    for (uint16_t y = 0; y < source->height; y++) {
        for (uint16_t x = 0; x < source->width; x++) {
            color = *(ptr++);
            hagl_put_pixel(x0 + x, y0 + y, color);
        }
    }
#endif
};

void hagl_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source) {
#ifdef HAGL_HAS_HAL_SCALE_BLIT
    hagl_hal_scale_blit(x0, y0, w, h, source);
#else
    color_t color;
    color_t *ptr = (color_t *) source->buffer;
    uint32_t x_ratio = (uint32_t)((source->width << 16) / w);
    uint32_t y_ratio = (uint32_t)((source->height << 16) / h);

    for (uint16_t y = 0; y < h; y++) {
        for (uint16_t x = 0; x < w; x++) {
            uint16_t px = ((x * x_ratio) >> 16);
            uint16_t py = ((y * y_ratio) >> 16);
            color = ptr[(uint8_t)((py * source->width) + px)];
            hagl_put_pixel(x0 + x, y0 + y, color);
        }
    }
#endif
};

void hagl_clear_screen() {
#ifdef HAGL_HAS_HAL_CLEAR_SCREEN
    hagl_hal_clear_screen();
#else
    uint16_t x0 = clip_window.x0;
    uint16_t y0 = clip_window.y0;
    uint16_t x1 = clip_window.x1;
    uint16_t y1 = clip_window.y1;

    hagl_set_clip_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT -1);
    hagl_fill_rectangle(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT -1, 0x00);
    hagl_set_clip_window(x0, y0, x1, y1);
#endif
}

void hagl_clear_clip_window() {
    hagl_fill_rectangle(
        clip_window.x0, clip_window.y0, clip_window.x1, clip_window.y1,
        0x00
    );
}

void hagl_draw_circle(int16_t xc, int16_t yc, int16_t r, color_t color) {
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    hagl_put_pixel(xc + x, yc + y, color);
    hagl_put_pixel(xc - x, yc + y, color);
    hagl_put_pixel(xc + x, yc - y, color);
    hagl_put_pixel(xc - x, yc - y, color);
    hagl_put_pixel(xc + y, yc + x, color);
    hagl_put_pixel(xc - y, yc + x, color);
    hagl_put_pixel(xc + y, yc - x, color);
    hagl_put_pixel(xc - y, yc - x, color);

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        hagl_put_pixel(xc + x, yc + y, color);
        hagl_put_pixel(xc - x, yc + y, color);
        hagl_put_pixel(xc + x, yc - y, color);
        hagl_put_pixel(xc - x, yc - y, color);
        hagl_put_pixel(xc + y, yc + x, color);
        hagl_put_pixel(xc - y, yc + x, color);
        hagl_put_pixel(xc + y, yc - x, color);
        hagl_put_pixel(xc - y, yc - x, color);
    }
}

void hagl_fill_circle(int16_t x0, int16_t y0, int16_t r, color_t color) {
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x) {
        hagl_draw_hline(x0 - x, y0 + y, x * 2, color);
        hagl_draw_hline(x0 - x, y0 - y, x * 2, color);
        hagl_draw_hline(x0 - y, y0 + x, y * 2, color);
        hagl_draw_hline(x0 - y, y0 - x, y * 2, color);
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void hagl_draw_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, color_t color) {
    int16_t wx, wy;
    int32_t xa, ya;
    int32_t t;
    int32_t asq = a * a;
    int32_t bsq = b * b;

    hagl_put_pixel(x0, y0 + b, color);
    hagl_put_pixel(x0, y0 - b, color);

    wx = 0;
    wy = b;
    xa = 0;
    ya = asq * 2 * b;
    t = asq / 4 - asq * b;

    while (1) {
        t += xa + bsq;

        if (t >= 0) {
            ya -= asq * 2;
            t -= ya;
            wy--;
        }

        xa += bsq * 2;
        wx++;

        if (xa >= ya) {
            break;
        }

        hagl_put_pixel(x0 + wx, y0 - wy, color);
        hagl_put_pixel(x0 - wx, y0 - wy, color);
        hagl_put_pixel(x0 + wx, y0 + wy, color);
        hagl_put_pixel(x0 - wx, y0 + wy, color);
    }

    hagl_put_pixel(x0 + a, y0, color);
    hagl_put_pixel(x0 - a, y0, color);

    wx = a;
    wy = 0;
    xa = bsq * 2 * a;

    ya = 0;
    t = bsq / 4 - bsq * a;

    while (1) {
        t += ya + asq;

        if (t >= 0) {
            xa -= bsq * 2;
            t = t - xa;
            wx--;
        }

        ya += asq * 2;
        wy++;

        if (ya > xa) {
            break;
        }

        hagl_put_pixel(x0 + wx, y0 - wy, color);
        hagl_put_pixel(x0 - wx, y0 - wy, color);
        hagl_put_pixel(x0 + wx, y0 + wy, color);
        hagl_put_pixel(x0 - wx, y0 + wy, color);
    }
}

void hagl_fill_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, color_t color) {
    int16_t wx, wy;
    int32_t xa, ya;
    int32_t t;
    int32_t asq = a * a;
    int32_t bsq = b * b;

    hagl_put_pixel(x0, y0 + b, color);
    hagl_put_pixel(x0, y0 - b, color);

    wx = 0;
    wy = b;
    xa = 0;
    ya = asq * 2 * b;
    t = asq / 4 - asq * b;

    while (1) {
        t += xa + bsq;

        if (t >= 0) {
            ya -= asq * 2;
            t -= ya;
            wy--;
        }

        xa += bsq * 2;
        wx++;

        if (xa >= ya) {
            break;
        }

        hagl_draw_hline(x0 - wx, y0 - wy, wx * 2, color);
        hagl_draw_hline(x0 - wx, y0 + wy, wx * 2, color);
    }

    hagl_draw_hline(x0 - a, y0, a * 2, color);

    wx = a;
    wy = 0;
    xa = bsq * 2 * a;

    ya = 0;
    t = bsq / 4 - bsq * a;

    while (1) {
        t += ya + asq;

        if (t >= 0) {
            xa -= bsq * 2;
            t = t - xa;
            wx--;
        }

        ya += asq * 2;
        wy++;

        if (ya > xa) {
            break;
        }

        hagl_draw_hline(x0 - wx, y0 - wy, wx * 2, color);
        hagl_draw_hline(x0 - wx, y0 + wy, wx * 2, color);
    }
}


void hagl_draw_polygon(int16_t amount, int16_t *vertices, color_t color) {

    for(int16_t i = 0; i < amount - 1; i++) {
        hagl_draw_line(
            vertices[(i << 1 ) + 0],
            vertices[(i << 1 ) + 1],
            vertices[(i << 1 ) + 2],
            vertices[(i << 1 ) + 3],
            color
        );
    }
    hagl_draw_line(
        vertices[0],
        vertices[1],
        vertices[(amount <<1 ) - 2],
        vertices[(amount <<1 ) - 1],
        color
    );
}

/* Adapted from  http://alienryderflex.com/polygon_fill/ */
void hagl_fill_polygon(int16_t amount, int16_t *vertices, color_t color) {
    uint16_t nodes[64];
    int16_t y;

    float x0;
    float y0;
    float x1;
    float y1;

    int16_t miny = DISPLAY_HEIGHT;
    int16_t maxy = 0;

    for (uint8_t i = 0; i < amount; i++) {
        if (miny > vertices[(i << 1) + 1]) {
            miny = vertices[(i << 1) + 1];
        }
        if (maxy < vertices[(i << 1) + 1]) {
            maxy = vertices[(i << 1) + 1];
        }
    }

    /*  Loop through the rows of the image. */
    for (y = miny; y < maxy; y++) {

        /*  Build a list of nodes. */
        int16_t count = 0;
        int16_t j = amount - 1;

        for (int16_t i = 0; i < amount; i++) {
            x0 = vertices[(i << 1) + 0];
            y0 = vertices[(i << 1) + 1];
            x1 = vertices[(j << 1) + 0];
            y1 = vertices[(j << 1) + 1];

            if (
                (y0 < (float)y && y1 >= (float)y) ||
                (y1 < (float)y && y0 >= (float)y)
            ) {
                nodes[count] = (int16_t)(x0 + (y - y0) / (y1 - y0) * (x1 - x0));
                count++;
            }
            j = i;
        }

        /* Sort the nodes, via a simple “Bubble” sort. */
        int16_t i = 0;
        while (i < count - 1) {
            if (nodes[i] > nodes[i + 1]) {
                int16_t swap = nodes[i];
                nodes[i] = nodes[i + 1];
                nodes[i + 1] = swap;
                if (i) {
                    i--;
                }
            } else {
                i++;
            }
        }

        /* Draw lines between nodes. */
        for (int16_t i = 0; i < count; i += 2) {
            int16_t width = nodes[i + 1] - nodes[i];
            hagl_draw_hline(nodes[i], y, width, color);
        }
    }
}

void hagl_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, color_t color) {
    int16_t vertices[6] = {x0, y0, x1, y1, x2, y2};
    hagl_draw_polygon(3, vertices, color);
};

void hagl_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, color_t color) {
    int16_t vertices[6] = {x0, y0, x1, y1, x2, y2};
    hagl_fill_polygon(3, vertices, color);
}

void hagl_draw_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, color_t color) {

    uint16_t width, height;
    int16_t x, y, d;

    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < clip_window.x0) || (y1 < clip_window.y0))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > clip_window.x1) || (y0 > clip_window.y1)) {
        return;
    }

    /* Max radius is half of shortest edge. */
    width = x1 - x0 + 1;
    height = y1 - y0 + 1;
    r = min(r, min(width / 2, height / 2));

    hagl_draw_hline(x0 + r, y0, width - 2 * r, color);
    hagl_draw_hline(x0 + r, y1, width - 2 * r, color);
    hagl_draw_vline(x0, y0 + r, height - 2 * r, color);
    hagl_draw_vline(x1, y0 + r, height - 2 * r, color);

    x = 0;
    y = r;
    d = 3 - 2 * r;

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        /* Top right */
        hagl_put_pixel(x1 - r + x, y0 + r - y, color);
        hagl_put_pixel(x1 - r + y, y0 + r - x, color);

        /* Top left */
        hagl_put_pixel(x0 + r - x, y0 + r - y, color);
        hagl_put_pixel(x0 + r - y, y0 + r - x, color);

        /* Bottom right */
        hagl_put_pixel(x1 - r + x, y1 - r + y, color);
        hagl_put_pixel(x1 - r + y, y1 - r + x, color);

        /* Bottom left */
        hagl_put_pixel(x0 + r - x, y1 - r + y, color);
        hagl_put_pixel(x0 + r - y, y1 - r + x, color);
    }
};

void hagl_fill_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, color_t color) {

    uint16_t width, height;
    int16_t rx0, ry0, rx1, x, y, d;

    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < clip_window.x0) || (y1 < clip_window.y0))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > clip_window.x1) || (y0 > clip_window.y1)) {
        return;
    }

    /* Max radius is half of shortest edge. */
    width = x1 - x0 + 1;
    height = y1 - y0 + 1;
    r = min(r, min(width / 2, height / 2));

    x = 0;
    y = r;
    d = 3 - 2 * r;

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        /* Top  */
        ry0 = y0 + r - x;
        rx0 = x0 + r - y;
        rx1 = x1 - r + y;
        width = rx1 -  rx0;
        hagl_draw_hline(rx0, ry0, width, color);

        ry0 = y0 + r - y;
        rx0 = x0 + r - x;
        rx1 = x1 - r + x;
        width = rx1 -  rx0;
        hagl_draw_hline(rx0, ry0, width, color);

        /* Bottom */
        ry0 = y1 - r + y;
        rx0 = x0 + r - x;
        rx1 = x1 - r + x;
        width = rx1 -  rx0;
        hagl_draw_hline(rx0, ry0, width, color);

        ry0 = y1 - r + x;
        rx0 = x0 + r - y;
        rx1 = x1 - r + y;
        width = rx1 -  rx0;
        hagl_draw_hline(rx0, ry0, width, color);
    }

    /* Center */
    hagl_fill_rectangle(x0, y0 + r, x1, y1 - r, color);
};



static uint16_t tjpgd_data_reader(JDEC *decoder, uint8_t *buffer, uint16_t size)
{
    tjpgd_iodev_t *device = (tjpgd_iodev_t *)decoder->device;

    if (buffer) {
        /* Read bytes from input stream. */
        return (uint16_t)fread(buffer, 1, size, device->fp);
    } else {
        /* Skip bytes from input stream. */
        return fseek(device->fp, size, SEEK_CUR) ? 0 : size;
    }
}

static uint16_t tjpgd_data_writer(JDEC* decoder, void* bitmap, JRECT* rectangle)
{
    tjpgd_iodev_t *device = (tjpgd_iodev_t *)decoder->device;
    uint8_t width = (rectangle->right - rectangle->left) + 1;
    uint8_t height = (rectangle->bottom - rectangle->top) + 1;

    bitmap_t block = {
        .width = width,
        .height = height,
        .depth = DISPLAY_DEPTH,
        .pitch = width * (DISPLAY_DEPTH / 8),
        .size =  width * (DISPLAY_DEPTH / 8) * height,
        .buffer = (uint8_t *)bitmap
    };

    hagl_blit(rectangle->left + device->x0, rectangle->top + device->y0, &block);

    return 1;
}

uint32_t hagl_load_image(int16_t x0, int16_t y0, const char *filename)
{
    uint8_t work[3100];
    JDEC decoder;
    JRESULT result;
    tjpgd_iodev_t device;

    device.x0 = x0;
    device.y0 = y0;
    device.fp = fopen(filename, "rb");

    if (!device.fp) {
        return HAGL_ERR_FILE_IO;
    }
    result = jd_prepare(&decoder, tjpgd_data_reader, work, 3100, (void *)&device);
    if (result == JDR_OK) {
        result = jd_decomp(&decoder, tjpgd_data_writer, 0);
        if (JDR_OK != result) {
            fclose(device.fp);
            return HAGL_ERR_TJPGD + result;
        }
    } else {
        fclose(device.fp);
        return HAGL_ERR_TJPGD + result;
    }

    fclose(device.fp);
    return HAGL_OK;
}

color_t hagl_color(uint8_t r, uint8_t g, uint8_t b)
{
#ifdef HAGL_HAS_HAL_COLOR
    return hagl_hal_color(r, g, b);
#else
    return rgb565(r, g, b);
#endif
}

bitmap_t *hagl_init() {
#ifdef HAGL_HAS_HAL_INIT
    bitmap_t *bb = hagl_hal_init();
    hagl_clear_screen();
    return bb;
#else
    hagl_clear_screen();
    return NULL;
#endif
};

size_t hagl_flush() {
#ifdef HAGL_HAS_HAL_FLUSH
    return hagl_hal_flush();
#else
    return 0;
#endif
};

void hagl_close() {
#ifdef HAGL_HAS_HAL_CLOSE
    hagl_hal_close();
#else
#endif
};
