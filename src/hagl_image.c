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

#include <stdint.h>
#include <stdio.h>

#include "hagl/image.h"
#include "hagl/surface.h"
#include "hagl.h"
#include "tjpgd.h"

typedef struct {
    FILE *fp;
    int16_t x0;
    int16_t y0;
    const hagl_surface_t *surface;
} tjpgd_iodev_t;

static uint16_t
tjpgd_data_reader(JDEC *decoder, uint8_t *buffer, uint16_t size)
{
    tjpgd_iodev_t *device = (tjpgd_iodev_t *)decoder->device;

    if (buffer) {
        /* Read bytes from input stream. */
        return (uint16_t)fread(buffer, 1, size, device->fp);
    } else {
        /* Skip bytes from input stream. */
        return fseek(device->fp, size, SEEK_CUR) ? 0 : size;
    }
}

static uint16_t
tjpgd_data_writer(JDEC *decoder, void *bitmap, JRECT *rectangle)
{
    tjpgd_iodev_t *device = (tjpgd_iodev_t *)decoder->device;
    uint8_t width = (rectangle->right - rectangle->left) + 1;
    uint8_t height = (rectangle->bottom - rectangle->top) + 1;

    hagl_bitmap_t block = {
        .width = width,
        .height = height,
        .depth = device->surface->depth,
        .pitch = width * (device->surface->depth / 8),
        .size =  width * (device->surface->depth / 8) * height,
        .buffer = (uint8_t *)bitmap
    };

    hagl_blit(device->surface, rectangle->left + device->x0, rectangle->top + device->y0, &block);

    return 1;
}

uint32_t
hagl_load_image(void const *surface, int16_t x0, int16_t y0, const char *filename)
{
    uint8_t work[3100];
    JDEC decoder;
    JRESULT result;
    tjpgd_iodev_t device;

    device.x0 = x0;
    device.y0 = y0;
    device.fp = fopen(filename, "rb");
    device.surface = surface;

    if (!device.fp) {
        return HAGL_ERR_FILE_IO;
    }
    result = jd_prepare(&decoder, tjpgd_data_reader, work, 3100, (void *)&device);
    if (result == JDR_OK) {
        result = jd_decomp(&decoder, tjpgd_data_writer, 0);
        if (JDR_OK != result) {
            fclose(device.fp);
            return HAGL_ERR_TJPGD + result;
        }
    } else {
        fclose(device.fp);
        return HAGL_ERR_TJPGD + result;
    }

    fclose(device.fp);
    return HAGL_OK;
}
