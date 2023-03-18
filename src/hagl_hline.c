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

#include "hagl/color.h"
#include "hagl/line.h"
#include "hagl/surface.h"

void
hagl_draw_hline_xyw(void const *_surface, int16_t x0, int16_t y0, uint16_t w, hagl_color_t color)
{
    const hagl_surface_t *surface = _surface;

    if (surface->hline) {
        int16_t width = w;

        /* x0 or y0 is over the edge, nothing to do. */
        if ((x0 > surface->clip.x1) || (y0 > surface->clip.y1) || (y0 < surface->clip.y0))  {
            return;
        }

        /* x0 is left of clip window, ignore start part. */
        if (x0 < surface->clip.x0) {
            width = width + x0;
            x0 = surface->clip.x0;
        }

        /* Everything outside clip window, nothing to do. */
        if (width <= 0)  {
            return;
        }

        /* Cut anything going over right edge of clip window. */
        if (((x0 + width) > surface->clip.x1)) {
            width = width - (x0 + width - 1 - surface->clip.x1);
        }

        surface->hline(&surface, x0, y0, width, color);
    } else {
        hagl_draw_line(surface, x0, y0, x0 + w - 1, y0, color);
    }
}
