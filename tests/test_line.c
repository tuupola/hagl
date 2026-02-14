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

SUITE(line_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_draw_line_horizontal);
    RUN_TEST(test_draw_line_horizontal_regression);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(line_suite);
    GREATEST_MAIN_END();
}
