/*

MIT License

Copyright (c) 2026 Mika Tuupola

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
#include <string.h>

#include "greatest.h"

#include "font5x7.h"
#include "font5x8.h"
#include "font6x9.h"
#include "fontx.h"

TEST test_meta_dimensions(void) {
    fontx_meta_t meta;
    uint8_t status;

    status = fontx_meta(&meta, font6x9);
    ASSERT_EQ(0, status);
    ASSERT_EQ(6, meta.width);
    ASSERT_EQ(9, meta.height);
    ASSERT_EQ(FONTX_TYPE_DBCS, meta.type);

    status = fontx_meta(&meta, font5x7);
    ASSERT_EQ(0, status);
    ASSERT_EQ(5, meta.width);
    ASSERT_EQ(7, meta.height);
    ASSERT_EQ(FONTX_TYPE_DBCS, meta.type);

    status = fontx_meta(&meta, font5x8);
    ASSERT_EQ(0, status);
    ASSERT_EQ(5, meta.width);
    ASSERT_EQ(8, meta.height);
    ASSERT_EQ(FONTX_TYPE_DBCS, meta.type);

    PASS();
}

TEST test_meta_name(void) {
    fontx_meta_t meta;

    fontx_meta(&meta, font6x9);
    ASSERT_EQ(0, memcmp(meta.name, "MISC    ", 8));

    fontx_meta(&meta, font5x7);
    ASSERT_EQ(0, memcmp(meta.name, "MISC    ", 8));

    fontx_meta(&meta, font5x8);
    ASSERT_EQ(0, memcmp(meta.name, "MISC    ", 8));

    PASS();
}

/*
 * fontx_glyph() tests for DBCS fonts.
 *
 * font6x9 first two blocks:
 *   Block 0: 0x0020 - 0x007E (95 chars)
 *   Block 1: 0x00A0 - 0x017F (224 chars)
 *   Gap at 0x007F - 0x009F
 *
 * font5x7 first two blocks:
 *   Block 0: 0x0020 - 0x007E (95 chars)
 *   Block 1: 0x00A0 - 0x021F (384 chars)
 *
 * font5x8 first two blocks:
 *   Block 0: 0x0020 - 0x007E (95 chars)
 *   Block 1: 0x00A0 - 0x017F (224 chars)
 *   Gap at 0x007F - 0x009F
 */

TEST test_dbcs_glyph_dimensions(void) {
    fontx_glyph_t glyph;
    uint8_t status;

    status = fontx_glyph(&glyph, 0x41, font6x9);
    ASSERT_EQ(FONTX_OK, status);
    ASSERT_EQ(6, glyph.width);
    ASSERT_EQ(9, glyph.height);
    ASSERT_EQ(1, glyph.pitch);
    ASSERT_EQ(9, glyph.size);

    status = fontx_glyph(&glyph, 0x41, font5x7);
    ASSERT_EQ(FONTX_OK, status);
    ASSERT_EQ(5, glyph.width);
    ASSERT_EQ(7, glyph.height);
    ASSERT_EQ(1, glyph.pitch);
    ASSERT_EQ(7, glyph.size);

    status = fontx_glyph(&glyph, 0x41, font5x8);
    ASSERT_EQ(FONTX_OK, status);
    ASSERT_EQ(5, glyph.width);
    ASSERT_EQ(8, glyph.height);
    ASSERT_EQ(1, glyph.pitch);
    ASSERT_EQ(8, glyph.size);

    PASS();
}

