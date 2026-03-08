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

#include <string.h>

#include "crc32.h"
#include "greatest.h"
#include "hagl/bitmap.h"
#include "hagl/blit.h"
#include "hagl/pixel.h"
#include "save_image.h"

#define TEST_WIDTH 320
#define TEST_HEIGHT 240
#define TEST_DEPTH 16

#define SOURCE_WIDTH 4
#define SOURCE_HEIGHT 4

static hagl_bitmap_t bitmap;
static uint8_t buffer[TEST_WIDTH * TEST_HEIGHT * (TEST_DEPTH / 8)];

static hagl_bitmap_t source;
static uint8_t src_buffer[SOURCE_WIDTH * SOURCE_HEIGHT * (TEST_DEPTH / 8)];

static uint32_t count_pixels(hagl_bitmap_t *bitmap, hagl_color_t color) {
    uint32_t count = 0;
    for (int16_t y = 0; y < bitmap->height; y++) {
        for (int16_t x = 0; x < bitmap->width; x++) {
            if (hagl_get_pixel(bitmap, x, y) == color) {
                count++;
            }
        }
    }
    return count;
}

static void setup_callback(void *data) {
    memset(buffer, 0, sizeof(buffer));
    hagl_bitmap_init(&bitmap, TEST_WIDTH, TEST_HEIGHT, TEST_DEPTH, buffer);

    /* Fill source bitmap with all-white pixels. */
    memset(src_buffer, 0xFF, sizeof(src_buffer));
    hagl_bitmap_init(&source, SOURCE_WIDTH, SOURCE_HEIGHT, TEST_DEPTH, src_buffer);
}

static void teardown_callback(void *data) {
    char filename[256];
    snprintf(filename, sizeof(filename), "output/%s.png", greatest_info.name_buf);
    save_image(&bitmap, filename);
}

/*
 * Basic blit of a 4x4 source at (10,10):
 *
 * (10,10)--(13,10)
 *   |          |
 * (10,13)--(13,13)
 */
TEST test_blit_xy(void) {
    hagl_blit_xy(&bitmap, 10, 10, &source);

    /* On target: four corners */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 13, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 10, 13));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 13, 13));

    /* On target: interior pixel */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 11, 11));

    /* Outside: one pixel beyond each edge */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 9, 10));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 14, 10));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 10, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 10, 14));

    /* Total: 4x4 = 16 pixels */
    ASSERT_EQ(16, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Scale blit of a 4x4 source into an 8x8 region at (20,20):
 *
 * (20,20)-------(27,20)
 *   |               |
 * (20,27)-------(27,27)
 */
TEST test_blit_xywh(void) {
    hagl_blit_xywh(&bitmap, 20, 20, 8, 8, &source);

    /* On target: four corners of the scaled region */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 20, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 27, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 20, 27));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 27, 27));

    /* On target: center pixel */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&bitmap, 23, 23));

    /* Outside: one pixel beyond each edge */
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 19, 20));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 28, 20));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 20, 19));
    ASSERT_EQ(0x0000, hagl_get_pixel(&bitmap, 20, 28));

    /* Total: 8x8 = 64 pixels */
    ASSERT_EQ(64, count_pixels(&bitmap, 0xFFFF));

    PASS();
}

/*
 * Verify xyxy produces the same result as xywh:
 *
 * xyxy: hagl_blit_xyxy(&bitmap, 20, 20, 27, 27, &source)
 * xywh: hagl_blit_xywh(&bitmap, 20, 20, 8, 8, &source)
 */
TEST test_blit_xyxy_match_xywh(void) {
    /* Draw with _xyxy. */
    hagl_blit_xyxy(&bitmap, 20, 20, 27, 27, &source);

    uint32_t crc_xyxy = crc32(bitmap.buffer, bitmap.size);

    /* Clear and draw the same area with _xywh. */
    memset(bitmap.buffer, 0, bitmap.size);
    hagl_blit_xywh(&bitmap, 20, 20, 8, 8, &source);

    uint32_t crc_xywh = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(crc_xywh, crc_xyxy);
    PASS();
}

/*
 * Verify xyxy handles reversed endpoints (x0 > x1, y0 > y1):
 *
 * xyxy reversed: hagl_blit_xyxy(&bitmap, 27, 27, 20, 20, &source)
 * xyxy normal:   hagl_blit_xyxy(&bitmap, 20, 20, 27, 27, &source)
 */
TEST test_blit_xyxy_reversed(void) {
    /* Draw with reversed endpoints. */
    hagl_blit_xyxy(&bitmap, 27, 27, 20, 20, &source);

    uint32_t crc_reversed = crc32(bitmap.buffer, bitmap.size);

    /* Clear and draw with normal order. */
    memset(bitmap.buffer, 0, bitmap.size);
    hagl_blit_xyxy(&bitmap, 20, 20, 27, 27, &source);

    uint32_t crc_normal = crc32(bitmap.buffer, bitmap.size);

    ASSERT_EQ(crc_normal, crc_reversed);
    PASS();
}

SUITE(blit_suite) {
    SET_SETUP(setup_callback, NULL);
    SET_TEARDOWN(teardown_callback, NULL);
    RUN_TEST(test_blit_xy);
    RUN_TEST(test_blit_xywh);
    RUN_TEST(test_blit_xyxy_match_xywh);
    RUN_TEST(test_blit_xyxy_reversed);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(blit_suite);
    GREATEST_MAIN_END();
}
