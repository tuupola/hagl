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
 * Circle outline with center (100,100) and radius 10:
 *
 *          (100,90)
 *         /        \
 *        /          \
 * (90,100)  (100,100) (110,100)
 *        \          /
 *         \        /
 *          (100,110)
 */
TEST
test_draw_circle(void) {
    hagl_draw_circle(&backend, 100, 100, 10, 0xFFFF);

    /* On outline: cardinal points */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 100, 90));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 100, 110));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 90, 100));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 110, 100));

    /* Inside: center is empty (outline only) */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 100, 100));

    /* Outside: 1 pixel beyond each cardinal point */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 100, 89));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 100, 111));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 89, 100));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 111, 100));

    /* Total: 56 pixels */
    ASSERT_EQ(56, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_draw_circle_regression(void) {
    hagl_draw_circle(&backend, 100, 100, 10, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x2AE78002, crc);
    PASS();
}

/*
 * Circle with radius 0:
 */
TEST
test_draw_circle_radius_0(void) {
    hagl_draw_circle(&backend, 50, 50, 0, 0xFFFF);

    /* On outline: the center pixel */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 50, 50));

    /* Outside: immediate neighbors are empty */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 49, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 51, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 51));

    /* Total: 1 pixel */
    ASSERT_EQ(1, count_pixels(&backend, 0xFFFF));

    PASS();
}

/*
 * Circle with radius 1:
 *
 *        (100,99)
 *          |
 * (99,100)   (101,100)
 *          |
 *        (100,101)
 *
 * Produces a cross/diamond with 4 pixels. Center is empty.
 */
TEST
test_draw_circle_radius_1(void) {
    hagl_draw_circle(&backend, 100, 100, 1, 0xFFFF);

    /* On outline: cardinal neighbors */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 100, 99));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 100, 101));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 99, 100));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 101, 100));

    /* Inside: center is empty (outline only) */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 100, 100));

    /* Outside: diagonal neighbors */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 99, 99));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 101, 99));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 99, 101));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 101, 101));

    /* Total: 4 pixels */
    ASSERT_EQ(4, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_draw_circle_radius_1_regression(void) {
    hagl_draw_circle(&backend, 100, 100, 1, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x8A89E763, crc);
    PASS();
}

/*
 * Circle clipped by top-left corner of display:
 * Center at (5,5), radius 10. Top and left arcs are clipped.
 */
TEST
test_draw_circle_clip_top_left(void) {
    hagl_draw_circle(&backend, 5, 5, 10, 0xFFFF);

    /* On outline: visible cardinal points */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 5, 15));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 5));

    /* Clipped: top and left cardinals are off-screen at (5,-5) and (-5,5) */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 5, 0));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 0, 5));

    /* Inside: center is empty */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 5, 5));

    /* On outline: near-diagonal points */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 12, 12));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 14, 0));

    /* Total: 25 visible pixels */
    ASSERT_EQ(25, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_draw_circle_clip_top_left_regression(void) {
    hagl_draw_circle(&backend, 5, 5, 10, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x92716B4E, crc);
    PASS();
}

/*
 * Circle entirely outside the display:
 * Center at (-50,-50), radius 10. No pixels visible.
 *
 *       (-50,-40)
 *      /         \
 * (-60,-50)  (-40,-50)
 *      \         /
 *       (-50,-60)
 */
TEST
test_draw_circle_clip_outside(void) {
    hagl_draw_circle(&backend, -50, -50, 10, 0xFFFF);

    ASSERT_EQ(0, count_pixels(&backend, 0xFFFF));
    PASS();
}

/*
 * Circle clipped by a custom clip window:
 * Center at (15,15), radius 10, clip (10,10)-(25,25).
 */
TEST
test_draw_circle_custom_clip(void) {
    hagl_set_clip(&backend, 10, 10, 25, 25);
    hagl_draw_circle(&backend, 15, 15, 10, 0xFFFF);

    /* On outline: visible cardinal points */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 25));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 25, 15));

    /* Clipped: top and left cardinals at (15,5) and (5,15) */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 10));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 10, 15));

    /* Inside: center is empty */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 15));

    /* On outline: near-diagonal points */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 22, 22));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 24, 10));

    /* Total: 25 visible pixels */
    ASSERT_EQ(25, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_draw_circle_custom_clip_regression(void) {
    hagl_set_clip(&backend, 10, 10, 25, 25);
    hagl_draw_circle(&backend, 15, 15, 10, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x260CB22C, crc);
    PASS();
}

/*
 * Filled circle with center (100,100) and radius 10:
 *
 *          (100,90)
 *         /XXXXXXXX\
 *        /XXXXXXXXXX\
 * (90,100)XXXXXXXXXXXX(109,100)
 *        \XXXXXXXXXX/
 *         \XXXXXXXX/
 *          (100,110)
 *
 * TODO: hline width is y*2 so the rightmost pixel on each
 * row is 1 short of the outline. The widest row covers
 * x=90..109 (20 pixels) instead of x=90..110 (21 pixels).
 */
TEST
test_fill_circle(void) {
    hagl_fill_circle(&backend, 100, 100, 10, 0xFFFF);

    /* On fill: cardinal points */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 100, 90));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 100, 110));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 90, 100));

    /* Inside: center is filled */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 100, 100));

    /* Outside: 1 pixel beyond each cardinal point */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 100, 89));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 100, 111));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 89, 100));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 111, 100));

    /* Total: 328 pixels */
    ASSERT_EQ(328, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_fill_circle_regression(void) {
    hagl_fill_circle(&backend, 100, 100, 10, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x7BCF5A7A, crc);
    PASS();
}

/*
 * Filled circle with radius 0:
 */
TEST
test_fill_circle_radius_0(void) {
    hagl_fill_circle(&backend, 50, 50, 0, 0xFFFF);

    /* On fill: the center pixel */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 50, 50));

    /* Outside: immediate neighbors are empty */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 49, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 51, 50));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 49));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 51));

    /* Total: 1 pixel */
    ASSERT_EQ(1, count_pixels(&backend, 0xFFFF));

    PASS();
}

