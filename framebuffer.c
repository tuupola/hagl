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

/*

https://jsandler18.github.io/extra/framebuffer.html

A Framebuffer is a piece of memory that is shared between the CPU and the GPU.
The CPU writes RGB pixels to the buffer, and the GPU renders it to whatever
output device you have connected.

The Depth of a framebuffer is the number of bits in every pixel. For this
tutorial, we will be exclusively using a depth of 24, meaning that each of the
red, green and blue values will have 8 bits, or 1 byte to them.

The Pitch of a framebuffer is simply the number of bytes that are in each row
on screen.

We can calculate the Pixels Per Row by pitch / (depth / 8), or equivalently
(pitch * 8) / depth.

We can calculate the offset within the framebuffer of a pixel located at
Coordinates (x,y) by pitch * y + (depth / 8) * x

*/

#include <stdlib.h>
#include <stdint.h>

#include "framebuffer.h"

static const char *TAG = "framebuffer";

void framebuffer_init(framebuffer_t *fb)
{
    fb->bpp = fb->depth / 8; /* Bytes per pixel. */
    fb->pitch = fb->width * fb->bpp; /* Bytes per row. */
    fb->size = fb->pitch * fb->height; /* Size in bytes. */

    uint16_t *buffer = malloc(fb->size);
    //uint16_t *buffer = heap_caps_malloc(fb->size, MALLOC_CAP_DMA);
    fb->buffer = buffer;
}

void framebuffer_destroy(framebuffer_t *fb)
{
    free(fb->buffer);
}
