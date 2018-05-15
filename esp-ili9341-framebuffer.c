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

#include <string.h>
#include <ili9341.h>

#include "blit.h"
#include "copepod.h"
#include "framebuffer.h"
#include "esp-ili9341-framebuffer.h"

framebuffer_t g_fb = {
    .width = FRAMEBUFFER_WIDTH,
    .height =FRAMEBUFFER_HEIGHT,
    .depth = 16,
};

spi_device_handle_t g_spi;

/*
 * Initializes the ILI9341 + framebuffer HAL.
 */
void pod_hal_init(void)
{
    spi_master_init(&g_spi);
    ili9341_init(&g_spi);
    framebuffer_init(&g_fb);
}

/*
 * Flushes the framebuffer contents to the actual display.
 */
void pod_hal_flush(void)
{
    ili9431_bitmap(g_spi, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, g_fb.buffer);
}

/*
 * Putpixel function. This is the only mandatory function which HAL
 * must implement for copepod to be able to draw graphical primitives.
 * This version draws to a framebuffer.
 */
void pod_hal_putpixel(uint16_t x0, uint16_t y0, uint16_t color)
{
    uint16_t *ptr = g_fb.buffer + g_fb.pitch * y0 + (g_fb.depth / 8) * x0;

    if((x0 < g_fb.width) && (y0 < g_fb.height)) {
    	*ptr = color;
    }
}

/*
 * Blit the source bitmap to the framebuffer.
 */
void pod_hal_blit(uint16_t x0, uint16_t y0, bitmap_t *src)
{
    blit(x0, y0, src, &g_fb);
}

/*
 * Blit the source bitmap to the framebuffer scaled up or down.
 * TODO: stretch might be more proper naming?
 */
void pod_hal_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *src)
{
    scale_blit(x0, y0, w, h, src, &g_fb);
}

/*
 * Accelerated horizontal line drawing.
 */
//void pod_hal_hline(uint16_t x1, uint16_t y1, uint16_t width, uint16_t color)
// {
//     uint16_t bitmap[width];

//     for (uint16_t i = 0; i < width; i++) {
//         ((uint16_t *)bitmap)[i] = color;
//     }

//     pod_hal_blit(x1, y1, &bitmap);
// }

/*
 * Accelerated vertical line drawing.
 */
// void pod_hal_vline(uint16_t x1, uint16_t y1, uint16_t height, uint16_t color)
// {
//     {
//     uint16_t *ptr = g_fb.buffer + g_fb.pitch * y0 + (g_fb.depth / 8) * x0;


//     for (uint16_t i = 0; i < height; i++) {
//         ((uint16_t *)bitmap)[i] = color;
//     }

//     pod_hal_blit(x1, y1, &bitmap);
// }
