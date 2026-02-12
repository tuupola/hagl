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
#include "hagl.h"

static hagl_backend_t backend;

static uint32_t
count_pixels(hagl_backend_t *backend, hagl_color_t color) {
    uint32_t count = 0;
    for (int16_t y = 0; y < backend->height; y++) {
        for (int16_t x = 0; x < backend->width; x++) {
            if (hagl_get_pixel(backend, x, y) == color) {
                count++;
            }
        }
    }
    return count;
}

static void
setup_callback(void *data) {
    memset(&backend, 0, sizeof(hagl_backend_t));
    hagl_hal_init(&backend);
    hagl_set_clip(&backend, 0, 0,  backend.width - 1,  backend.height - 1);
    hagl_clear(&backend);
}

/*
 * Axis-aligned rectangle:
 *
 * (10,10)-----(20,10)
 *   |###########|
 *   |###########|
 * (10,20)-----(20,20)
 */
TEST
test_fill_rectangle_xyxy(void) {
    hagl_fill_rectangle_xyxy(&backend, 10, 10, 20, 20, 0xFFFF);

    /* Inside: center of the rectangle */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 15));

    /* Inside: all four corners */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 20));

    /* Inside: edge midpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 15));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 15));

    /* Outside: 1 pixel beyond each edge */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 21, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 21));

    /* Outside: 1 pixel diagonally beyond each corner */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 21, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 21, 21));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 21));

    /* Total filled area: 11 x 11 = 121 pixels */
    ASSERT_EQ(121, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_fill_rectangle_xyxy_regression(void) {
    hagl_fill_rectangle_xyxy(&backend, 10, 10, 20, 20, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x52CF24BD, crc);
    PASS();
}

TEST
test_fill_rectangle_xyxy_match_xywh(void) {
    /* Draw with _xyxy. */
    hagl_fill_rectangle_xyxy(&backend, 10, 10, 20, 20, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc_xyxy = crc32(backend.buffer, size);

    /* Clear and draw the same area with _xywh. */
    memset(backend.buffer, 0, size);
    hagl_fill_rectangle_xywh(&backend, 10, 10, 11, 11, 0xFFFF);

    uint32_t crc_xywh = crc32(backend.buffer, size);

    ASSERT_EQ(crc_xyxy, crc_xywh);
    PASS();
}

TEST
test_fill_rectangle_xyxy_swapped(void) {
    /* Draw with swapped coordinates (x0 > x1, y0 > y1). */
    hagl_fill_rectangle_xyxy(&backend, 20, 20, 10, 10, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc_swapped = crc32(backend.buffer, size);

    /* Clear and draw with normal coordinates. */
    memset(backend.buffer, 0, size);
    hagl_fill_rectangle_xyxy(&backend, 10, 10, 20, 20, 0xFFFF);

    uint32_t crc_normal = crc32(backend.buffer, size);

    ASSERT_EQ(crc_normal, crc_swapped);
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
TEST
test_fill_rectangle_xyxy_clip_top_left(void) {
    hagl_fill_rectangle_xyxy(&backend, -10, -10, 10, 10, 0xFFFF);

    /* Inside: visible portion */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 5, 5));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 0, 0));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 0));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 0, 10));

    /* Outside: beyond the rectangle inside display */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 11, 5));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 5, 11));

    /* Total filled area: 11 x 11 = 121 pixels */
    ASSERT_EQ(121, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_fill_rectangle_xyxy_clip_top_left_regression(void) {
    hagl_fill_rectangle_xyxy(&backend, -10, -10, 10, 10, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

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
TEST
test_fill_rectangle_xyxy_clip_bottom_right(void) {
    hagl_fill_rectangle_xyxy(&backend, 310, 230, 330, 250, 0xFFFF);

    /* Inside: visible portion */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 315, 235));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 310, 230));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 319, 239));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 319, 230));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 310, 239));

    /* Outside: beyond the rectangle inside display */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 309, 235));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 315, 229));

    /* Total filled area: 10 x 10 = 100 pixels */
    ASSERT_EQ(100, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_fill_rectangle_xyxy_clip_bottom_right_regression(void) {
    hagl_fill_rectangle_xyxy(&backend, 310, 230, 330, 250, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

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
TEST
test_fill_rectangle_xyxy_clip_outside(void) {
    hagl_fill_rectangle_xyxy(&backend, -30, -30, -10, -10, 0xFFFF);

    ASSERT_EQ(0, count_pixels(&backend, 0xFFFF));
    PASS();
}

SUITE(fill_rectangle_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_fill_rectangle_xyxy);
    RUN_TEST(test_fill_rectangle_xyxy_regression);
    RUN_TEST(test_fill_rectangle_xyxy_match_xywh);
    RUN_TEST(test_fill_rectangle_xyxy_swapped);
    RUN_TEST(test_fill_rectangle_xyxy_clip_top_left);
    RUN_TEST(test_fill_rectangle_xyxy_clip_top_left_regression);
    RUN_TEST(test_fill_rectangle_xyxy_clip_bottom_right);
    RUN_TEST(test_fill_rectangle_xyxy_clip_bottom_right_regression);
    RUN_TEST(test_fill_rectangle_xyxy_clip_outside);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(fill_rectangle_suite);
    GREATEST_MAIN_END();
}
