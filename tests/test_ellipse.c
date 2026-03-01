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
#include "hagl/circle.h"
#include "hagl/clip.h"
#include "hagl/ellipse.h"
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
 * Ellipse outline with center (100,100), a=20 (horizontal), b=10 (vertical):
 *
 *            (100,90)
 *           /        \
 *          /          \
 * (80,100)  (100,100)  (120,100)
 *          \          /
 *           \        /
 *            (100,110)
 */
TEST test_draw_ellipse(void) {
    hagl_draw_ellipse(&bitmap, 100, 100, 20, 10, 0xFFFF);

    /* On outline: cardinal points */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 100, 90));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 100, 110));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 80, 100));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 120, 100));

    /* Inside: center is empty (outline only) */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 100, 100));

    /* Outside: 1 pixel beyond each cardinal point */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 100, 89));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 100, 111));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 79, 100));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 121, 100));

    /* Total: 88 pixels */
    ASSERT_EQ(88, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_ellipse_regression(void) {
    hagl_draw_ellipse(&bitmap, 100, 100, 20, 10, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xF096F4F2, crc);
    PASS();
}

SUITE(ellipse_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_draw_ellipse);
    RUN_TEST(test_draw_ellipse_regression);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(ellipse_suite);
    GREATEST_MAIN_END();
}
