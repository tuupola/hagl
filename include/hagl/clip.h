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

#ifndef _HAGL_CLIP_H
#define _HAGL_CLIP_H

#include <stdint.h>
#include <stdbool.h>

#include "hagl/window.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

bool hagl_clip_line(int16_t *x0, int16_t *y0, int16_t *x1, int16_t *y1, hagl_window_t window);

/**
 * Set the clip window
 *
 * Clip windows restricts the drawable area. It does not affect
 * the coordinates.
 *
 * @param surface
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 */
void
hagl_set_clip(void *surface, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAGL_CLIP_H */