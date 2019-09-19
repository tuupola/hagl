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

#include <stdint.h>

#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#define FRAMEBUFFER_SIZE(width, height, depth) (width * (depth / 8) * height)

typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t pitch;  /* Bytes per row. */
    uint8_t depth;   /* Number of bits per pixel. */
    uint32_t size;   /* Size in bytes. */
    uint8_t *buffer;
} framebuffer_t;

uint32_t framebuffer_size(framebuffer_t *fb);
void framebuffer_init(framebuffer_t *fb, uint8_t *buffer);
void framebuffer_destroy(framebuffer_t *fb);

#endif /* _FRAMEBUFFER_H */