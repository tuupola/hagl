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
 * Horizontal line:
 *
 * (10,50)-----(20,50)
 */
TEST
test_draw_line_horizontal(void) {
    hagl_draw_line(&backend, 10, 50, 20, 50, 0xFFFF);

    /* On line: endpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 50));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 50));

    /* On line: midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 50));

    /* Outside: above and below midpoint */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 51));

    /* Outside: left and right of endpoints */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 21, 50));

    /* Total: 11 pixels */
    ASSERT_EQ(11, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_draw_line_horizontal_regression(void) {
    hagl_draw_line(&backend, 10, 50, 20, 50, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

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
    hagl_draw_line(&backend, 50, 10, 50, 20, 0xFFFF);

    /* On line: endpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 50, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 50, 20));

    /* On line: midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 50, 15));

    /* Outside: left and right of midpoint */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 49, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 51, 15));

    /* Outside: above and below endpoints */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 21));

    /* Total: 11 pixels */
    ASSERT_EQ(11, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_draw_line_vertical_regression(void) {
    hagl_draw_line(&backend, 50, 10, 50, 20, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

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
    hagl_draw_line(&backend, 10, 10, 20, 20, 0xFFFF);

    /* On line: endpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 20));

    /* On line: midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 15));

    /* On line: staircase neighbors of midpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 14, 15));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 16));

    /* Outside: off the staircase */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 16, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 14));

    /* Outside: beyond endpoints */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 21, 21));

    /* Total: 21 pixels (staircase pattern) */
    ASSERT_EQ(21, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_draw_line_diagonal_regression(void) {
    hagl_draw_line(&backend, 10, 10, 20, 20, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

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
    hagl_draw_line(&backend, 50, 50, 50, 50, 0xFFFF);

    /* On line: the pixel itself */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 50, 50));

    /* Outside: all four neighbors */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 49, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 51, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 51));

    /* Total: 1 pixel */
    ASSERT_EQ(1, count_pixels(&backend, 0xFFFF));

    PASS();
}

/*
 * Line clipped by top-left corner of display:
 */
TEST
test_draw_line_clip_top_left(void) {
    hagl_draw_line(&backend, -10, -10, 10, 10, 0xFFFF);

    /* On line: clipped start at origin */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 0, 0));

    /* On line: visible endpoint */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 10));

    /* On line: midpoint of visible portion */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 5, 5));

    /* Outside: beyond visible endpoint */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 11, 11));

    /* Outside: off the staircase */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 1, 0));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 0, 2));

    /* Total: 21 pixels (staircase from origin to (10,10)) */
    ASSERT_EQ(21, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_draw_line_clip_top_left_regression(void) {
    hagl_draw_line(&backend, -10, -10, 10, 10, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0xC93AD1B1, crc);
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
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(line_suite);
    GREATEST_MAIN_END();
}
