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
 * Single pixel at (100, 120):
 */
TEST
test_put_pixel(void) {
    hagl_put_pixel(&backend, 100, 120, 0xFFFF);

    /* On pixel: the pixel itself */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 100, 120));

    /* Outside: all four neighbors */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 99, 120));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 101, 120));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 100, 119));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 100, 121));

    /* Outside: diagonal neighbors */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 99, 119));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 101, 119));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 99, 121));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 101, 121));

    /* Total: exactly 1 pixel */
    ASSERT_EQ(1, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST
test_put_pixel_regression(void) {
    hagl_put_pixel(&backend, 100, 120, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0xDE7EC0EF, crc);
    PASS();
}

/*
 * Single pixel with a specific color (0x1234):
 */
TEST
test_put_pixel_color(void) {
    hagl_put_pixel(&backend, 50, 75, 0x1234);

    /* On pixel: correct color returned */
    ASSERT_EQ(0x1234, hagl_get_pixel(&backend, 50, 75));

    /* Outside: neighbors are black */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 49, 75));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 51, 75));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 74));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 50, 76));

    /* Total: exactly 1 pixel of color 0x1234 */
    ASSERT_EQ(1, count_pixels(&backend, 0x1234));

    /* Total: no white pixels */
    ASSERT_EQ(0, count_pixels(&backend, 0xFFFF));

    PASS();
}

/*
 * Overwrite pixel with a new color:
 */
TEST
test_put_pixel_overwrite(void) {
    hagl_put_pixel(&backend, 80, 60, 0xAAAA);

    /* On pixel: first color */
    ASSERT_EQ(0xAAAA, hagl_get_pixel(&backend, 80, 60));

    /* Overwrite with a new color */
    hagl_put_pixel(&backend, 80, 60, 0x5555);

    /* On pixel: second color replaced the first */
    ASSERT_EQ(0x5555, hagl_get_pixel(&backend, 80, 60));

    /* Total: no pixels of the old color remain */
    ASSERT_EQ(0, count_pixels(&backend, 0xAAAA));

    /* Total: exactly 1 pixel of the new color */
    ASSERT_EQ(1, count_pixels(&backend, 0x5555));

    PASS();
}

SUITE(pixel_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_put_pixel);
    RUN_TEST(test_put_pixel_regression);
    RUN_TEST(test_put_pixel_color);
    RUN_TEST(test_put_pixel_overwrite);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(pixel_suite);
    GREATEST_MAIN_END();
}
