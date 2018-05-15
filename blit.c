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
#include <math.h>

#include <esp_log.h>

#include "blit.h"
#include "bitmap.h"

/*
 * Blit source bitmap to target bitmap. Note that bitmap is just and
 * alias to framebuffer so you can use which ever suits best.
 */

void blit(int16_t x0, int16_t y0, bitmap_t *src, bitmap_t *dst)
{
    uint16_t srcw = src->width;
    uint16_t srch = src->height;
    int16_t x1 = 0;
    int16_t y1 = 0;

    /* x0 or y0 is over the edge, nothing to do. */
    if ((x0 > dst->width) || (y0 > dst->height))  {
        return;
    }

    /* x0 is negative, ignore parts outside of screen. */
    if (x0 < 0) {
        srcw = srcw + x0;
        x1 = abs(x0);
        x0 = 0;
    }

    /* y0 is negative, ignore parts outside of screen. */
    if (y0 < 0) {
        srch = srch + y0;
        y1 = abs(y0);
        y0 = 0;
    }

    /* Ignore everything going over right edge. */
    if (srcw > dst->width - x0) {
        srcw = dst->width - x0;
    }

    /* Ignore everything going over bottom edge. */
    if (srch > dst->height - y0) {
        srch = dst->height - y0;
    }

    uint16_t *dstptr = (uint16_t *) (dst->buffer + (dst->pitch * y0) + (dst->depth / 8) * x0);
    uint16_t *srcptr = (uint16_t *) src->buffer;
    //uint16_t *srcptr = (uint16_t *) src->buffer + y1 * src->pitch + x1;;

    for (uint16_t y = 0; y < srch; y++) {
        for (uint16_t x = 0; x < srcw; x++) {
             *(dstptr++) = *(srcptr++);
        }
        dstptr += dst->pitch / (dst->depth / 8) - srcw;
        //srcptr += src->pitch / (src->depth / 8) - srcw;
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

void scale_blit(int16_t x0, int16_t y0, uint16_t dstw, uint16_t dsth, bitmap_t *src, bitmap_t *dst)
{
    int16_t x1 = 0;
    int16_t y1 = 0;

    uint16_t px, py;

    uint16_t srcw = src->width;
    uint16_t srch = src->height;
    uint32_t x_ratio = (uint32_t)((srcw << 16) / dstw);
    uint32_t y_ratio = (uint32_t)((srch << 16) / dsth);

    /* x0 or y0 is over the edge, nothing to do. */
    if ((x0 > dst->width) || (y0 > dst->height)) {
        return;
    }

    /* x0 is negative, ignore parts outside of screen. */
    if (x0 < 0) {
        dstw = dstw + x0;
        x1 = abs(x0);
        x0 = 0;
    }

    /* y0 is negative, ignore parts outside of screen. */
    if (y0 < 0) {
        dsth = dsth + y0;
        y1 = abs(y0);
        y0 = 0;
    }

        /* Ignore everything going over right edge. */
    if (dstw > (dst->width - x0)) {
        dstw = dst->width - x0;
    }

    /* Ignore everything going over bottom edge. */
    if (dsth > (dst->height - y0)) {
        dsth = dst->height - y0;
    }

    uint16_t *dstptr = (uint16_t *) (dst->buffer + dst->pitch * y0 + (dst->depth / 8) * x0);
    uint16_t *srcptr = (uint16_t *) src->buffer;
    //uint16_t *srcptr = (uint16_t *) src->buffer + y1 * src->pitch + x1;;

    for (uint16_t y = 0; y < dsth; y++) {
        for (uint16_t x = 0; x < dstw; x++) {
            px = ((x * x_ratio) >> 16);
            py = ((y * y_ratio) >> 16);
            *(dstptr++) = srcptr[(uint16_t)((py * srcw) + px)];
        }
        dstptr += dst->pitch / (dst->depth / 8) - dstw;
    }
}
