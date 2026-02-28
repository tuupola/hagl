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

#include <string.h>

#include "crc32.h"
#include "greatest.h"
#include "hagl/bitmap.h"
#include "hagl/clip.h"
#include "hagl/hline.h"
#include "hagl/pixel.h"

#define TEST_WIDTH 320
#define TEST_HEIGHT 240
#define TEST_DEPTH 16

static hagl_bitmap_t bitmap;
static uint8_t buffer[TEST_WIDTH * TEST_HEIGHT * (TEST_DEPTH / 8)];

static uint32_t count_pixels(hagl_bitmap_t *bitmap, hagl_color_t color) {
    uint32_t count = 0;
    for (int16_t y = 0; y < bitmap->height; y++) {
        for (int16_t x = 0; x < bitmap->width; x++) {
            if (hagl_get_pixel(bitmap, x, y) == color) {
                count++;
            }
        }
    }
    return count;
}

static void setup_callback(void *data) {
    memset(buffer, 0, sizeof(buffer));
    hagl_bitmap_init(&bitmap, TEST_WIDTH, TEST_HEIGHT, TEST_DEPTH, buffer);
}

/*
 * Horizontal line using xyw:
 *
 * (10,50)-----(20,50)
 */
TEST test_draw_hline_xyw(void) {
    hagl_draw_hline_xyw(&bitmap, 10, 50, 11, 0xFFFF);

    /* On line: endpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 50));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 20, 50));

    /* On line: midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 15, 50));

    /* Outside: above and below midpoint */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 15, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 15, 51));

    /* Outside: left and right of endpoints */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 9, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 21, 50));

    /* Total: 11 pixels */
    ASSERT_EQ(11, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_hline_xyw_regression(void) {
    hagl_draw_hline_xyw(&bitmap, 10, 50, 11, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xDF4E4483, crc);
    PASS();
}

/*
 * Single pixel horizontal line:
 *
 * (50,50)
 */
TEST test_draw_hline_xyw_single_pixel(void) {
    hagl_draw_hline_xyw(&bitmap, 50, 50, 1, 0xFFFF);

    /* On line: the pixel itself */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 50));

    /* Outside: all four neighbors */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 51, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 51));

    /* Total: 1 pixel */
    ASSERT_EQ(1, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

SUITE(hline_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_draw_hline_xyw);
    RUN_TEST(test_draw_hline_xyw_regression);
    RUN_TEST(test_draw_hline_xyw_single_pixel);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(hline_suite);
    GREATEST_MAIN_END();
}
