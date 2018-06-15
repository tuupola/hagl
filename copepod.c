/*

Copyright (c) 2018 Mika Tuupola

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

*/

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bitmap.h"
#include "blit.h"
#include "config.h"
#include "copepod.h"
#include "framebuffer.h"

/*
 * Puts a pixel RGB565 color. This is the only mandatory function HAL must
 * support.
 *
 * TODO: Coordinates checking.
 */
void pod_putpixel(uint16_t x1, uint16_t y1, uint16_t color)
{
    pod_hal_putpixel(x1, y1, color);
}

/*
 * Draw a vertical line with given RGB565 color. If HAL supports it uses
 * hardware hline drawing. If not falls back to vanilla line drawing.
 *
 * TODO: Coordinates checking.
 */
void pod_hline(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color) {
#ifdef POD_HAS_HAL_HLINE
    pod_hal_hline(x0, y0, width, color);
#else
    pod_line(x0, y0, x0 + width, y0, color);
#endif
}

/*
 * Draw a vertical line with given RGB565 color. If HAL supports it uses
 * hardware vline drawing. If not falls back to vanilla line drawing.
 *
 * TODO: Coordinates checking.
 */
void pod_vline(uint16_t x0, uint16_t y0, uint16_t height, uint16_t color) {
#ifdef POD_HAS_HAL_VLINE
    pod_hal_vline(x0, y0, height, color);
#else
    pod_line(x0, y0, x0, y0 + height, color);
#endif
}

/*
 * Draw a line using Bresenham's algorithm with given RGB565 color.
 *
 * TODO: Coordinates checking.
 */
/* https://github.com/jb55/bresenham-line.c/blob/master/bresenham_line.c */
void pod_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int16_t dx;
    int16_t sx;
    int16_t dy;
    int16_t sy;
    int16_t err;
    int16_t e2;

    dx = ABS(x2 - x1);
    sx = x1 < x2 ? 1 : -1;
    dy = ABS(y2 - y1);
    sy = y1 < y2 ? 1 : -1;
    err = (dx > dy ? dx : -dy) / 2;

    while (1) {
        pod_putpixel(x1, y1, color);

        if (x1 == x2 && y1 == y2) {
            break;
        };

        e2 = err;

        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dy) {
            err += dx;
            y1 += sy;
        }
    }
}

/*
 * Draw a rectangle with given RGB565 color.
 */
void pod_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
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

    uint16_t width = x1 - x0;
    uint16_t height = y1 - y0;

    pod_hline(x0, y0, width, color);
    pod_hline(x0, y1, width, color);
    pod_vline(x0, y0, height, color);
    pod_vline(x1, y0, height, color);
}

/*
 * Draw a filled rectangle with given RGB565 color.
 *
 * TODO: Coordinates checking.
 */
void pod_fillrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
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

    uint16_t width = x1 - x0;
    uint16_t height = y1 - y0;

    for (uint16_t i = 0; i < height; i++) {
        pod_hline(x0, y0 + i, width, color);
    }
}

/*
 * Write a single character. Currently supports only 8x8 fonts which must
 * be the https://github.com/dhepper/font8x8 format.
 *
 * TODO: Different fonts sizes. Transparency support.
 */

/* https://www.geeksforgeeks.org/pass-2d-array-parameter-c/ */
void pod_putchar(char ascii, int16_t x0, int16_t y0, uint16_t color, char font[128][8])
{
    bool set;
    bitmap_t bitmap = {
        .width = 8,
        .height = 8,
        .depth = 16,
    };

    bitmap_init(&bitmap);

    uint16_t *ptr = (uint16_t *) bitmap.buffer;

    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            set = font[(uint8_t)ascii][x] & 1 << y;
            if (set) {
                *(ptr++) = color;
            } else {
                *(ptr++) = 0x0000;
            }
        }
    }

    pod_blit(x0, y0, &bitmap);
    bitmap_destroy(&bitmap);
}

/*
 * Write a string of text by calling pod_putchar() repeadetly. CR and LF
 * continue from the next line. Currently supporst only 8x8 fonts which
 * must be the https://github.com/dhepper/font8x8 format.
 *
 * TODO: Different fonts sizes.
 */

void pod_puttext(char *str, int16_t x0, int16_t y0, uint16_t color, char font[128][8])
{
    char temp;

    do {
        temp = *str++;

        if (13 == temp || 10 == temp) {
            x0 = 0;
            y0 += 8;
        } else {
            pod_putchar(temp, x0, y0, color, font);
            x0 += 8;
        }
    } while (*str != 0);
}

/*
 * Blits a bitmap to a destination hardcoded in the HAL driver. Destination
 * parameter is left out intentionally to keep the API simpler. If you need
 * configurable source and destination see the file blit.c.
 *
 * TODO: Handle transparency.
 */

void pod_blit(int16_t x0, int16_t y0, bitmap_t *source) {
#ifdef POD_HAS_HAL_BLIT
    pod_hal_blit(x0, y0, source);
#else
    uint16_t color;
    uint16_t *ptr = (uint16_t *) source->buffer;

    for (uint16_t y = 0; y < source->height; y++) {
        for (uint16_t x = 0; x < source->width; x++) {
            color = *(ptr++);
            pod_hal_putpixel(x0 + x, y0 + y, color);
        }
    }
#endif
};

void pod_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source) {
#ifdef POD_HAS_HAL_SCALE_BLIT
    pod_hal_scale_blit(x0, y0, w, h, source);
#else
    /* TODO: Use pdo_putpixel() to write to framebuffer. */
#endif
};

void pod_init() {
#ifdef POD_HAS_HAL_INIT
    pod_hal_init();
#else
#endif
};

void pod_flush() {
#ifdef POD_HAS_HAL_FLUSH
    pod_hal_flush();
#else
#endif
};

