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

#include <stdint.h>

#include "greatest.h"
#include "hagl/bitmap.h"
#include "hagl/color.h"
#include "rgb565.h"

#define TEST_WIDTH 320
#define TEST_HEIGHT 240
#define TEST_DEPTH 16

static hagl_bitmap_t bitmap;
static uint8_t buffer[TEST_WIDTH * TEST_HEIGHT * (TEST_DEPTH / 8)];

static uint8_t last_r, last_g, last_b;
static hagl_color_t mock_color_result;

static hagl_color_t mock_color(void *self, uint8_t r, uint8_t g, uint8_t b) {
    last_r = r;
    last_g = g;
    last_b = b;
    return mock_color_result;
}

static void setup_callback(void *data) {
    memset(buffer, 0, sizeof(buffer));
    hagl_bitmap_init(&bitmap, TEST_WIDTH, TEST_HEIGHT, TEST_DEPTH, buffer);
    last_r = last_g = last_b = 0;
    mock_color_result = 0;
}

TEST test_color_default_black(void) {
    bitmap.color = NULL;

    hagl_color_t result = hagl_color(&bitmap, 0, 0, 0);
    hagl_color_t expected = rgb565(0, 0, 0);

    ASSERT_EQ(expected, result);

    PASS();
}

TEST test_color_default_white(void) {
    bitmap.color = NULL;

    hagl_color_t result = hagl_color(&bitmap, 255, 255, 255);
    hagl_color_t expected = rgb565(255, 255, 255);

    ASSERT_EQ(expected, result);

    PASS();
}

TEST test_color_fallback_arbitrary(void) {
    bitmap.color = NULL;

    uint8_t r = 128, g = 64, b = 32;
    hagl_color_t result = hagl_color(&bitmap, r, g, b);
    hagl_color_t expected = rgb565(r, g, b);

    ASSERT_EQ(expected, result);

    PASS();
}

TEST test_color_delegates_to_surface(void) {
    bitmap.color = mock_color;
    mock_color_result = 0x1234;

    hagl_color_t result = hagl_color(&bitmap, 100, 150, 200);

    ASSERT_EQ(100, last_r);
    ASSERT_EQ(150, last_g);
    ASSERT_EQ(200, last_b);
    ASSERT_EQ(0x1234, result);

    PASS();
}

SUITE(color_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_color_default_black);
    RUN_TEST(test_color_default_white);
    RUN_TEST(test_color_fallback_arbitrary);
    RUN_TEST(test_color_delegates_to_surface);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(color_suite);
    GREATEST_MAIN_END();
}
