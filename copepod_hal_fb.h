/*

Copyright (c) 2018 Mika Tuupola

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

*/

#include <stdint.h>

#include "framebuffer.h"
#include "ili9341.h"

#define POD_FB  (g_fb) /* Name of the global variable holding framebuffer. */

#define POD_HAS_HAL_BLIT
#define POD_HAS_HAL_HLINE
#define POD_HAS_HAL_VLINE

extern framebuffer_t POD_FB;

void pod_hal_putpixel(uint16_t x1, uint16_t y1, uint16_t color);
void pod_hal_blit(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, framebuffer_t *src);
void pod_hal_hline(uint16_t x1, uint16_t y1, uint16_t w, uint16_t color);
void pod_hal_vline(uint16_t x1, uint16_t y1, uint16_t h, uint16_t color);
