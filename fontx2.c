/*

MIT License

Copyright (c) 2012 ChaN
Copyright (c) 2020 Mika Tuupola

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

This file is part of the Copepod graphics library:
https://github.com/tuupola/copepod

Adapted from the blog post by ChaN:
http://elm-chan.org/docs/dosv/fontx_e.html

SPDX-License-Identifier: MIT

*/

#include <stdint.h>
#include <string.h>

#include "fontx2.h"

fontx2_meta_t fontx2_meta(const uint8_t *font) {
    fontx2_meta_t meta;

    memset(meta.name, '\0', sizeof(meta.name));
    memcpy(meta.name, font, 8);
    meta.width = font[FONTX2_WIDTH];
    meta.height = font[FONTX2_HEIGHT];
    meta.type = font[FONTX2_TYPE];

    return meta;
}


fontx2_glyph_t *fontx2_glyph(uint16_t code, const uint8_t* font) {
    uint32_t nc, bc, sb, eb, total;
    uint16_t fsz;
    const uint8_t *block_table;
    static fontx2_glyph_t glyph;

    glyph.width = font[FONTX2_WIDTH];
    glyph.height = font[FONTX2_HEIGHT];
    glyph.pitch = (glyph.width + 7) / 8;
    glyph.size = glyph.pitch * glyph.height;

    if (FONTX2_TYPE_SBCS == font[FONTX2_TYPE]) {
        if (code < 0x100) {
            glyph.buffer = &font[FONTX2_GLYPH_DATA_START + code * glyph.size];
            return &glyph;
        }
    } else {
        block_table = &font[FONTX2_BLOCK_TABLE_START];
        nc = 0;
        bc = font[FONTX2_BLOCK_TABLE_SIZE];
        while (bc--) {
            /* Get range of the code block_table. */
            sb = block_table[0] + block_table[1] * 0x100;
            eb = block_table[2] + block_table[3] * 0x100;

             /* Check if in the code block_table. */
            if (code >= sb && code <= eb) {
                /* Number of codes from top of the block_table. */
                nc += code - sb;
                glyph.buffer = &font[
                    FONTX2_BLOCK_TABLE_START +
                    4 * font[FONTX2_BLOCK_TABLE_SIZE] +
                    nc * glyph.size
                ];
                return &glyph;
            }
            /* Number of codes in the previous block_tables. */
            nc += eb - sb + 1;
            /* Next code block_table. */
            block_table += 4;
        }
    }

    return NULL;
}