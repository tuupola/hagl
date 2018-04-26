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

#include "copepod.h"
#include "copepod_ll_fb.h"

void pod_ll_putpixel(uint16_t x0, uint16_t y0, uint16_t color)
{
    uint16_t *ptr = g_fb.buffer + g_fb.pitch * y0 + g_fb.bpp * x0;

    if((x0 < g_fb.width) && (y0 < g_fb.height)) {
    	*ptr = color;
    }
}

void pod_ll_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t *bitmap)
{
    uint16_t *dstptr = g_fb.buffer + g_fb.pitch * y0 + g_fb.bpp * x0;
    uint16_t *srcptr = bitmap;

    for (uint16_t y = 0; y < h; y++) {
        for (uint16_t x = 0; x < w; x++) {
            *(dstptr++) = *(srcptr++);
        }
        dstptr += g_fb.pitch / g_fb.bpp - w;
    }
}

void pod_ll_hline(uint16_t x1, uint16_t y1, uint16_t width, uint16_t color)
{
    uint16_t bitmap[width];

    for (uint16_t i = 0; i < width; i++) {
        ((uint16_t *)bitmap)[i] = color;
    }

    pod_ll_blit(x1, y1, width, 1, &bitmap);
}

void pod_ll_vline(uint16_t x1, uint16_t y1, uint16_t height, uint16_t color)
{
    uint16_t bitmap[height];

    for (uint16_t i = 0; i < height; i++) {
        ((uint16_t *)bitmap)[i] = color;
    }

    pod_ll_blit(x1, y1, height, 1, &bitmap);
}