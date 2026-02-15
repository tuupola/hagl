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

#include <stdbool.h>

#include "greatest.h"
#include "hagl.h"

/*
 * All tests use the same clip window (10,10)-(50,50).
 *
 * (10,10)-----------(50,10)
 *   |                  |
 *   |                  |
 *   |                  |
 * (10,50)-----------(50,50)
 */
static const hagl_window_t window = {10, 10, 50, 50};

/*
 * Both endpoints inside the clip window:
 *
 * (10,10)-----------(50,10)
 *   |                  |
 *   |  (20,20)         |
 *   |      \           |
 *   |       (40,40)    |
 *   |                  |
 * (10,50)-----------(50,50)
 *
 * Coordinates returned unchanged.
 */
TEST
test_clip_line_inside(void) {
    int16_t x0 = 20, y0 = 20, x1 = 40, y1 = 40;

    bool result = hagl_clip_line(&x0, &y0, &x1, &y1, window);

    ASSERT_EQ(true, result);
    ASSERT_EQ(20, x0);
    ASSERT_EQ(20, y0);
    ASSERT_EQ(40, x1);
    ASSERT_EQ(40, y1);

    PASS();
}

/*
 * Both endpoints on the clip window boundary:
 *
 * (10,10)-----------(50,10)
 *    \                 |
 *     \                |
 *      \               |
 *       \              |
 * (10,50)-----------(50,50)
 *
 * Boundary is inclusive, coordinates returned unchanged.
 */
TEST
test_clip_line_boundary(void) {
    int16_t x0 = 10, y0 = 10, x1 = 50, y1 = 50;

    bool result = hagl_clip_line(&x0, &y0, &x1, &y1, window);

    ASSERT_EQ(true, result);
    ASSERT_EQ(10, x0);
    ASSERT_EQ(10, y0);
    ASSERT_EQ(50, x1);
    ASSERT_EQ(50, y1);

    PASS();
}

TEST
test_clip_line_reject(void) {
    int16_t x0, y0, x1, y1;

    /* Left */
    x0 = -10; y0 = 30; x1 = -5; y1 = 30;
    ASSERT_EQ(false, hagl_clip_line(&x0, &y0, &x1, &y1, window));

    /* Right */
    x0 = 60; y0 = 30; x1 = 70; y1 = 30;
    ASSERT_EQ(false, hagl_clip_line(&x0, &y0, &x1, &y1, window));

    /* Above */
    x0 = 30; y0 = -10; x1 = 30; y1 = -5;
    ASSERT_EQ(false, hagl_clip_line(&x0, &y0, &x1, &y1, window));

    /* Below */
    x0 = 30; y0 = 60; x1 = 30; y1 = 70;
    ASSERT_EQ(false, hagl_clip_line(&x0, &y0, &x1, &y1, window));

    PASS();
}

SUITE(clip_suite) {
    RUN_TEST(test_clip_line_inside);
    RUN_TEST(test_clip_line_boundary);
    RUN_TEST(test_clip_line_reject);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(clip_suite);
    GREATEST_MAIN_END();
}
