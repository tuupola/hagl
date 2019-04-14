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
#include <stdio.h>

#include "bitmap.h"
#include "blit.h"
#include "cohen-sutherland.h"
#include "copepod.h"
#include "copepod-hal.h"
#include "framebuffer.h"

clip_window_t clip_window = {
    .min_x = 0,
    .min_y = 0,
    .max_x = DISPLAY_WIDTH - 1,
    .max_y = DISPLAY_HEIGHT - 1,
};

void pod_clip_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    clip_window.min_x = x0;
    clip_window.min_y = y0;
    clip_window.max_x = x1;
    clip_window.max_y = y1;
}
/*
 * Puts a pixel RGB565 color. This is the only mandatory function HAL must
 * support.
 */
void pod_put_pixel(int16_t x0, int16_t y0, uint16_t color)
{
    /* x0 or y0 is before the edge, nothing to do. */
    if ((x0 < clip_window.min_x) || (y0 < clip_window.min_y))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > clip_window.max_x) || (y0 > clip_window.max_y)) {
        return;
    }

    /* If still in bounds set the pixel. */
    pod_hal_putpixel(x0, y0, color);
}

/*
 * Draw a horizontal line with given RGB565 color. If HAL supports it uses
 * hardware hline drawing. If not falls back to vanilla line drawing.
 */
void pod_draw_hline(int16_t x0, int16_t y0, uint16_t w, uint16_t color) {
#ifdef POD_HAS_HAL_HLINE
    int16_t width = w;

    /* x0 or y0 is over the edge, nothing to do. */
    if ((x0 > clip_window.max_x) || (y0 > clip_window.max_y) || (y0 < clip_window.min_y))  {
        return;
    }

    /* x0 is left of clip window, ignore start part. */
    if (x0 < clip_window.min_x) {
        width = width + x0;
        x0 = clip_window.min_x;
    }

    /* Everything outside clip window, nothing to do. */
    if (width < 0)  {
        return;
    }

    /* Cut anything going over right edge of clip window. */
    if (((x0 + width) > clip_window.max_x)) {
        width = width - (x0 + width - clip_window.max_x);
    }
    pod_hal_hline(x0, y0, width, color);
#else
    pod_draw_line(x0, y0, x0 + w, y0, color);
#endif
}

/*
 * Draw a vertical line with given RGB565 color. If HAL supports it uses
 * hardware vline drawing. If not falls back to vanilla line drawing.
 */
void pod_draw_vline(int16_t x0, int16_t y0, uint16_t h, uint16_t color) {
    int16_t height = h;
#ifdef POD_HAS_HAL_VLINE
    /* x0 or y0 is over the edge, nothing to do. */
    if ((x0 > clip_window.max_x) || (x0 < clip_window.min_x) || (y0 > clip_window.max_y))  {
        return;
    }

    /* y0 is top of clip window, ignore start part. */
    if (y0 < clip_window.min_y) {
        height = height + y0;
        y0 = clip_window.min_y;
    }

    /* Everything outside clip window, nothing to do. */
    if (height < 0)  {
        return;
    }

    /* Cut anything going over right edge. */
    if (((y0 + height) > clip_window.max_y))  {
        height = height - (y0 + height - clip_window.max_y);
    }

    pod_hal_vline(x0, y0, height, color);
#else
    pod_draw_line(x0, y0, x0, y0 + h, color);
#endif
}

/*
 * Draw a line using Bresenham's algorithm with given RGB565 color.
 */
/* https://github.com/jb55/bresenham-line.c/blob/master/bresenham_line.c */
void pod_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    /* Clip coordinates to fit clip window. */
    if (false == line_clip(&x0, &y0, &x1, &y1, clip_window)) {
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
        pod_put_pixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) {
            break;
        };

        e2 = err;

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
 * Draw a rectangle with given RGB565 color.
 */
void pod_draw_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
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

    pod_draw_hline(x0, y0, width, color);
    pod_draw_hline(x0, y1, width, color);
    pod_draw_vline(x0, y0, height, color);
    pod_draw_vline(x1, y0, height, color);
}

/*
 * Draw a filled rectangle with given RGB565 color.
 */
void pod_fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
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
        pod_draw_hline(x0, y0 + i, width, color);
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
    uint8_t buffer[128];

    bitmap_t bitmap = {
        .width = 8,
        .height = 8,
        .depth = 16,
        .pitch = 16,  /* width * (depth / 8) */
        .size = 128,   /* pitch * height. */
        .buffer = buffer
    };

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
            pod_put_pixel(x0 + x, y0 + y, color);
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

void pod_cls() {
    pod_fill_rectangle(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT -1, 0x00);
}

void pod_draw_circle(int16_t xc, int16_t yc, int16_t r, uint16_t color) {
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x) {
        pod_put_pixel(xc+x, yc+y, color);
        pod_put_pixel(xc-x, yc+y, color);
        pod_put_pixel(xc+x, yc-y, color);
        pod_put_pixel(xc-x, yc-y, color);
        pod_put_pixel(xc+y, yc+x, color);
        pod_put_pixel(xc-y, yc+x, color);
        pod_put_pixel(xc+y, yc-x, color);
        pod_put_pixel(xc-y, yc-x, color);
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        pod_put_pixel(xc+x, yc+y, color);
        pod_put_pixel(xc-x, yc+y, color);
        pod_put_pixel(xc+x, yc-y, color);
        pod_put_pixel(xc-x, yc-y, color);
        pod_put_pixel(xc+y, yc+x, color);
        pod_put_pixel(xc-y, yc+x, color);
        pod_put_pixel(xc+y, yc-x, color);
        pod_put_pixel(xc-y, yc-x, color);
    }
}

void pod_fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x) {
        pod_draw_hline(x0-x, y0+y, x * 2 , color);
        pod_draw_hline(x0-x, y0-y, x * 2, color);
        pod_draw_hline(x0-y, y0+x, y * 2, color);
        pod_draw_hline(x0-y, y0-x, y * 2, color);
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void pod_draw_polygon(int16_t amount, int16_t *vertices, uint16_t color) {

    for(int16_t i = 0; i < amount - 1; i++) {
        pod_draw_line(
            vertices[(i << 1 ) + 0],
            vertices[(i << 1 ) + 1],
            vertices[(i << 1 ) + 2],
            vertices[(i << 1 ) + 3],
            color
        );
    }
    pod_draw_line(
        vertices[0],
        vertices[1],
        vertices[(amount <<1 ) - 2],
        vertices[(amount <<1 ) - 1],
        color
    );
}

/* Adapted from  http://alienryderflex.com/polygon_fill/ */
void pod_fill_polygon(int16_t amount, int16_t *vertices, uint16_t color) {
    uint16_t nodes[64];
    int16_t y;

    float x0;
    float y0;
    float x1;
    float y1;

    /*  Loop through the rows of the image. */
    for (y = 0; y < DISPLAY_HEIGHT; y++) {

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
            pod_draw_hline(nodes[i], y, width, color);
            //pod_draw_line(nodes[i], y, nodes[i + 1], y, color);
        }
    }
}

void pod_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    int16_t vertices[6] = {x0, y0, x1, y1, x2, y2};
    pod_draw_polygon(3, vertices, color);
};

void pod_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    int16_t vertices[6] = {x0, y0, x1, y1, x2, y2};
    pod_fill_polygon(3, vertices, color);
}


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

void pod_destroy() {
#ifdef POD_HAS_HAL_DESTROY
    pod_hal_destroy();
#else
#endif
};

