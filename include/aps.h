/*

MIT License

Copyright (c) 2018-2023 Mika Tuupola

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

#include <time.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    clock_t start;
    uint64_t value;
    float smoothing;
    float current;
} aps_instance_t;

/**
 * Initialize the given anything per second counter instance
 */
static inline void
aps_init(aps_instance_t *aps)
{
    aps->start = clock() - 1;
    aps->value = 0;
    aps->current = 0.0;

    /* Larger value is less smoothing. */
    if (!aps->smoothing) {
        aps->smoothing = 0.98;
    }
}

/**
 * Update the given anything per second counter instance
 *
 * Use to measure the rendering speed. Should be called always
 * after flushing the back buffer.
 *
 * @return current aps
 */
static inline float
aps_update(aps_instance_t *aps, uint32_t add)
{


    float measured = 0.0;
    clock_t ticks = clock() - aps->start;;

    aps->value += add;

    measured = aps->value / (float) ticks * CLOCKS_PER_SEC;
    aps->current = (measured * aps->smoothing) + (aps->current * (1.0 - aps->smoothing));

    return aps->current;
}

/**
 * Reset the given anything per second counter instance
 */
static inline void
aps_reset(aps_instance_t *aps)
{
    aps->start = clock() - 1;
    aps->value = 0;
    aps->current = 0;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAGL_APS_H */