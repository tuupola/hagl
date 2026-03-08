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
#include "hagl/rectangle.h"
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
 * Axis-aligned rectangle:
 *
 * (10,10)-----(20,10)
 *   |###########|
 *   |###########|
 * (10,20)-----(20,20)
 */
TEST test_fill_rectangle_xyxy(void) {
    hagl_fill_rectangle_xyxy(&bitmap, 10, 10, 20, 20, 0xFFFF);

    /* Inside: center of the rectangle */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 15, 15));

    /* Inside: all four corners */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 20, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 20, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 20));

    /* Inside: edge midpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 15, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 20, 15));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 15, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 15));

    /* Outside: 1 pixel beyond each edge */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 9, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 21, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 15, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 15, 21));

    /* Outside: 1 pixel diagonally beyond each corner */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 9, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 21, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 21, 21));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 9, 21));

    /* Total filled area: 11 x 11 = 121 pixels */
    ASSERT_EQ(121, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_fill_rectangle_xyxy_regression(void) {
    hagl_fill_rectangle_xyxy(&bitmap, 10, 10, 20, 20, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0x52CF24BD, crc);
    PASS();
}

TEST test_fill_rectangle_xyxy_match_xywh(void) {
    /* Draw with _xyxy. */
    hagl_fill_rectangle_xyxy(&bitmap, 10, 10, 20, 20, 0xFFFF);

    uint32_t crc_xyxy = crc32(bitmap.buffer, bitmap.size);

    /* Clear and draw the same area with _xywh. */
    memset(bitmap.buffer, 0, bitmap.size);
    hagl_fill_rectangle_xywh(&bitmap, 10, 10, 11, 11, 0xFFFF);

    uint32_t crc_xywh = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(crc_xyxy, crc_xywh);
    PASS();
}

TEST test_fill_rectangle_xyxy_swapped(void) {
    /* Draw with swapped coordinates (x0 > x1, y0 > y1). */
    hagl_fill_rectangle_xyxy(&bitmap, 20, 20, 10, 10, 0xFFFF);

    uint32_t crc_swapped = crc32(bitmap.buffer, bitmap.size);

    /* Clear and draw with normal coordinates. */
    memset(bitmap.buffer, 0, bitmap.size);
    hagl_fill_rectangle_xyxy(&bitmap, 10, 10, 20, 20, 0xFFFF);

    uint32_t crc_normal = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(crc_normal, crc_swapped);
    PASS();
}

/*
 * Single pixel (width = 1, height = 1):
 *
 * (50,50)
 */
TEST test_fill_rectangle_xyxy_single_pixel(void) {
    hagl_fill_rectangle_xyxy(&bitmap, 50, 50, 50, 50, 0xFFFF);

    /* Inside: the pixel itself */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 50));

    /* Outside: all four neighbors */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 51, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 51));

    /* Total filled area: 1 x 1 = 1 pixel */
    ASSERT_EQ(1, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Horizontal line (height = 1):
 *
 * (10,50)-----(20,50)
 */
TEST test_fill_rectangle_xyxy_horizontal_line(void) {
    hagl_fill_rectangle_xyxy(&bitmap, 10, 50, 20, 50, 0xFFFF);

    /* Inside: endpoints and midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 50));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 20, 50));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 15, 50));

    /* Outside: above and below */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 15, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 15, 51));

    /* Outside: left and right */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 9, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 21, 50));

    /* Total filled area: 11 x 1 = 11 pixels */
    ASSERT_EQ(11, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_fill_rectangle_xyxy_horizontal_line_regression(void) {
    hagl_fill_rectangle_xyxy(&bitmap, 10, 50, 20, 50, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xDF4E4483, crc);
    PASS();
}

/*
 * Vertical line (width = 1):
 *
 * (50,10)
 *   |
 *   |
 * (50,20)
 */
TEST test_fill_rectangle_xyxy_vertical_line(void) {
    hagl_fill_rectangle_xyxy(&bitmap, 50, 10, 50, 20, 0xFFFF);

    /* Inside: endpoints and midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 15));

    /* Outside: left and right */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 51, 15));

    /* Outside: above and below */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 50, 21));

    /* Total filled area: 1 x 11 = 11 pixels */
    ASSERT_EQ(11, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_fill_rectangle_xyxy_vertical_line_regression(void) {
    hagl_fill_rectangle_xyxy(&bitmap, 50, 10, 50, 20, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0x538BB775, crc);
    PASS();
}

/*
 * Rectangle clipped by top-left corner of display:
 *
 *            .(0,0)
 *             |
 * (-10,-10)-------(10,-10)
 *    |        |######|
 *    |        |######|
 * (-10,10)----+---(10,10)
 *
 * Only the (0,0) to (10,10) portion is visible.
 */
TEST test_fill_rectangle_xyxy_clip_top_left(void) {
    hagl_fill_rectangle_xyxy(&bitmap, -10, -10, 10, 10, 0xFFFF);

    /* Inside: visible portion */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 5, 5));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 0, 0));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 0));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 0, 10));

    /* Outside: beyond the rectangle inside display */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 11, 5));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 5, 11));

    /* Total filled area: 11 x 11 = 121 pixels */
    ASSERT_EQ(121, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_fill_rectangle_xyxy_clip_top_left_regression(void) {
    hagl_fill_rectangle_xyxy(&bitmap, -10, -10, 10, 10, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0x22DBDA01, crc);
    PASS();
}

/*
 * Rectangle clipped by bottom-right corner of display (320x240):
 *
 * (310,230)-------(330,230)
 *    |######|      |
 *    |######|      |
 * (310,250)-+---(330,250)
 *           |
 *        .(319,239)
 *
 * Only the (310,230) to (319,239) portion is visible.
 */
TEST test_fill_rectangle_xyxy_clip_bottom_right(void) {
    hagl_fill_rectangle_xyxy(&bitmap, 310, 230, 330, 250, 0xFFFF);

    /* Inside: visible portion */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 315, 235));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 310, 230));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 319, 239));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 319, 230));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 310, 239));

    /* Outside: beyond the rectangle inside display */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 309, 235));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 315, 229));

    /* Total filled area: 10 x 10 = 100 pixels */
    ASSERT_EQ(100, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_fill_rectangle_xyxy_clip_bottom_right_regression(void) {
    hagl_fill_rectangle_xyxy(&bitmap, 310, 230, 330, 250, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xB46E0159, crc);
    PASS();
}

/*
 * Rectangle entirely outside display:
 *
 * (-30,-30)-----(-10,-30)
 *    |              |
 *    |              |
 * (-30,-10)-----(-10,-10)
 *
 *                     .(0,0) display starts here
 */
TEST test_fill_rectangle_xyxy_clip_outside(void) {
    hagl_fill_rectangle_xyxy(&bitmap, -30, -30, -10, -10, 0xFFFF);

    ASSERT_EQ(0, count_pixels(&bitmap, 0xFFFF));
    PASS();
}

/*
 * Rectangle clipped by a custom clip window:
 *
 *       (40,40)-----------(110,40)
 *          |                  |
 *          | (50,50)---(100,50)
 *          |   |#########|  |
 *          |   |#########|  |
 *          | (50,100)-(100,100)
 *          |                  |
 *       (40,110)----------(110,110)
 *
 * Clip window set to (50,50)-(100,100).
 * Only the (50,50) to (100,100) portion is visible.
 */
TEST test_fill_rectangle_xyxy_custom_clip(void) {
    hagl_set_clip(&bitmap, 50, 50, 100, 100);
    hagl_fill_rectangle_xyxy(&bitmap, 40, 40, 110, 110, 0xFFFF);

    /* Inside: clip window corners */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 50));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 100, 50));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 50, 100));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 100, 100));

    /* Inside: center of clip window */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 75, 75));

    /* Outside: 1 pixel beyond clip window */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 49, 75));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 101, 75));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 75, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 75, 101));

    /* Total filled area: 51 x 51 = 2601 pixels */
    ASSERT_EQ(2601, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

TEST test_fill_rectangle_xyxy_custom_clip_regression(void) {
    hagl_set_clip(&bitmap, 50, 50, 100, 100);
    hagl_fill_rectangle_xyxy(&bitmap, 40, 40, 110, 110, 0xFFFF);

    uint32_t crc = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(0xDC5E143B, crc);
    PASS();
}

SUITE(fill_rectangle_suite) {
    SET_SETUP(setup_callback, NULL);
    SET_TEARDOWN(teardown_callback, NULL);
    RUN_TEST(test_fill_rectangle_xyxy);
    RUN_TEST(test_fill_rectangle_xyxy_regression);
    RUN_TEST(test_fill_rectangle_xyxy_match_xywh);
    RUN_TEST(test_fill_rectangle_xyxy_swapped);
    RUN_TEST(test_fill_rectangle_xyxy_single_pixel);
    RUN_TEST(test_fill_rectangle_xyxy_horizontal_line);
    RUN_TEST(test_fill_rectangle_xyxy_horizontal_line_regression);
    RUN_TEST(test_fill_rectangle_xyxy_vertical_line);
    RUN_TEST(test_fill_rectangle_xyxy_vertical_line_regression);
    RUN_TEST(test_fill_rectangle_xyxy_clip_top_left);
    RUN_TEST(test_fill_rectangle_xyxy_clip_top_left_regression);
    RUN_TEST(test_fill_rectangle_xyxy_clip_bottom_right);
    RUN_TEST(test_fill_rectangle_xyxy_clip_bottom_right_regression);
    RUN_TEST(test_fill_rectangle_xyxy_clip_outside);
    RUN_TEST(test_fill_rectangle_xyxy_custom_clip);
    RUN_TEST(test_fill_rectangle_xyxy_custom_clip_regression);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(fill_rectangle_suite);
    GREATEST_MAIN_END();
}