/* First character in the first block ie. space. */
TEST test_dbcs_glyph_first_in_block(void) {
    fontx_glyph_t glyph;
    uint8_t status;
    const uint8_t expected[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    status = fontx_glyph(&glyph, 0x20, font6x9);

    ASSERT_EQ(FONTX_OK, status);
    ASSERT_EQ(0, memcmp(glyph.buffer, expected, glyph.size));

    PASS();
}

/* Last character in the first block (tilde). */
TEST test_dbcs_glyph_last_in_block(void) {
    fontx_glyph_t glyph;
    uint8_t status;
    const uint8_t expected[] = {0x00, 0x00, 0x28, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00};

    status = fontx_glyph(&glyph, 0x7e, font6x9);

    ASSERT_EQ(FONTX_OK, status);
    ASSERT_EQ(0, memcmp(glyph.buffer, expected, glyph.size));

    PASS();
}

/* First character of the second block, verifies block table accumulation. */
TEST test_dbcs_glyph_second_block(void) {
    fontx_glyph_t glyph;
    uint8_t status;

    status = fontx_glyph(&glyph, 0xa0, font6x9);

    ASSERT_EQ(FONTX_OK, status);
    ASSERT_EQ(6, glyph.width);
    ASSERT_EQ(9, glyph.height);

    PASS();
}

/* Verify glyph bitmap content for "A" in both fonts. */
TEST test_dbcs_glyph_content(void) {
    fontx_glyph_t glyph;
    uint8_t status;
    const uint8_t expected_6x9[] = {0x00, 0x20, 0x50, 0x88, 0xf8, 0x88, 0x88, 0x00, 0x00};
    const uint8_t expected_5x7[] = {0x60, 0x90, 0x90, 0xf0, 0x90, 0x90, 0x00};
    const uint8_t expected_5x8[] = {0x00, 0x60, 0x90, 0x90, 0xf0, 0x90, 0x90, 0x00};

    status = fontx_glyph(&glyph, 0x41, font6x9);
    ASSERT_EQ(FONTX_OK, status);
    ASSERT_EQ(0, memcmp(glyph.buffer, expected_6x9, glyph.size));

    status = fontx_glyph(&glyph, 0x41, font5x7);
    ASSERT_EQ(FONTX_OK, status);
    ASSERT_EQ(0, memcmp(glyph.buffer, expected_5x7, glyph.size));

    status = fontx_glyph(&glyph, 0x41, font5x8);
    ASSERT_EQ(FONTX_OK, status);
    ASSERT_EQ(0, memcmp(glyph.buffer, expected_5x8, glyph.size));

    PASS();
}

/* Code 0x7F falls in the gap between block 0 and block 1. */
TEST test_dbcs_glyph_gap_not_found(void) {
    fontx_glyph_t glyph;
    uint8_t status;

    status = fontx_glyph(&glyph, 0x7f, font6x9);

    ASSERT_EQ(FONTX_ERR_GLYPH_NOT_FOUND, status);

    PASS();
}

/* Code 0x00 is before the first block which starts at 0x20. */
TEST test_dbcs_glyph_before_first_block(void) {
    fontx_glyph_t glyph;
    uint8_t status;

    status = fontx_glyph(&glyph, 0x00, font6x9);

    ASSERT_EQ(FONTX_ERR_GLYPH_NOT_FOUND, status);

    PASS();
}

/* Glyph buffer should point directly into the font array ie. zero copy. */
TEST test_dbcs_glyph_buffer_not_copied(void) {
    fontx_glyph_t glyph;
    uint8_t status;
    const uint8_t *font_start;
    const uint8_t *font_end;

    font_start = font6x9;
    font_end = font6x9 + sizeof(font6x9);
    status = fontx_glyph(&glyph, 0x41, font6x9);
    ASSERT_EQ(FONTX_OK, status);
    ASSERT(glyph.buffer >= font_start);
    ASSERT(glyph.buffer + glyph.size <= font_end);

    font_start = font5x7;
    font_end = font5x7 + sizeof(font5x7);
    status = fontx_glyph(&glyph, 0x41, font5x7);
    ASSERT_EQ(FONTX_OK, status);
    ASSERT(glyph.buffer >= font_start);
    ASSERT(glyph.buffer + glyph.size <= font_end);

    font_start = font5x8;
    font_end = font5x8 + sizeof(font5x8);
    status = fontx_glyph(&glyph, 0x41, font5x8);
    ASSERT_EQ(FONTX_OK, status);
    ASSERT(glyph.buffer >= font_start);
    ASSERT(glyph.buffer + glyph.size <= font_end);

    PASS();
}

SUITE(fontx_suite) {
    RUN_TEST(test_meta_dimensions);
    RUN_TEST(test_meta_name);
    RUN_TEST(test_dbcs_glyph_dimensions);
    RUN_TEST(test_dbcs_glyph_first_in_block);
    RUN_TEST(test_dbcs_glyph_last_in_block);
    RUN_TEST(test_dbcs_glyph_second_block);
    RUN_TEST(test_dbcs_glyph_content);
    RUN_TEST(test_dbcs_glyph_gap_not_found);
    RUN_TEST(test_dbcs_glyph_before_first_block);
    RUN_TEST(test_dbcs_glyph_buffer_not_copied);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(fontx_suite);
    GREATEST_MAIN_END();
}
