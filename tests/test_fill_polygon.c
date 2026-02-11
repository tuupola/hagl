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

static void setup_callback(void *data) {
    memset(&backend, 0, sizeof(hagl_backend_t));
    hagl_hal_init(&backend);
    hagl_set_clip(&backend, 0, 0,  backend.width - 1,  backend.height - 1);
    hagl_clear(&backend);
}

TEST fill_polygon_square(void) {
    /* Square: 10,10 -> 20,10 -> 20,20 -> 10,20 */
    int16_t vertices[] = {10, 10, 20, 10, 20, 20, 10, 20};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    /* Inside: center of the square */
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

    PASS();
}

TEST fill_polygon_square_regression(void) {
    int16_t vertices[] = {10, 10, 20, 10, 20, 20, 10, 20};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x52CF24BD, crc);
    PASS();
}

SUITE(polygon_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(fill_polygon_square);
    RUN_TEST(fill_polygon_square_regression);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(polygon_suite);
    GREATEST_MAIN_END();
}
