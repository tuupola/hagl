/*

MIT License

Copyright (c) 2012 ChaN
Copyright (c) 2020-2022 Mika Tuupola

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

Adapted from the blog post by ChaN:
http://elm-chan.org/docs/dosv/fontx_e.html

SPDX-License-Identifier: MIT

*/

#ifndef _HAGL_FONTX_H
#define _HAGL_FONTX_H

#ifdef __cplusplus
extern "C" {
#endif

#define FONTX_OK                   (0)
#define FONTX_ERR_GLYPH_NOT_FOUND  (1)
#define FONTX_NAME                 (6)
#define FONTX_WIDTH               (14)
#define FONTX_HEIGHT              (15)
#define FONTX_TYPE                (16)
#define FONTX_TYPE_SBCS            (0)
#define FONTX_TYPE_DBCS            (1)
#define FONTX_GLYPH_DATA_START    (17)
#define FONTX_BLOCK_TABLE_SIZE    (17)
#define FONTX_BLOCK_TABLE_START   (18)

#include <stdint.h>
#include <stddef.h>

/* Pitch is bytes per row. Size is size in bytes. */
typedef struct{
    uint8_t width;
    uint8_t height;
    uint8_t size;
    uint8_t pitch;
    const uint8_t *buffer;
} fontx_glyph_t;

typedef struct{
    char name[9];
    uint8_t width;
    uint8_t height;
    uint8_t type;
} fontx_meta_t;

uint8_t fontx_meta(fontx_meta_t *meta, const uint8_t *font);
uint8_t fontx_glyph(fontx_glyph_t *glyph, wchar_t code, const uint8_t *font);

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_FONTX_H */
