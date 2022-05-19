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

#include <stdint.h>

#ifndef _BITMAP_H
#define _BITMAP_H

#define BITMAP_SIZE(width, height, depth) (width * (depth / 8) * height)

/*
Pitch is bytes per row. Depth is number of bits per pixel. Size is size
in bytes.
*/
typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t pitch;
    uint8_t depth;
    uint32_t size;
    uint8_t *buffer;
} bitmap_t;

uint32_t bitmap_size(bitmap_t *bitmap);
void bitmap_init(bitmap_t *bitmap, uint8_t *buffer);
void bitmap_blit(int16_t x0, int16_t y0, bitmap_t *src, bitmap_t *dst);
void bitmap_scale_blit(int16_t x0, int16_t y0, uint16_t w, uint16_t h, bitmap_t *src, bitmap_t *dst);

#endif /* _BITMAP_H */