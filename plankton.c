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

#include "esp_log.h"

#include "plankton.h"
#include "font8x8.h"

#define swap(x, y) do \
    { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
        memcpy(swap_temp,&y,sizeof(x)); \
        memcpy(&y,&x,       sizeof(x)); \
        memcpy(&x,swap_temp,sizeof(x)); \
    } while (0)

// #define PLN_HAS_LL_HLINE /* TODO, these should come from config. */
// #define PLN_HAS_LL_VLINE
// #define PLN_HAS_LL_BITMAP

static const char *TAG = "plankton";

void pln_put_pixel(uint16_t x1, uint16_t y1, uint16_t color)
{
    if((x1 < 320) && (y1 < 240)) {
        pln_ll_put_pixel(x1, y1, color);
    }
}

void pln_hline(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color) {
#ifdef PLN_HAS_LL_HLINE
    pln_ll_draw_hline(x0, y0, width, color);
#else
    pln_line(x0, y0, x0 + width, y0, color);
#endif
}

void pln_vline(uint16_t x0, uint16_t y0, uint16_t height, uint16_t color) {
#ifdef PLN_HAS_LL_HLINE
    pln_ll_draw_vline(x0, y0, width, color);
#else
    pln_line(x0, y0, x0, y0 + height, color);
#endif
}

/* https://github.com/jb55/bresenham-line.c/blob/master/bresenham_line.c */
void pln_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
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
        pln_put_pixel(x1, y1, color);

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

void pln_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint16_t width = ABS(x0 - x1);
    uint16_t height = ABS(y0 - y1);

    pln_hline(x0, y0, width, color);
    pln_hline(x0, y1, width, color);
    pln_vline(x0, y0, height, color);
    pln_vline(x1, y0, height, color);
}

void pln_fillrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint16_t width = ABS(x0 - x1);
    uint16_t height = ABS(y0 - y1);

    for(uint16_t i = 0; i < height; i++) {
        pln_hline(x0, y0 + i, width, color);
    }
}

void pln_put_char(char ascii, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint8_t *font = font8x8_basic[(uint8_t)ascii];
    uint16_t bitmap[8][8];
    uint16_t *ptr = &bitmap[0][0];

    int x,y;
    int set;
    int mask;
    for (x=0; x < 8; x++) {
        for (y=0; y < 8; y++) {
            set = font[x] & 1 << y;
            if (set) {
                *(ptr++) = color;
            } else {
                *(ptr++) = 0x0000;
            }
        }
    }

    pln_ll_put_bitmap(x1, y1, 8, 8, &bitmap);
}

void pln_put_text(char *str, uint16_t x1, uint16_t y1, uint16_t color)
{
    char temp;

    do {
        temp = *str++;

        if (13 == temp || 10 == temp) {
            x1 = 0;
            y1 += 8;
        } else {
            pln_put_char(temp, x1, y1, color);
            x1 += 8;
        }
    } while (*str != 0);
}

void pln_put_bitmap(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t *bitmap) {
    pln_ll_put_bitmap(x0, y0, w, h, bitmap);
};

