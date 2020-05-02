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

#include <time.h>
#include <stdbool.h>
#include <stdint.h>

static clock_t ticks;
static clock_t start;
static uint32_t frames = 1;
static float current = 0;
static bool firstrun = true;

float fps()
{
    /* Larger value is less smoothing */
    float smoothing = 0.9;
    float measured = 0;

    if (firstrun) {
        start = clock();
        firstrun = false;
    }
    frames++;

    ticks = clock() - start;
    measured = frames / (float) ticks * CLOCKS_PER_SEC;
    measured = (measured * smoothing) + (current * (1.0 - smoothing));

    return measured;
}

void fps_reset()
{
    firstrun = true;
    frames = 1;
    current = 0;
}