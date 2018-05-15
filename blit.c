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

#include <esp_log.h>

#include "blit.h"
#include "bitmap.h"

/*
 * Blit source bitmap to target bitmap. Note that bitmap is just and
 * alias to framebuffer so you can use which ever suits best.
 */

void blit(uint16_t x0, uint16_t y0, bitmap_t *src, bitmap_t *dst)
{
    uint16_t w = src->width;
    uint16_t h = src->height;
    uint16_t *dstptr = dst->buffer + (dst->pitch * y0) + (dst->depth / 8) * x0;
    uint16_t *srcptr = src->buffer;

    for (uint16_t y = 0; y < h; y++) {
        for (uint16_t x = 0; x < w; x++) {
             *(dstptr++) = *(srcptr++);
        }
        dstptr += dst->pitch / (dst->depth / 8) - w;
        //srcptr += src->pitch / (src->depth / 8) - w;
    }
}

/*
 * Blit source bitmap to target bitmap scaling it up or down to given
 * dimensions. Note that bitmap is just and alias to framebuffer so you
 * can use which ever suits best.
 *
 * http://www.tech-algorithm.com/articles/nearest-neighbor-image-scaling
 * http://www.davdata.nl/math/bmresize.html
 */

void scale_blit(uint16_t x0, uint16_t y0, uint16_t dstw, uint16_t dsth, bitmap_t *src, bitmap_t *dst)
{
    uint16_t px, py;
    uint16_t srcw = src->width;
    uint16_t srch = src->height;

    uint32_t x_ratio = (uint32_t)((srcw << 16) / dstw);
    uint32_t y_ratio = (uint32_t)((srch << 16) / dsth);

    uint16_t *dstptr = dst->buffer + dst->pitch * y0 + (dst->depth / 8) * x0;
    uint16_t *srcptr = src->buffer;

    for (uint16_t y = 0; y < dsth; y++) {
        for (uint16_t x = 0; x < dstw; x++) {
            px = ((x * x_ratio) >> 16);
            py = ((y * y_ratio) >> 16);
            *(dstptr++) = srcptr[(uint16_t)((py * srcw) + px)];
        }
        dstptr += dst->pitch / (dst->depth / 8) - dstw;
    }
}
