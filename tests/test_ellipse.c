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

/*
 * Ellipse with a=0, b=0:
 */
TEST test_draw_ellipse_a0_b0(void) {
    hagl_draw_ellipse(&bitmap, 50, 50, 0, 0, 0xFFFF);

    /* On outline: the center pixel */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 50));

    /* Outside: immediate neighbors are empty */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 51, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 51));

    /* Total: 1 pixel */
    ASSERT_EQ(1, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Ellipse with a=1, b=1 (same as circle r=1):
 * Produces a cross with 4 pixels. Center is empty.
 */
TEST test_draw_ellipse_a1_b1(void) {
    hagl_draw_ellipse(&bitmap, 100, 100, 1, 1, 0xFFFF);

    /* On outline: cardinal neighbors */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 100, 99));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 100, 101));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 99, 100));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 101, 100));

    /* Inside: center is empty */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 100, 100));

    /* Outside: diagonal neighbors */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 99, 99));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 101, 99));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 99, 101));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 101, 101));

    /* Total: 4 pixels */
    ASSERT_EQ(4, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Ellipse with equal radii must match circle:
 * hagl_draw_ellipse(100, 100, 10, 10) == hagl_draw_circle(100, 100, 10)
 */
TEST test_draw_ellipse_matches_circle(void) {
    /* Draw with ellipse. */
    hagl_draw_ellipse(&bitmap, 100, 100, 10, 10, 0xFFFF);

    uint32_t crc_ellipse = crc32(bitmap.buffer, bitmap.size);

    /* Clear and draw the same shape with circle. */
    memset(bitmap.buffer, 0, bitmap.size);
    hagl_draw_circle(&bitmap, 100, 100, 10, 0xFFFF);

    uint32_t crc_circle = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(crc_circle, crc_ellipse);
    PASS();
}

/*
 * Ellipse clipped by top-left corner of display:
 * Center at (5,5), a=20 (horizontal), b=10 (vertical).
 * Top and left arcs are clipped.
 */
TEST test_draw_ellipse_clip_top_left(void) {
    hagl_draw_ellipse(&bitmap, 5, 5, 20, 10, 0xFFFF);

    /* On outline: visible cardinal points */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 5, 15));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 25, 5));

    /* Clipped: top and left cardinals are off-screen */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 5, 0));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 0, 5));

    /* Inside: center is empty */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 5, 5));

    /* Total: 33 visible pixels */
    ASSERT_EQ(33, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_ellipse_clip_top_left_regression(void) {
    hagl_draw_ellipse(&bitmap, 5, 5, 20, 10, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0x3A1D3CAE, crc);
    PASS();
}

/*
 * Ellipse entirely outside the display:
 * Center at (-50,-50), a=10, b=10. No pixels visible.
 */
TEST test_draw_ellipse_clip_outside(void) {
    hagl_draw_ellipse(&bitmap, -50, -50, 10, 10, 0xFFFF);

    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));
    PASS();
}

/*
 * Ellipse clipped by a custom clip window:
 * Center (100,85), a=60, b=40, clip (50,50)-(150,120).
 * All four cardinal points are outside the clip window.
 */
TEST test_draw_ellipse_custom_clip(void) {
    hagl_set_clip(&bitmap, 50, 50, 150, 120);
    hagl_draw_ellipse(&bitmap, 100, 85, 60, 40, 0xFFFF);

    /* On outline: visible pixels at clip edges */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 63));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 150, 63));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 70, 50));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 130, 120));

    /* Inside: center is empty */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 100, 85));

    /* Outside: just beyond clip window */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 85));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 151, 85));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 100, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 100, 121));

    /* Total: 92 visible pixels */
    ASSERT_EQ(92, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_ellipse_custom_clip_regression(void) {
    hagl_set_clip(&bitmap, 50, 50, 150, 120);
    hagl_draw_ellipse(&bitmap, 100, 85, 60, 40, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xB7B6BD23, crc);
    PASS();
}

SUITE(ellipse_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_draw_ellipse);
    RUN_TEST(test_draw_ellipse_regression);
    RUN_TEST(test_draw_ellipse_a0_b0);
    RUN_TEST(test_draw_ellipse_a1_b1);
    RUN_TEST(test_draw_ellipse_matches_circle);
    RUN_TEST(test_draw_ellipse_clip_top_left);
    RUN_TEST(test_draw_ellipse_clip_top_left_regression);
    RUN_TEST(test_draw_ellipse_clip_outside);
    RUN_TEST(test_draw_ellipse_custom_clip);
    RUN_TEST(test_draw_ellipse_custom_clip_regression);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(ellipse_suite);
    GREATEST_MAIN_END();
}
