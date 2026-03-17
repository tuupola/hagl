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
#include <time.h>

#include "greatest.h"

#include "fps.h"

static fps_instance_t fps;

static void setup_callback(void *data) {
    memset(&fps, 0, sizeof(fps));
}

TEST test_fps_init_defaults(void) {
    fps_init(&fps);

    ASSERT_EQ(0, fps.frames);
    ASSERT_EQ(0.0, fps.current);
    ASSERT_IN_RANGE(0.98, fps.smoothing, 0.001);

    PASS();
}

TEST test_fps_init_custom_smoothing(void) {
    fps.smoothing = 0.5;
    fps_init(&fps);

    ASSERT_IN_RANGE(0.5, fps.smoothing, 0.001);
    ASSERT_EQ(0, fps.frames);
    ASSERT_EQ(0.0, fps.current);

    PASS();
}

TEST test_fps_init_start(void) {
    fps_init(&fps);

    ASSERT(fps.start != 0);

    PASS();
}

TEST test_fps_reset_clears_frames_and_current(void) {
    fps_init(&fps);

    fps_update(&fps);
    fps_update(&fps);
    fps_update(&fps);

    /* Make sure frames accumulated before reset. */
    ASSERT_EQ(3, fps.frames);

    fps_reset(&fps);

    /* Reset should make them zero again. */
    ASSERT_EQ(0, fps.frames);
    ASSERT_EQ(0.0, fps.current);

    PASS();
}

TEST test_fps_reset_preserves_smoothing(void) {
    fps.smoothing = 0.75;
    fps_init(&fps);

    ASSERT_IN_RANGE(0.75, fps.smoothing, 0.001);

    fps_update(&fps);
    fps_reset(&fps);

    ASSERT_IN_RANGE(0.75, fps.smoothing, 0.001);

    PASS();
}

TEST test_fps_reset_refreshes_start(void) {
    clock_t original;
    clock_t elapsed;

    fps_init(&fps);
    original = fps.start;

    /* Spin until the clock advances at least 1 tick. */
    do {
        elapsed = clock();
    } while (elapsed <= original + 1);

    fps_reset(&fps);

    ASSERT(fps.start != original);

    PASS();
}

TEST test_fps_update_increments_frames(void) {
    uint32_t i;

    fps_init(&fps);

    for (i = 1; i <= 10; i++) {
        fps_update(&fps);
        ASSERT_EQ(i, fps.frames);
    }

    PASS();
}

SUITE(fps_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_fps_init_defaults);
    RUN_TEST(test_fps_init_custom_smoothing);
    RUN_TEST(test_fps_init_start);
    RUN_TEST(test_fps_reset_clears_frames_and_current);
    RUN_TEST(test_fps_reset_preserves_smoothing);
    RUN_TEST(test_fps_reset_refreshes_start);
    RUN_TEST(test_fps_update_increments_frames);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(fps_suite);
    GREATEST_MAIN_END();
}
