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

#ifndef _HAGL_H
#define _HAGL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#include "hagl/pixel.h"
#include "hagl/surface.h"
#include "hagl/backend.h"
#include "hagl_hal.h"
#include "hagl/bitmap.h"
#include "hagl/clip.h"
#include "hagl/line.h"
#include "hagl/hline.h"
#include "hagl/vline.h"
#include "hagl/rectangle.h"
#include "hagl/circle.h"
#include "hagl/ellipse.h"
#include "hagl/polygon.h"
#include "hagl/triangle.h"
#include "hagl/image.h"
#include "hagl/blit.h"
#include "hagl/char.h"

#define ABS(x)  ((x) > 0 ? (x) : -(x))

#define HAGL_CHAR_BUFFER_SIZE    (16 * 16 * DISPLAY_DEPTH / 2)

#define HAGL_OK                  (0)
#define HAGL_ERR_GENERAL         (1)
#define HAGL_ERR_FILE_IO         (2)

/**
 * Clear area of the current clip window
 */
void hagl_clear_clip_window();

/**
 * Clear the display
 */
void hagl_clear(void *surface);
hagl_backend_t *hagl_init(void);
size_t hagl_flush(hagl_backend_t *backend);
void hagl_close(hagl_backend_t *backend);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_H */
