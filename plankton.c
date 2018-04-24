#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "esp_log.h"

#include "plankton.h"
#include "font8x8.h"

#define swap(x,y) do \
    { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
        memcpy(swap_temp,&y,sizeof(x)); \
        memcpy(&y,&x,       sizeof(x)); \
        memcpy(&x,swap_temp,sizeof(x)); \
    } while (0)

#define PLN_HAS_LL_HLINE /* TODO, these should come from config. */
#define PLN_HAS_LL_VLINE
#define PLN_HAS_LL_BITMAP

static const char *TAG = "plankton";

void pln_put_pixel(uint16_t x1, uint16_t y1, uint16_t colour)
{
    if((x1 < 320) && (y1 < 240)) {
        pln_ll_put_pixel(x1, y1, colour);
    }
}

/* https://github.com/jb55/bresenham-line.c/blob/master/bresenham_line.c */

void pln_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
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
        pln_ll_put_pixel(x1, y1, colour);

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

void pln_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
#ifdef PLN_HAS_LL_HLINE
    pln_ll_draw_hline(x1, y1, x2, y1, colour);
    pln_ll_draw_hline(x1, y2, x2, y2, colour);
#else
    pln_draw_line(x1, y1, x2, y1, colour);
    pln_draw_line(x1, y2, x2, y2, colour);
#endif
#ifdef PLN_HAS_LL_VLINE
    pln_ll_draw_vline(x1, y2, x1, y1, colour);
    pln_ll_draw_vline(x2, y1, x2, y2, colour);
#else
    pln_draw_line(x1, y2, x1, y1, colour);
    pln_draw_line(x2, y1, x2, y2, colour);
#endif
}


void pln_fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
#ifdef PLN_HAS_LL_BITMAP
    if (x1 > x2) {
        swap(x1, x2);
    }

    if (y1 > y2) {
        swap(y1, y2);
    }

    uint16_t width = (x2 - x1);
    uint16_t height = (y2 - y1);
    uint32_t size = width * height;
    uint16_t bitmap[size];

    for (uint32_t i = 0; i <= size; i++) {
        bitmap[i] = colour;
    }

    // ESP_LOGD(TAG, "PLN...");
    // ESP_LOG_BUFFER_HEXDUMP(TAG, bitmap, size, ESP_LOG_INFO);

    pln_ll_put_bitmap(x1, y1, width, height, &bitmap);
#else
    for (uint16_t yi = y1; yi <= y2; yi++) {
#ifdef PLN_HAS_LL_HLINE
        /* Hardware support for horizontal line. */
        pln_ll_draw_hline(x1, yi, x2, yi, colour);
#else
        /* Software line drawing. */
        pln_draw_line(x1, yi, x2, yi, colour);
#endif
    }
#endif
}

void pln_put_char(char ascii, uint16_t x1, uint16_t y1, uint16_t colour)
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
                *(ptr++) = colour;
            } else {
                *(ptr++) = 0x0000;
            }
        }
    }

    pln_ll_put_bitmap(x1, y1, 8, 8, &bitmap);
}

