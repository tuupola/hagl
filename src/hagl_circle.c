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

#include <stdint.h>

#include "hagl/color.h"
#include "hagl/pixel.h"
#include "hagl/hline.h"

void
draw_circle_helper_Pixel( int xc, int yc, int x, int y, hagl_color_t color ){
    hagl_put_pixel(surface, xc+x, yc-y, color ); // UR
    hagl_put_pixel(surface, xc+x, yc+y, color ); // DR
    hagl_put_pixel(surface, xc-x, yc+y, color ); // DL
    hagl_put_pixel(surface, xc-x, yc-y, color ); // UL
    hagl_put_pixel(surface, xc+y, yc-x, color ); // RU
    hagl_put_pixel(surface, xc-y, yc-x, color ); // LU
    hagl_put_pixel(surface, xc-y, yc+x, color ); // LD
    hagl_put_pixel(surface, xc+y, yc+x, color ); // RD
}

void
draw_circle_helper_Line( int xc, int yc, int x, int y, int len, hagl_color_t color ){
    if(len==1){
        draw_circle_helper_Pixel(xc,yc,x-1,y,color);
        return;
    }
    hagl_draw_hline(surface, xc+x-len, yc-y, len, color ); // UR
    hagl_draw_hline(surface, xc+x-len, yc+y, len, color ); // DR
    hagl_draw_hline(surface, xc-x+1, yc+y, len, color );   // DL
    hagl_draw_hline(surface, xc-x+1, yc-y, len, color );   // UL
    hagl_draw_vline(surface, xc+y, yc-x+1, len, color );   // RU
    hagl_draw_vline(surface, xc-y, yc-x+1, len, color );   // LU
    hagl_draw_vline(surface, xc-y, yc+x-len, len, color ); // LD
    hagl_draw_vline(surface, xc+y, yc+x-len, len, color ); // RD
}

void
draw_circle_helper_URDL( int xc, int yc, int x, int y, int len, hagl_color_t color ){
    if(len==1){
        draw_circle_helper_Pixel(xc,yc,x-1,y,color);
        return;
    }
    hagl_draw_hline(surface, xc-x+1, yc-y, len*2-1, color ); // ULR
    hagl_draw_hline(surface, xc-x+1, yc+y, len*2-1, color ); // DLR
    hagl_draw_vline(surface, xc+y, yc-x+1, len*2-1, color ); // RUD
    hagl_draw_vline(surface, xc-y, yc-x+1, len*2-1, color ); // LUD
}

void
hagl_draw_circle(void const *surface, int16_t xc, int16_t yc, int16_t r, hagl_color_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;
    int16_t dx = 0;
    int16_t edgesDrawed = 0;
    while( y>=x ){
        if( d<=0 ){
            d = d + 4 * x + 6;
            x++;
            dx++;
        } else {
            if(edgesDrawed){
                draw_circle_helper_Line( xc, yc, x+1, y, dx+1, color );
            } else {
                draw_circle_helper_URDL( xc, yc, x+1, y, dx+1, color );
                edgesDrawed = 1;
            }
            dx = 0;
            d = d + 4 * (x - y) + 10;
            x++;
            y--;
        }
    }
}

void
hagl_fill_circle(void const *surface, int16_t xc, int16_t yc, int16_t r, hagl_color_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x) {
        hagl_draw_hline(surface, xc - x, yc + y, x * 2 + 1, color);
        hagl_draw_hline(surface, xc - x, yc - y, x * 2 + 1, color);
        hagl_draw_hline(surface, xc - y, yc + x, y * 2 + 1, color);
        hagl_draw_hline(surface, xc - y, yc - x, y * 2 + 1, color);

        if( d<=0 ){
            d = d + 4 * x + 6;
            x++;
        } else {
            d = d + 4 * (x - y) + 10;
            x++;
            y--;
        }
    }
}
