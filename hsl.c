/*

Copyright (c) 2004-2007, Lode Vandevenne

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

-cut-

This file is part of the HAGL graphics library:
https://github.com/tuupola/hagl


SPDX-License-Identifier: BSD-2-Clause

*/

#include <stdint.h>

#include "hsl.h"

rgb_t hsl_to_rgb888(hsl_t *hsl)
{
    rgb_t rgb;
    float r, g, b, h, s, l;
    float temp1, temp2, tempr, tempg, tempb;

    h = hsl->h / 256.0;
    s = hsl->s / 256.0;
    l = hsl->l / 256.0;

    /* Saturation 0 means shade of grey. */
    if(s == 0) {
        r = g = b = l;
    } else {
        if (l < 0.5) {
            temp2 = l * (1 + s);
        } else {
            temp2 = (l + s) - (l * s);
        }
        temp1 = 2 * l - temp2;
        tempr = h + 1.0 / 3.0;
        if (tempr > 1) {
            tempr--;
        }
        tempg = h;
        tempb = h - 1.0 / 3.0;
        if (tempb < 0) {
            tempb++;
        }

        /* Red */
        if (tempr < 1.0 / 6.0) {
            r = temp1 + (temp2 - temp1) * 6.0 * tempr;
        } else if (tempr < 0.5) {
            r = temp2;
        } else if (tempr < 2.0 / 3.0) {
            r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
        } else { r = temp1; }

        /* Green */
        if (tempg < 1.0 / 6.0) {
             g = temp1 + (temp2 - temp1) * 6.0 * tempg;
        } else if (tempg < 0.5) {
            g = temp2;
        } else if (tempg < 2.0 / 3.0) {
            g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
        } else {
            g = temp1;
        }

        /* Blue */
        if (tempb < 1.0 / 6.0) {
            b = temp1 + (temp2 - temp1) * 6.0 * tempb;
        } else if (tempb < 0.5) {
            b = temp2;
        } else if (tempb < 2.0 / 3.0) {
            b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
        } else {
            b = temp1;
        }
    }

    rgb.r = (uint8_t)(r * 255.0);
    rgb.g = (uint8_t)(g * 255.0);
    rgb.b = (uint8_t)(b * 255.0);

    return rgb;
}
