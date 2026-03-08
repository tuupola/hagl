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
#include "hagl/pixel.h"
#include "hagl/vline.h"
#include "save_image.h"

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

static void teardown_callback(void *data) {
    char filename[256];
    snprintf(filename, sizeof(filename), "output/%s.png", greatest_info.name_buf);
    save_image(&bitmap, filename);
}

/*
 * Vertical line using xyh:
 */
TEST test_draw_vline_xyh(void) {
    hagl_draw_vline_xyh(&bitmap, 50, 10, 11, 0xFFFF);

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

TEST test_draw_vline_xyh_regression(void) {
    hagl_draw_vline_xyh(&bitmap, 50, 10, 11, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0x538BB775, crc);
    PASS();
}

/*
 * Single pixel vertical line:
 */
TEST test_draw_vline_xyh_single_pixel(void) {
    hagl_draw_vline_xyh(&bitmap, 50, 50, 1, 0xFFFF);

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
 * Zero height vertical line:
 */
TEST test_draw_vline_xyh_zero_height(void) {
    hagl_draw_vline_xyh(&bitmap, 50, 50, 0, 0xFFFF);

    /* Nothing should be drawn */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 50));

    /* Total: 0 pixels */
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Full display height vertical line:
 */
TEST test_draw_vline_xyh_full_height(void) {
    hagl_draw_vline_xyh(&bitmap, 160, 0, 240, 0xFFFF);

    /* On line: top and bottom edges of display */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 160, 0));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 160, 239));

    /* On line: midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 160, 120));

    /* Outside: left and right of midpoint */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 159, 120));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 161, 120));

    /* Total: 240 pixels */
    ASSERT_EQ(240, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Verify xyy produces the same result as xyh:
 *
 * xyy: hagl_draw_vline_xyy(&bitmap, 50, 10, 20, color)
 * xyh: hagl_draw_vline_xyh(&bitmap, 50, 10, 11, color)
 */
TEST test_draw_vline_xyy_match_xyh(void) {
    /* Draw with _xyy. */
    hagl_draw_vline_xyy(&bitmap, 50, 10, 20, 0xFFFF);

    uint32_t crc_xyy = crc32(bitmap.buffer, bitmap.size);

    /* Clear and draw the same area with _xyh. */
    memset(bitmap.buffer, 0, bitmap.size);
    hagl_draw_vline_xyh(&bitmap, 50, 10, 11, 0xFFFF);

    uint32_t crc_xyh = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(crc_xyh, crc_xyy);
    PASS();
}

/*
 * Verify xyy handles reversed endpoints (y0 > y1):
 *
 * xyy reversed: hagl_draw_vline_xyy(&bitmap, 50, 20, 10, color)
 * xyy normal:   hagl_draw_vline_xyy(&bitmap, 50, 10, 20, color)
 */
TEST test_draw_vline_xyy_reversed(void) {
    /* Draw with reversed endpoints. */
    hagl_draw_vline_xyy(&bitmap, 50, 20, 10, 0xFFFF);

    uint32_t crc_reversed = crc32(bitmap.buffer, bitmap.size);

    /* Clear and draw with normal order. */
    memset(bitmap.buffer, 0, bitmap.size);
    hagl_draw_vline_xyy(&bitmap, 50, 10, 20, 0xFFFF);

    uint32_t crc_normal = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(crc_normal, crc_reversed);
    PASS();
}

/*
 * Line overflowing bottom edge of display:
 * Only 10 pixels visible: (50,230) to (50,239)
 */
TEST test_draw_vline_xyh_clip_bottom(void) {
    hagl_draw_vline_xyh(&bitmap, 50, 230, 20, 0xFFFF);

    /* On line: top endpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 230));

    /* On line: bottom edge of display */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 239));

    /* Outside: above start */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 229));

    /* Total: 10 pixels */
    ASSERT_EQ(10, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_vline_xyh_clip_bottom_regression(void) {
    hagl_draw_vline_xyh(&bitmap, 50, 230, 20, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xE1BF3D53, crc);
    PASS();
}

/*
 * Line starting off top edge of display:
 * Only 15 pixels visible: (50,0) to (50,14)
 */
TEST test_draw_vline_xyh_clip_top(void) {
    hagl_draw_vline_xyh(&bitmap, 50, -5, 20, 0xFFFF);

    /* On line: top edge of display */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 0));

    /* On line: bottom endpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 14));

    /* Outside: below end */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 15));

    /* Total: 15 pixels */
    ASSERT_EQ(15, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_vline_xyh_clip_top_regression(void) {
    hagl_draw_vline_xyh(&bitmap, 50, -5, 20, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xDC3E1A44, crc);
    PASS();
}

/*
 * Lines entirely outside the display. Four sub-cases covering
 * all early-return conditions.
 */
TEST test_draw_vline_xyh_outside(void) {
    /* x left of display */
    hagl_draw_vline_xyh(&bitmap, -1, 10, 20, 0xFFFF);
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    /* x right of display */
    hagl_draw_vline_xyh(&bitmap, 320, 10, 20, 0xFFFF);
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    /* y below display */
    hagl_draw_vline_xyh(&bitmap, 50, 240, 20, 0xFFFF);
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    /* entirely above display */
    hagl_draw_vline_xyh(&bitmap, 50, -30, 20, 0xFFFF);
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Line clipped by a custom clip window on both sides:
 * Only 51 pixels visible: (75,50) to (75,100)
 */
TEST test_draw_vline_xyh_custom_clip(void) {
    hagl_set_clip(&bitmap, 50, 50, 100, 100);
    hagl_draw_vline_xyh(&bitmap, 75, 40, 80, 0xFFFF);

    /* On line: clip window edges */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 75, 50));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 75, 100));

    /* Outside: just beyond clip window */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 75, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 75, 101));

    /* Total: 51 pixels */
    ASSERT_EQ(51, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_vline_xyh_custom_clip_regression(void) {
    hagl_set_clip(&bitmap, 50, 50, 100, 100);
    hagl_draw_vline_xyh(&bitmap, 75, 40, 80, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0x5DA1FBC2, crc);
    PASS();
}

SUITE(vline_suite) {
    SET_SETUP(setup_callback, NULL);
    SET_TEARDOWN(teardown_callback, NULL);
    RUN_TEST(test_draw_vline_xyh);
    RUN_TEST(test_draw_vline_xyh_regression);
    RUN_TEST(test_draw_vline_xyh_single_pixel);
    RUN_TEST(test_draw_vline_xyh_zero_height);
    RUN_TEST(test_draw_vline_xyh_full_height);
    RUN_TEST(test_draw_vline_xyy_match_xyh);
    RUN_TEST(test_draw_vline_xyy_reversed);
    RUN_TEST(test_draw_vline_xyh_clip_bottom);
    RUN_TEST(test_draw_vline_xyh_clip_bottom_regression);
    RUN_TEST(test_draw_vline_xyh_clip_top);
    RUN_TEST(test_draw_vline_xyh_clip_top_regression);
    RUN_TEST(test_draw_vline_xyh_outside);
    RUN_TEST(test_draw_vline_xyh_custom_clip);
    RUN_TEST(test_draw_vline_xyh_custom_clip_regression);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(vline_suite);
    GREATEST_MAIN_END();
}
