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

#include "greatest.h"
#include "crc32.h"
#include "hagl/bitmap.h"
#include "hagl/pixel.h"
#include "hagl/clip.h"
#include "hagl/line.h"

#define TEST_WIDTH  320
#define TEST_HEIGHT 240
#define TEST_DEPTH  16

static hagl_bitmap_t bitmap;
static uint8_t buffer[TEST_WIDTH * TEST_HEIGHT * (TEST_DEPTH / 8)];

static uint32_t
count_pixels(hagl_bitmap_t *bitmap, hagl_color_t color) {
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

static void
setup_callback(void *data) {
    memset(buffer, 0, sizeof(buffer));
    hagl_bitmap_init(&bitmap, TEST_WIDTH, TEST_HEIGHT, TEST_DEPTH, buffer);
}

/*
 * Horizontal line:
 *
 * (10,50)-----(20,50)
 */
TEST
test_draw_line_horizontal(void) {
    hagl_draw_line(&bitmap, 10, 50, 20, 50, 0xFFFF);

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

TEST
test_draw_line_horizontal_regression(void) {
    hagl_draw_line(&bitmap, 10, 50, 20, 50, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xDF4E4483, crc);
    PASS();
}

/*
 * Vertical line:
 *
 * (50,10)
 *   |
 *   |
 * (50,20)
 */
TEST
test_draw_line_vertical(void) {
    hagl_draw_line(&bitmap, 50, 10, 50, 20, 0xFFFF);

    /* On line: endpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 20));

    /* On line: midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 15));

    /* Outside: left and right of midpoint */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 51, 15));

    /* Outside: above and below endpoints */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 21));

    /* Total: 11 pixels */
    ASSERT_EQ(11, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST
test_draw_line_vertical_regression(void) {
    hagl_draw_line(&bitmap, 50, 10, 50, 20, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0x538BB775, crc);
    PASS();
}

/*
 * Diagonal line (45 degrees):
 *
 * (10,10)
 *   \
 *    \
 *     (20,20)
 *
 * Bresenham produces a staircase pattern with 21 pixels:
 * (10,10) (10,11) (11,11) (11,12) ... (19,20) (20,20)
 */
TEST
test_draw_line_diagonal(void) {
    hagl_draw_line(&bitmap, 10, 10, 20, 20, 0xFFFF);

    /* On line: endpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 20, 20));

    /* On line: midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 15, 15));

    /* On line: staircase neighbors of midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 14, 15));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 15, 16));

    /* Outside: off the staircase */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 16, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 15, 14));

    /* Outside: beyond endpoints */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 9, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 21, 21));

    /* Total: 21 pixels (staircase pattern) */
    ASSERT_EQ(21, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST
test_draw_line_diagonal_regression(void) {
    hagl_draw_line(&bitmap, 10, 10, 20, 20, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xAD9B84B1, crc);
    PASS();
}

/*
 * Single pixel line:
 *
 * (50,50)
 */
TEST
test_draw_line_single_pixel(void) {
    hagl_draw_line(&bitmap, 50, 50, 50, 50, 0xFFFF);

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

/*
 * Line clipped by top-left corner of display:
 */
TEST
test_draw_line_clip_top_left(void) {
    hagl_draw_line(&bitmap, -10, -10, 10, 10, 0xFFFF);

    /* On line: clipped start at origin */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 0, 0));

    /* On line: visible endpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 10));

    /* On line: midpoint of visible portion */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 5, 5));

    /* Outside: beyond visible endpoint */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 11, 11));

    /* Outside: off the staircase */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 1, 0));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 0, 2));

    /* Total: 21 pixels (staircase from origin to (10,10)) */
    ASSERT_EQ(21, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST
test_draw_line_clip_top_left_regression(void) {
    hagl_draw_line(&bitmap, -10, -10, 10, 10, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xC93AD1B1, crc);
    PASS();
}

/*
 * Line entirely outside the display:
 *
 * (-30,-30)
 *    \
 *     \
 *      (-10,-10)
 *                  .(0,0) display starts here
 */
TEST
test_draw_line_clip_outside(void) {
    hagl_draw_line(&bitmap, -30, -30, -10, -10, 0xFFFF);

    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));
    PASS();
}

/*
 * Line clipped by a custom clip window:
 *
 * (40,40)
 *    \
 *     (50,50)-------(100,50)
 *       |               |
 *       |               |
 *     (50,100)------(100,100)
 *                         \
 *                          (110,110)
 */
TEST
test_draw_line_custom_clip(void) {
    hagl_set_clip(&bitmap, 50, 50, 100, 100);
    hagl_draw_line(&bitmap, 40, 40, 110, 110, 0xFFFF);

    /* On line: clip window corners */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 50));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 100, 100));

    /* On line: midpoint of visible portion */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 75, 75));

    /* Outside: before clip window */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 40, 40));

    /* Outside: after clip window */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 101, 101));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 110, 110));

    /* Total: 101 pixels (staircase from (50,50) to (100,100)) */
    ASSERT_EQ(101, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST
test_draw_line_custom_clip_regression(void) {
    hagl_set_clip(&bitmap, 50, 50, 100, 100);
    hagl_draw_line(&bitmap, 40, 40, 110, 110, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0x5BC8B87A, crc);
    PASS();
}

SUITE(line_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_draw_line_horizontal);
    RUN_TEST(test_draw_line_horizontal_regression);
    RUN_TEST(test_draw_line_vertical);
    RUN_TEST(test_draw_line_vertical_regression);
    RUN_TEST(test_draw_line_diagonal);
    RUN_TEST(test_draw_line_diagonal_regression);
    RUN_TEST(test_draw_line_single_pixel);
    RUN_TEST(test_draw_line_clip_top_left);
    RUN_TEST(test_draw_line_clip_top_left_regression);
    RUN_TEST(test_draw_line_clip_outside);
    RUN_TEST(test_draw_line_custom_clip);
    RUN_TEST(test_draw_line_custom_clip_regression);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(line_suite);
    GREATEST_MAIN_END();
}
