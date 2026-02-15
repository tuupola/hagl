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
 *
 *      (50,50)
 */
TEST
test_draw_circle_single_pixel(void) {
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

SUITE(circle_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_draw_circle);
    RUN_TEST(test_draw_circle_regression);
    RUN_TEST(test_draw_circle_single_pixel);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(circle_suite);
    GREATEST_MAIN_END();
}
