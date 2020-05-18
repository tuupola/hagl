/*

MIT License

Copyright (c) 2018-2020 Mika Tuupola

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

#ifndef _HAGL_APS_H
#define _HAGL_APS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#define APS_RESET   (UINT32_MAX)

/**
 * Anything per second counter
 *
 * Use to measure anything per second. For example always after downlaoding
 * 1024 bytes you could get downloaded bytes per second with:
 *
 * float bps;
 * bps = aps(1024);
 *
 * You camn reset the counter by passing APS_RESET as the parameter.
 *
 * bps = aps(APS_RESET);
 *
 * @param add amount to add since the last call
 * @return average per second of given values
 */
static inline float aps(uint32_t add)
{
    static struct timespec start;
    static uint64_t value = 0;
    static float current = 0.0;
    static bool firstrun = true;

    struct timespec now;
    static uint32_t seconds;
    float measured = 0.0;

    /* Larger value is less smoothing. */
    float smoothing = 0.95;

    if (firstrun) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        firstrun = false;
    }

    if (APS_RESET == add) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        value = 0;
        current = 0;
        firstrun = false;
        return 0;
    }

    value += add;

    clock_gettime(CLOCK_MONOTONIC, &now);
    seconds = now.tv_sec - start.tv_sec;

    if (seconds) {
        measured = value / (float) seconds;
        current = (measured * smoothing) + (current * (1.0 - smoothing));

        return current;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_APS_H */