/*
 * Filled circle entirely outside the display:
 * Center at (-50,-50), radius 10. No pixels visible.
 *
 *       (-50,-40)
 *      /XXXXXXXX\
 * (-60,-50)XX(-40,-50)
 *      \XXXXXXXX/
 *       (-50,-60)
 */
TEST
test_fill_circle_clip_outside(void) {
    hagl_fill_circle(&backend, -50, -50, 10, 0xFFFF);

    ASSERT_EQ(0, count_pixels(&backend, 0xFFFF));
    PASS();
}

/*
 * Filled circle clipped by a custom clip window:
 * Center at (15,15), radius 10, clip (10,10)-(25,25).
 */
TEST
test_fill_circle_custom_clip(void) {
    hagl_set_clip(&backend, 10, 10, 25, 25);
    hagl_fill_circle(&backend, 15, 15, 10, 0xFFFF);

    /* Inside clip: center is filled */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 15));

    /* Inside clip: top corners are filled */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 25, 10));

    /* Inside clip: bottom cardinal */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 25));

    /* Inside clip: right edge */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 25, 15));

    /* Outside clip: clipped cardinals */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 5));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 5, 15));

    /* Outside clip: just beyond clip edges */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 9));

    /* Total: 240 pixels */
    ASSERT_EQ(240, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_fill_circle_custom_clip_regression(void) {
    hagl_set_clip(&backend, 10, 10, 25, 25);
    hagl_fill_circle(&backend, 15, 15, 10, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x7AE96857, crc);
    PASS();
}

SUITE(circle_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_draw_circle);
    RUN_TEST(test_draw_circle_regression);
    RUN_TEST(test_draw_circle_radius_0);
    RUN_TEST(test_draw_circle_radius_1);
    RUN_TEST(test_draw_circle_radius_1_regression);
    RUN_TEST(test_draw_circle_clip_top_left);
    RUN_TEST(test_draw_circle_clip_top_left_regression);
    RUN_TEST(test_draw_circle_clip_outside);
    RUN_TEST(test_draw_circle_custom_clip);
    RUN_TEST(test_draw_circle_custom_clip_regression);
    RUN_TEST(test_fill_circle);
    RUN_TEST(test_fill_circle_regression);
    RUN_TEST(test_fill_circle_radius_0);
    RUN_TEST(test_fill_circle_clip_outside);
    RUN_TEST(test_fill_circle_custom_clip);
    RUN_TEST(test_fill_circle_custom_clip_regression);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(circle_suite);
    GREATEST_MAIN_END();
}
