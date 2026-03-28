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

#include "aps.h"

static aps_instance_t aps;

static void setup_callback(void *data) {
    memset(&aps, 0, sizeof(aps));
}

TEST test_aps_init_defaults(void) {
    aps_init(&aps);

    ASSERT_EQ(0, aps.value);
    ASSERT_EQ(0.0, aps.current);
    ASSERT_IN_RANGE(0.98, aps.smoothing, 0.001);

    PASS();
}

TEST test_aps_init_custom_smoothing(void) {
    aps.smoothing = 0.5;
    aps_init(&aps);

    ASSERT_IN_RANGE(0.5, aps.smoothing, 0.001);
    ASSERT_EQ(0, aps.value);
    ASSERT_EQ(0.0, aps.current);

    PASS();
}

TEST test_aps_init_start(void) {
    aps_init(&aps);

    ASSERT(aps.start != 0);

    PASS();
}

TEST test_aps_reset_clears_value_and_current(void) {
    aps_init(&aps);

    aps_update(&aps, 1);
    aps_update(&aps, 1);
    aps_update(&aps, 1);

    /* Make sure value accumulated before reset. */
    ASSERT_EQ(3, aps.value);

    aps_reset(&aps);

    /* Reset should make them zero again. */
    ASSERT_EQ(0, aps.value);
    ASSERT_EQ(0.0, aps.current);

    PASS();
}

TEST test_aps_reset_preserves_smoothing(void) {
    aps.smoothing = 0.75;
    aps_init(&aps);

    ASSERT_IN_RANGE(0.75, aps.smoothing, 0.001);

    aps_update(&aps, 1);
    aps_reset(&aps);

    ASSERT_IN_RANGE(0.75, aps.smoothing, 0.001);

    PASS();
}

TEST test_aps_reset_refreshes_start(void) {
    clock_t original;
    clock_t elapsed;

    aps_init(&aps);
    original = aps.start;

    /* Spin until the clock advances at least 1 tick. */
    do {
        elapsed = clock();
    } while (elapsed <= original + 1);

    aps_reset(&aps);

    ASSERT(aps.start != original);

    PASS();
}

TEST test_aps_update_increments_value(void) {
    uint32_t i;

    aps_init(&aps);

    for (i = 1; i <= 10; i++) {
        aps_update(&aps, 1);
        ASSERT_EQ(i, aps.value);
    }

    PASS();
}

SUITE(aps_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_aps_init_defaults);
    RUN_TEST(test_aps_init_custom_smoothing);
    RUN_TEST(test_aps_init_start);
    RUN_TEST(test_aps_reset_clears_value_and_current);
    RUN_TEST(test_aps_reset_preserves_smoothing);
    RUN_TEST(test_aps_reset_refreshes_start);
    RUN_TEST(test_aps_update_increments_value);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(aps_suite);
    GREATEST_MAIN_END();
}
