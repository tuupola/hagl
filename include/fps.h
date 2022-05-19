/*

MIT License

Copyright (c) 2018-2022 Mika Tuupola

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
    static clock_t start;
    static uint32_t frames = 1;
    static float current = 0.0;
    static bool firstrun = true;
    clock_t ticks;

    /* Larger value is less smoothing. */
    float smoothing = 0.98;
    float measured = 0.0;

    if (firstrun) {
        start = clock() - 1;
        firstrun = false;
    }
    frames++;

    ticks = clock() - start;
    measured = frames / (float) ticks * CLOCKS_PER_SEC;
    current = (measured * smoothing) + (current * (1.0 - smoothing));

    return current;
}

#ifdef __cplusplus
}
#endif
#endif /* HAGL_FPS_H */