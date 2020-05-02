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

#include "rgb888.h"

hsl_t rgb888_to_hsl(rgb_t *rgb)
{
    hsl_t hsl;
    float r, g, b, h, s, l;
    r = rgb->r / 256.0;
    g = rgb->g / 256.0;
    b = rgb->b / 256.0;

    float maxColor = max(r, max(g, b));
    float minColor = min(r, min(g, b));

    /* R == G == B, so it's a shade of gray. */
    if (minColor == maxColor) {
        h = 0.0;
        s = 0.0;
        l = r;
    } else {
        l = (minColor + maxColor) / 2;

        if (l < 0.5) {
            s = (maxColor - minColor) / (maxColor + minColor);
        } else {
            s = (maxColor - minColor) / (2.0 - maxColor - minColor);
        }

        if (r == maxColor) {
            h = (g - b) / (maxColor - minColor);
        } else if (g == maxColor) {
            h = 2.0 + (b - r) / (maxColor - minColor);
        } else {
            h = 4.0 + (r - g) / (maxColor - minColor);
        }

        h /= 6; /* To bring it to a number between 0 and 1. */
        if (h < 0) {
            h++;
        }

    }

    hsl.h = (uint8_t)(h * 255.0);
    hsl.s = (uint8_t)(s * 255.0);
    hsl.l = (uint8_t)(l * 255.0);

    return hsl;
}

uint16_t rgb888_to_rgb565(rgb_t *input) {
    uint16_t r5 = (input->r * 249 + 1014 ) >> 11;
    uint16_t g6 = (input->g * 253 +  505) >> 10;
    uint16_t b5 = (input->b * 249 + 1014) >> 11;

    return (r5 | g6 | b5);
}