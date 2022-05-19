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

#ifndef _HAGL_RGB332_H
#define _HAGL_RGB332_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

static inline uint8_t rgb332(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t r3 = ((r >> 4) & 0b00000110) | (r & 0b00000001);
    uint8_t g3 = ((g >> 4) & 0b00000110) | (g & 0b00000001);
    uint8_t b3 = ((b >> 4) & 0b00000110) | (b & 0b00000001);
    return (r3 << 5) | (g3 << 2) | (b3);
}

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_RGB332_H */