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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bitmap.h"
#include "framebuffer.h"

typedef framebuffer_t bitmap_t;

uint32_t bitmap_size(bitmap_t *bitmap) {
    return bitmap->width * (bitmap->depth / 8) * bitmap->height;
};

void bitmap_init(bitmap_t *bitmap, uint8_t *buffer)
{
    bitmap->pitch = bitmap->width * (bitmap->depth / 8);  /* Bytes per row. */
    bitmap->size = bitmap->pitch * bitmap->height;  /* Size in bytes. */
    bitmap->buffer = buffer;

    memset(bitmap->buffer, 0x00, bitmap->size);
}

void bitmap_destroy(bitmap_t *bitmap)
{
    free(bitmap->buffer);
}
