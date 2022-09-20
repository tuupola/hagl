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

#include <time.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    clock_t start;
    uint32_t frames;
    float smoothing;
    float current;
} fps_instance_t;

/**
 * Initialize the given FPS counter instance
 */
static inline void
fps_init(fps_instance_t *fps) {
    fps->start = clock() - 1;
    fps->frames = 0;
    fps->current = 0.0;

    /* Larger value is less smoothing. */
    if (!fps->smoothing) {
        fps->smoothing = 0.98;
    }
}

/**
 * Update the given FPS counter instance
 *
 * Use to measure the rendering speed. Should be called always
 * after flushing the back buffer.
 *
 * @return current fps
 */
static inline float
fps_update(fps_instance_t *fps)
{
    float measured = 0.0;
    clock_t ticks = clock() - fps->start;;

    fps->frames++;

    measured = fps->frames / (float) ticks * CLOCKS_PER_SEC;
    fps->current = (measured * fps->smoothing) + (fps->current * (1.0 - fps->smoothing));

    return fps->current;
}

/**
 * Reset the given FPS counter instance
 */
static inline void
fps_reset(fps_instance_t *fps) {
    fps->start = clock() - 1;
    fps->frames = 0;
    fps->current = 0;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HAGL_FPS_H */