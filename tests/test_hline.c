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

/*
 * Zero width horizontal line:
 */
TEST test_draw_hline_xyw_zero_width(void) {
    hagl_draw_hline_xyw(&bitmap, 50, 50, 0, 0xFFFF);

    /* Nothing should be drawn */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 50));

    /* Total: 0 pixels */
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Full display width horizontal line:
 *
 * (0,100)-----------------------------(319,100)
 */
TEST test_draw_hline_xyw_full_width(void) {
    hagl_draw_hline_xyw(&bitmap, 0, 100, 320, 0xFFFF);

    /* On line: left and right edges of display */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 0, 100));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 319, 100));

    /* On line: midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 160, 100));

    /* Outside: above and below midpoint */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 160, 99));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 160, 101));

    /* Total: 320 pixels */
    ASSERT_EQ(320, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Verify xyx produces the same result as xyw:
 *
 * xyx: hagl_draw_hline_xyx(&bitmap, 10, 50, 20, color)
 * xyw: hagl_draw_hline_xyw(&bitmap, 10, 50, 11, color)
 */
TEST test_draw_hline_xyx_match_xyw(void) {
    /* Draw with _xyx. */
    hagl_draw_hline_xyx(&bitmap, 10, 50, 20, 0xFFFF);

    uint32_t crc_xyx = crc32(bitmap.buffer, bitmap.size);

    /* Clear and draw the same area with _xyw. */
    memset(bitmap.buffer, 0, bitmap.size);
    hagl_draw_hline_xyw(&bitmap, 10, 50, 11, 0xFFFF);

    uint32_t crc_xyw = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(crc_xyw, crc_xyx);
    PASS();
}

/*
 * Verify xyx handles reversed endpoints (x0 > x1):
 *
 * xyx reversed: hagl_draw_hline_xyx(&bitmap, 20, 50, 10, color)
 * xyx normal:   hagl_draw_hline_xyx(&bitmap, 10, 50, 20, color)
 */
TEST test_draw_hline_xyx_reversed(void) {
    /* Draw with reversed endpoints. */
    hagl_draw_hline_xyx(&bitmap, 20, 50, 10, 0xFFFF);

    uint32_t crc_reversed = crc32(bitmap.buffer, bitmap.size);

    /* Clear and draw with normal order. */
    memset(bitmap.buffer, 0, bitmap.size);
    hagl_draw_hline_xyx(&bitmap, 10, 50, 20, 0xFFFF);

    uint32_t crc_normal = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(crc_normal, crc_reversed);
    PASS();
}

/*
 * Line overflowing right edge of display:
 * Only 10 pixels visible: (310,50) to (319,50)
 */
TEST test_draw_hline_xyw_clip_right(void) {
    hagl_draw_hline_xyw(&bitmap, 310, 50, 20, 0xFFFF);

    /* On line: left endpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 310, 50));

    /* On line: right edge of display */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 319, 50));

    /* Outside: left of start */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 309, 50));

    /* Total: 10 pixels */
    ASSERT_EQ(10, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_hline_xyw_clip_right_regression(void) {
    hagl_draw_hline_xyw(&bitmap, 310, 50, 20, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xFF6B2971, crc);
    PASS();
}

/*
 * Line starting off left edge of display:
 * Only 15 pixels visible: (0,50) to (14,50)
 */
TEST test_draw_hline_xyw_clip_left(void) {
    hagl_draw_hline_xyw(&bitmap, -5, 50, 20, 0xFFFF);

    /* On line: left edge of display */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 0, 50));

    /* On line: right endpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 14, 50));

    /* Outside: right of end */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 15, 50));

    /* Total: 15 pixels */
    ASSERT_EQ(15, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_hline_xyw_clip_left_regression(void) {
    hagl_draw_hline_xyw(&bitmap, -5, 50, 20, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0x1A5ED74E, crc);
    PASS();
}

/*
 * Lines entirely outside the display. Four sub-cases covering
 * all early-return conditions in hagl_hline.c:
 *
 * 1. y below display (y < 0)
 * 2. y above display (y > 239)
 * 3. x right of display (x0 > 319)
 * 4. entirely left of display (x0 + width < 0)
 */
TEST test_draw_hline_xyw_outside(void) {
    /* y below display */
    hagl_draw_hline_xyw(&bitmap, 10, -1, 20, 0xFFFF);
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    /* y above display */
    hagl_draw_hline_xyw(&bitmap, 10, 240, 20, 0xFFFF);
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    /* x right of display */
    hagl_draw_hline_xyw(&bitmap, 320, 50, 20, 0xFFFF);
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    /* entirely left of display */
    hagl_draw_hline_xyw(&bitmap, -30, 50, 20, 0xFFFF);
    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Line clipped by a custom clip window on both sides:
 * Only 51 pixels visible: (50,75) to (100,75)
 */
TEST test_draw_hline_xyw_custom_clip(void) {
    hagl_set_clip(&bitmap, 50, 50, 100, 100);
    hagl_draw_hline_xyw(&bitmap, 40, 75, 80, 0xFFFF);

    /* On line: clip window edges */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 75));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 100, 75));

    /* Outside: just beyond clip window */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 75));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 101, 75));

    /* Total: 51 pixels */
    ASSERT_EQ(51, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_draw_hline_xyw_custom_clip_regression(void) {
    hagl_set_clip(&bitmap, 50, 50, 100, 100);
    hagl_draw_hline_xyw(&bitmap, 40, 75, 80, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xBF25A347, crc);
    PASS();
}

/*
 * Line clipped by left edge of custom clip window only:
 * Line from x=40 to x=59 (width 20) at y=75.
 * Clip window (50,50)-(200,200). Only x=50..59 visible.
 *
 * (40,75)----[50,75)=====(59,75)
 *             ^clip.x0
 */
TEST test_draw_hline_xyw_custom_clip_left(void) {
    hagl_set_clip(&bitmap, 50, 50, 200, 200);
    hagl_draw_hline_xyw(&bitmap, 40, 75, 20, 0xFFFF);

    /* On line: left edge of clip window */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 75));

    /* On line: right endpoint of original line */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 59, 75));

    /* Outside: just left of clip window */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 75));

    /* Outside: just right of original line end */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 60, 75));

    /* Total: 10 pixels */
    ASSERT_EQ(10, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

SUITE(hline_suite) {
    SET_SETUP(setup_callback, NULL);
    SET_TEARDOWN(teardown_callback, NULL);
    RUN_TEST(test_draw_hline_xyw);
    RUN_TEST(test_draw_hline_xyw_regression);
    RUN_TEST(test_draw_hline_xyw_single_pixel);
    RUN_TEST(test_draw_hline_xyw_zero_width);
    RUN_TEST(test_draw_hline_xyw_full_width);
    RUN_TEST(test_draw_hline_xyx_match_xyw);
    RUN_TEST(test_draw_hline_xyx_reversed);
    RUN_TEST(test_draw_hline_xyw_clip_right);
    RUN_TEST(test_draw_hline_xyw_clip_right_regression);
    RUN_TEST(test_draw_hline_xyw_clip_left);
    RUN_TEST(test_draw_hline_xyw_clip_left_regression);
    RUN_TEST(test_draw_hline_xyw_outside);
    RUN_TEST(test_draw_hline_xyw_custom_clip);
    RUN_TEST(test_draw_hline_xyw_custom_clip_regression);
    RUN_TEST(test_draw_hline_xyw_custom_clip_left);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(hline_suite);
    GREATEST_MAIN_END();
}
