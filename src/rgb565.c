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

#include <stdint.h>

#include "rgb565.h"

uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t rgb;

    rgb = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
    rgb = (((rgb) << 8) & 0xFF00) | (((rgb) >> 8) & 0xFF);

    return rgb;
}

rgb_t rgb565_to_rgb888(uint16_t *input)
{
    rgb_t rgb;

    uint8_t r5 = (*input & 0xf800) >> 8; // 1111100000000000
    uint8_t g6 = (*input & 0x07e0) >> 3; // 0000011111100000
    uint8_t b5 = (*input & 0x001f) << 3; // 0000000000011111

    rgb.r = (r5 * 527 + 23) >> 6;
    rgb.g = (g6 * 259 + 33) >> 6;
    rgb.b = (b5 * 527 + 23) >> 6;

    return rgb;
}