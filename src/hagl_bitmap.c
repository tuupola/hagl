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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "hagl/bitmap.h"

#include <stdio.h>
#include "hagl_hal.h"

static void
put_pixel(void *_bitmap, int16_t x0, int16_t y0, hagl_color_t color)
{
    hagl_bitmap_t *bitmap = _bitmap;

    hagl_color_t *ptr = (hagl_color_t *) (bitmap->buffer + bitmap->pitch * y0 + (bitmap->depth / 8) * x0);
    *ptr = color;
}

static hagl_color_t
get_pixel(void *_bitmap, int16_t x0, int16_t y0)
{
    hagl_bitmap_t *bitmap = _bitmap;
    return *(hagl_color_t *) (bitmap->buffer + bitmap->pitch * y0 + (bitmap->depth / 8) * x0);
}

void
hline(void *_bitmap, int16_t x0, int16_t y0, uint16_t width, hagl_color_t color)
{
    hagl_bitmap_t *bitmap = _bitmap;

    hagl_color_t *ptr = (hagl_color_t *) (bitmap->buffer + bitmap->pitch * y0 + (bitmap->depth / 8) * x0);
    for (uint16_t x = 0; x < width; x++) {
        *ptr++ = color;
    }
}

void
vline(void *_bitmap, int16_t x0, int16_t y0, uint16_t height, hagl_color_t color)
{
    hagl_bitmap_t *bitmap = _bitmap;

    hagl_color_t *ptr = (hagl_color_t *) (bitmap->buffer + bitmap->pitch * y0 + (bitmap->depth / 8) * x0);
    for (uint16_t y = 0; y < height; y++) {
        *ptr = color;
        ptr += bitmap->pitch / (bitmap->depth / 8);
    }
}

/*
 * Blit source bitmap to a destination bitmap->
 */

static void
blit(void *_dst, int16_t x0, int16_t y0, void *_src)
{
    hagl_bitmap_t *dst = _dst;
    hagl_bitmap_t *src = _src;

    int16_t srcw = src->width;
    int16_t srch = src->height;
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

    /* Everthing outside viewport, nothing to do. */
    if ((srcw < 0 ) || (srch < 0))  {
        return;
    }

    uint8_t *dstptr = (uint8_t *) (dst->buffer + (dst->pitch * y0) + ((dst->depth / 8) * x0));
    uint8_t *srcptr = (uint8_t *) (src->buffer + (src->pitch * y1) + ((dst->depth / 8) * x1));

    /* Bytes per pixel. */
    uint8_t bytes = dst->depth / 8;
    for (uint16_t y = 0; y < srch; y++) {
        for (uint16_t x = 0; x < srcw; x++) {
            for (uint16_t z = 0; z < bytes; z++) {
                *(dstptr++) = *(srcptr++);
            }
        }
        dstptr += (dst->pitch / (dst->depth / 8) - srcw) * bytes;
    }
}

/*
 * Blit source bitmap to target bitmap scaling it up or down to given
 * dimensions.
 *
 * http://www.tech-algorithm.com/articles/nearest-neighbor-image-scaling
 * http://www.davdata.nl/math/bmresize.html
 */

static void
scale_blit(void *_dst, int16_t x0, int16_t y0, uint16_t dstw, uint16_t dsth, void *_src)
{
    hagl_bitmap_t *dst = _dst;
    hagl_bitmap_t *src = _src;

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
        x0 = 0;
    }

    /* y0 is negative, ignore parts outside of screen. */
    if (y0 < 0) {
        dsth = dsth + y0;
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

    /* Bytes per pixel. */
    uint8_t bytes = dst->depth / 8;

    /* If sentence here is not the most elegant thing, but makes */
    /* the pointer maths much more easy to read. */
    if (2 == bytes) {
        uint16_t *dstptr = (uint16_t *) (dst->buffer + dst->pitch * y0 + (dst->depth / 8) * x0);
        uint16_t *srcptr = (uint16_t *) src->buffer;
        for (uint16_t y = 0; y < dsth; y++) {
            for (uint16_t x = 0; x < dstw; x++) {
                px = ((x * x_ratio) >> 16);
                py = ((y * y_ratio) >> 16);
                *(dstptr++) = srcptr[(uint8_t)((py * srcw) + px)];
            }
            dstptr += dst->pitch / (dst->depth / 8) - dstw;
        }
    } else {
        /* Assume 1 byte per pixel. */
        uint8_t *dstptr = (uint8_t *) (dst->buffer + dst->pitch * y0 + (dst->depth / 8) * x0);
        uint8_t *srcptr = (uint8_t *) src->buffer;
        for (uint16_t y = 0; y < dsth; y++) {
            for (uint16_t x = 0; x < dstw; x++) {
                px = ((x * x_ratio) >> 16);
                py = ((y * y_ratio) >> 16);
                *(dstptr++) = srcptr[(uint8_t)((py * srcw) + px)];
            }
            dstptr += dst->pitch / (dst->depth / 8) - dstw;
        }
    }
}

/* Initialise bitmap with given buffer. */
void
hagl_bitmap_init(hagl_bitmap_t *bitmap, int16_t width, uint16_t height, uint8_t depth, void *buffer)
{
    bitmap->width = width;
    bitmap->height = height;
    bitmap->depth = depth;
    bitmap->buffer = (uint8_t *) buffer;

    bitmap->pitch = bitmap->width * (bitmap->depth / 8);
    bitmap->size = bitmap->pitch * bitmap->height;

    bitmap->clip.x0 = 0;
    bitmap->clip.y0 = 0;
    bitmap->clip.x1 = bitmap->width - 1;
    bitmap->clip.y1 = bitmap->height - 1;

    bitmap->put_pixel = put_pixel;
    bitmap->get_pixel = get_pixel;
    bitmap->hline = hline;
    bitmap->vline = vline;
    bitmap->blit = blit;
    bitmap->scale_blit = scale_blit;
}