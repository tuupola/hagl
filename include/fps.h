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

#ifndef _HAGL_FPS_H
#define _HAGL_FPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Frames per second counter
 *
 * Use to measure rendering speed. Should be called always after flushing
 * the back buffer.
 *
 * float fps;
 * hagl_flush();
 * fps = fps();
 *
 * @return current fps
 */
static inline float fps()
{
    static struct timespec start;
    static uint32_t frames = 0;
    static float current = 0.0;
    static bool firstrun = true;

    struct timespec now;
    uint32_t seconds;
    float measured = 0.0;

    /* Larger value is less smoothing. */
    float smoothing = 0.98;


    if (firstrun) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        firstrun = false;
    }
    frames++;

    clock_gettime(CLOCK_MONOTONIC, &now);
    seconds = now.tv_sec - start.tv_sec;

    if (seconds) {
        measured = frames / (float) seconds;
        current = (measured * smoothing) + (current * (1.0 - smoothing));

        return current;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
#endif /* HAGL_FPS_H */