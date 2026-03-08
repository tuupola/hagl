/*

MIT License

Copyright (c) 2026 Mika Tuupola

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
Helper for saving hagl_bitmap_t framebuffers as PNG files via
stb_image_write. Used by the test suite for visual debugging.

This file is part of the HAGL graphics library:
https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#ifndef SAVE_IMAGE_H
#define SAVE_IMAGE_H

#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

#include "hagl/bitmap.h"
#include "stb_image_write.h"

/*
 * Convert an RGB565 framebuffer to RGB888 and write it as a PNG file.
 * Creates the parent directory if it does not exist.
 */
static void save_image(const hagl_bitmap_t *bitmap, const char *filename) {
    uint32_t pixel_count = bitmap->width * bitmap->height;
    uint8_t rgb[pixel_count * 3];
    uint16_t *src = (uint16_t *)bitmap->buffer;

    for (uint32_t i = 0; i < pixel_count; i++) {
        uint16_t pixel = src[i];
        uint8_t r5 = (pixel >> 11) & 0x1F;
        uint8_t g6 = (pixel >> 5) & 0x3F;
        uint8_t b5 = pixel & 0x1F;

        rgb[i * 3 + 0] = (r5 * 527 + 23) >> 6;
        rgb[i * 3 + 1] = (g6 * 259 + 33) >> 6;
        rgb[i * 3 + 2] = (b5 * 527 + 23) >> 6;
    }

    /* Extract directory from filename and create it if needed. */
    char dir[256];
    const char *last_slash = NULL;
    for (const char *p = filename; *p; p++) {
        if (*p == '/') {
            last_slash = p;
        }
    }
    if (last_slash) {
        size_t len = last_slash - filename;
        if (len < sizeof(dir)) {
            memcpy(dir, filename, len);
            dir[len] = '\0';
            mkdir(dir, 0755);
        }
    }

    stbi_write_png(filename, bitmap->width, bitmap->height, 3, rgb, bitmap->width * 3);
}

#endif /* SAVE_IMAGE_H */
