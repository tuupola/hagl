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

#ifndef _ESP_ILI9341_FRAMEBUFFER_H
#define _ESP_ILI9341_FRAMEBUFFER_H

#include <stdint.h>

#include "bitmap.h"

#define FRAMEBUFFER_WIDTH   320
#define FRAMEBUFFER_HEIGHT  240

#define POD_HAS_HAL_BLIT
#define POD_HAS_HAL_SCALE_BLIT
//#define POD_HAS_HAL_HLINE
//#define POD_HAS_HAL_VLINE
#define POD_HAS_HAL_INIT
#define POD_HAS_HAL_FLUSH

void pod_hal_init(void);
void pod_hal_flush(void);
void pod_hal_putpixel(uint16_t x1, uint16_t y1, uint16_t color);
void pod_hal_blit(uint16_t x1, uint16_t y1, bitmap_t *src);
void pod_hal_scale_blit(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, bitmap_t *src);
// void pod_hal_hline(uint16_t x1, uint16_t y1, uint16_t w, uint16_t color);
// void pod_hal_vline(uint16_t x1, uint16_t y1, uint16_t h, uint16_t color);

#endif /* _ESP_ILI9341_FRAMEBUFFER_H */