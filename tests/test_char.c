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

This file is part of the HAGL graphics library:
https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#include <stdint.h>
#include <string.h>

#include "greatest.h"

#include "hagl/bitmap.h"
#include "hagl/char.h"

#include "font5x7.h"
#include "font5x8.h"
#include "font6x9.h"

#define TEST_WIDTH 320
#define TEST_HEIGHT 240
#define TEST_DEPTH 16

#define GLYPH_BUFFER_SIZE (6 * 9 * (TEST_DEPTH / 8))

static hagl_bitmap_t surface;
static uint8_t surface_buffer[TEST_WIDTH * TEST_HEIGHT * (TEST_DEPTH / 8)];

static hagl_bitmap_t bitmap;
static uint8_t glyph_buffer[GLYPH_BUFFER_SIZE];

static void setup_callback(void *data) {
    memset(surface_buffer, 0, sizeof(surface_buffer));
    hagl_bitmap_init(&surface, TEST_WIDTH, TEST_HEIGHT, TEST_DEPTH, surface_buffer);

    memset(glyph_buffer, 0, sizeof(glyph_buffer));
    memset(&bitmap, 0, sizeof(bitmap));
    bitmap.buffer = glyph_buffer;
}

TEST test_get_glyph_bitmap_dimensions(void) {
    uint8_t status;

    status = hagl_get_glyph(&surface, 0x41, 0xFFFF, &bitmap, font6x9);
    ASSERT_EQ(0, status);
    ASSERT_EQ(6, bitmap.width);
    ASSERT_EQ(9, bitmap.height);
    ASSERT_EQ(16, bitmap.depth);
    ASSERT_EQ(12, bitmap.pitch);
    ASSERT_EQ(108, bitmap.size);

    status = hagl_get_glyph(&surface, 0x41, 0xFFFF, &bitmap, font5x7);
    ASSERT_EQ(0, status);
    ASSERT_EQ(5, bitmap.width);
    ASSERT_EQ(7, bitmap.height);
    ASSERT_EQ(16, bitmap.depth);
    ASSERT_EQ(10, bitmap.pitch);
    ASSERT_EQ(70, bitmap.size);

    status = hagl_get_glyph(&surface, 0x41, 0xFFFF, &bitmap, font5x8);
    ASSERT_EQ(0, status);
    ASSERT_EQ(5, bitmap.width);
    ASSERT_EQ(8, bitmap.height);
    ASSERT_EQ(16, bitmap.depth);
    ASSERT_EQ(10, bitmap.pitch);
    ASSERT_EQ(80, bitmap.size);

    PASS();
}

/*
 * Verify rasterized 'A' (0x41) from font5x7 with color 0xF800.
 *
 * 0x60  .xx..
 * 0x90  x..x.
 * 0x90  x..x.
 * 0xf0  xxxx.
 * 0x90  x..x.
 * 0x90  x..x.
 * 0x00  .....
 */
TEST test_get_glyph_content(void) {
    uint8_t status;
    hagl_color_t *pixels;

    status = hagl_get_glyph(&surface, 0x41, 0xF800, &bitmap, font5x7);
    ASSERT_EQ(0, status);

    pixels = (hagl_color_t *)bitmap.buffer;

    /* 0x60  .xx.. */
    ASSERT_EQ(0x0000, pixels[0 * 5 + 0]);
    ASSERT_EQ(0xF800, pixels[0 * 5 + 1]);
    ASSERT_EQ(0xF800, pixels[0 * 5 + 2]);
    ASSERT_EQ(0x0000, pixels[0 * 5 + 3]);
    ASSERT_EQ(0x0000, pixels[0 * 5 + 4]);

    /* 0x90  x..x. */
    ASSERT_EQ(0xF800, pixels[1 * 5 + 0]);
    ASSERT_EQ(0x0000, pixels[1 * 5 + 1]);
    ASSERT_EQ(0x0000, pixels[1 * 5 + 2]);
    ASSERT_EQ(0xF800, pixels[1 * 5 + 3]);
    ASSERT_EQ(0x0000, pixels[1 * 5 + 4]);

    /* 0x90  x..x. */
    ASSERT_EQ(0xF800, pixels[2 * 5 + 0]);
    ASSERT_EQ(0x0000, pixels[2 * 5 + 1]);
    ASSERT_EQ(0x0000, pixels[2 * 5 + 2]);
    ASSERT_EQ(0xF800, pixels[2 * 5 + 3]);
    ASSERT_EQ(0x0000, pixels[2 * 5 + 4]);

    /* 0xf0  xxxx. */
    ASSERT_EQ(0xF800, pixels[3 * 5 + 0]);
    ASSERT_EQ(0xF800, pixels[3 * 5 + 1]);
    ASSERT_EQ(0xF800, pixels[3 * 5 + 2]);
    ASSERT_EQ(0xF800, pixels[3 * 5 + 3]);
    ASSERT_EQ(0x0000, pixels[3 * 5 + 4]);

    /* 0x90  x..x. */
    ASSERT_EQ(0xF800, pixels[4 * 5 + 0]);
    ASSERT_EQ(0x0000, pixels[4 * 5 + 1]);
    ASSERT_EQ(0x0000, pixels[4 * 5 + 2]);
    ASSERT_EQ(0xF800, pixels[4 * 5 + 3]);
    ASSERT_EQ(0x0000, pixels[4 * 5 + 4]);

    /* 0x90  x..x. */
    ASSERT_EQ(0xF800, pixels[5 * 5 + 0]);
    ASSERT_EQ(0x0000, pixels[5 * 5 + 1]);
    ASSERT_EQ(0x0000, pixels[5 * 5 + 2]);
    ASSERT_EQ(0xF800, pixels[5 * 5 + 3]);
    ASSERT_EQ(0x0000, pixels[5 * 5 + 4]);

    /* 0x00  ..... */
    ASSERT_EQ(0x0000, pixels[6 * 5 + 0]);
    ASSERT_EQ(0x0000, pixels[6 * 5 + 1]);
    ASSERT_EQ(0x0000, pixels[6 * 5 + 2]);
    ASSERT_EQ(0x0000, pixels[6 * 5 + 3]);
    ASSERT_EQ(0x0000, pixels[6 * 5 + 4]);

    PASS();
}

SUITE(char_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_get_glyph_bitmap_dimensions);
    RUN_TEST(test_get_glyph_content);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(char_suite);
    GREATEST_MAIN_END();
}
