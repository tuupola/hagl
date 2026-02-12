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

#include "greatest.h"
#include "crc32.h"
#include "hagl.h"

static hagl_backend_t backend;

static uint32_t
count_pixels(hagl_backend_t *backend, hagl_color_t color)
{
    uint32_t count = 0;
    for (int16_t y = 0; y < backend->height; y++) {
        for (int16_t x = 0; x < backend->width; x++) {
            if (hagl_get_pixel(backend, x, y) == color) {
                count++;
            }
        }
    }
    return count;
}

static void setup_callback(void *data) {
    memset(&backend, 0, sizeof(hagl_backend_t));
    hagl_hal_init(&backend);
    hagl_set_clip(&backend, 0, 0,  backend.width - 1,  backend.height - 1);
    hagl_clear(&backend);
}

/*
 * Axis-aligned square:
 *
 * (10,10)-----(20,10)
 *   |###########|
 *   |###########|
 * (10,20)-----(20,20)
 */
TEST test_fill_polygon_square(void) {
    int16_t vertices[] = {10, 10, 20, 10, 20, 20, 10, 20};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    /* Inside: center of the square */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 15));

    /* Inside: all four corners */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 20));

    /* Inside: edge midpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 15));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 15));

    /* Outside: 1 pixel beyond each edge */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 21, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 21));

    /* Outside: 1 pixel diagonally beyond each corner */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 21, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 21, 21));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 21));

    /* Total filled area: 11 x 11 = 121 pixels */
    ASSERT_EQ(121, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST test_fill_polygon_square_regression(void) {
    int16_t vertices[] = {10, 10, 20, 10, 20, 20, 10, 20};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x52CF24BD, crc);
    PASS();
}

TEST test_fill_polygon_square_match_rectangle(void) {
    /* Draw a filled square polygon. */
    int16_t vertices[] = {10, 10, 20, 10, 20, 20, 10, 20};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc_polygon = crc32(backend.buffer, size);

    /* Clear and draw the same area with fill rectangle. */
    memset(backend.buffer, 0, size);
    hagl_fill_rectangle_xyxy(&backend, 10, 10, 20, 20, 0xFFFF);

    uint32_t crc_rectangle = crc32(backend.buffer, size);

    ASSERT_EQ(crc_rectangle, crc_polygon);
    PASS();
}

TEST test_fill_polygon_square_winding_order(void) {
    /* Clockwise */
    int16_t cw[] = {10, 10, 20, 10, 20, 20, 10, 20};
    hagl_fill_polygon(&backend, 4, cw, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc_cw = crc32(backend.buffer, size);

    /* Clear and draw counterclockwise */
    memset(backend.buffer, 0, size);
    int16_t ccw[] = {10, 10, 10, 20, 20, 20, 20, 10};
    hagl_fill_polygon(&backend, 4, ccw, 0xFFFF);

    uint32_t crc_ccw = crc32(backend.buffer, size);

    ASSERT_EQ(crc_cw, crc_ccw);
    PASS();
}

/*
 * Right triangle:
 *
 * (10,10)-----(30,10)
 *   |########/
 *   |######/
 *   |####/
 *   |##/
 * (10,30)
 */
TEST test_fill_polygon_triangle(void) {
    int16_t vertices[] = {10, 10, 30, 10, 10, 30};
    hagl_fill_polygon(&backend, 3, vertices, 0xFFFF);

    /* Inside: points well within the triangle */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 15));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 12, 20));

    /* Inside: all three vertices */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 30, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 30));

    /* Inside: edge midpoints */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 20));

    /* Outside: beyond the hypotenuse */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 25, 25));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 30, 30));

    /* Outside: beyond the axis-aligned edges */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 20));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 20, 9));

    PASS();
}

TEST test_fill_polygon_triangle_regression(void) {
    int16_t vertices[] = {10, 10, 30, 10, 10, 30};
    hagl_fill_polygon(&backend, 3, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x61E1F4F0, crc);
    PASS();
}

TEST test_fill_polygon_triangle_match_fill_triangle(void) {
    int16_t vertices[] = {10, 10, 30, 10, 10, 30};
    hagl_fill_polygon(&backend, 3, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc_polygon = crc32(backend.buffer, size);

    memset(backend.buffer, 0, size);
    hagl_fill_triangle(&backend, 10, 10, 30, 10, 10, 30, 0xFFFF);

    uint32_t crc_triangle = crc32(backend.buffer, size);

    ASSERT_EQ(crc_triangle, crc_polygon);
    PASS();
}

TEST test_fill_polygon_triangle_winding_order(void) {
    /* Clockwise */
    int16_t cw[] = {10, 10, 30, 10, 10, 30};
    hagl_fill_polygon(&backend, 3, cw, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc_cw = crc32(backend.buffer, size);

    /* Clear and draw counterclockwise */
    memset(backend.buffer, 0, size);
    int16_t ccw[] = {10, 10, 10, 30, 30, 10};
    hagl_fill_polygon(&backend, 3, ccw, 0xFFFF);

    uint32_t crc_ccw = crc32(backend.buffer, size);

    ASSERT_EQ(crc_cw, crc_ccw);
    PASS();
}

/*
 * Concave L-shape polygon:
 *
 * (10,10)-----(30,10)
 *   |#############|
 *   |##(20,20)--(30,20)
 *   |####|
 *   |####|
 * (10,40)-(20,40)
 */
TEST test_fill_polygon_concave(void) {
    int16_t vertices[] = {10, 10, 30, 10, 30, 20, 20, 20, 20, 40, 10, 40};
    hagl_fill_polygon(&backend, 6, vertices, 0xFFFF);

    /* Inside: top arm */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 25, 15));

    /* Inside: bottom arm */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 30));

    /* Inside: all six vertices */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 30, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 30, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 40));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 40));

    /* Inside: where both arms meet */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 20));

    /* Outside: inside bounding box but outside the L */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 25, 25));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 25, 30));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 25, 39));

    /* Outside: beyond edges */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 25));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 21, 30));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 15, 41));

    /* Total filled area: top 21x11 + bottom 11x20 = 451 pixels */
    ASSERT_EQ(451, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST test_fill_polygon_concave_regression(void) {
    int16_t vertices[] = {10, 10, 30, 10, 30, 20, 20, 20, 20, 40, 10, 40};
    hagl_fill_polygon(&backend, 6, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x080BDE15, crc);
    PASS();
}

/*
 * Self-intersecting bowtie shape:
 *
 * (10,10)-------(30,10)
 *     \#########/
 *       \#####/
 *         \#/
 *          X
 *         /#\
 *       /#####\
 *     /#########\
 * (10,30)-------(30,30)
 */
TEST test_fill_polygon_bowtie(void) {
    int16_t vertices[] = {10, 10, 30, 10, 10, 30, 30, 30};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    /* Inside: top triangle */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 12));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 15));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 11));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 25, 11));

    /* Inside: bottom triangle */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 28));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 25));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 29));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 25, 29));

    /* Outside: left and right of center */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 11, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 29, 15));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 11, 25));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 29, 25));

    /* Outside: beyond bounding box */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 20));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 31, 20));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 20, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 20, 31));

    PASS();
}

TEST test_fill_polygon_bowtie_regression(void) {
    int16_t vertices[] = {10, 10, 30, 10, 10, 30, 30, 30};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x803DF4F1, crc);
    PASS();
}

/*
 * Square clipped by top-left corner of display:
 *
 *            .(0,0)
 *             |
 * (-10,-10)-------(10,-10)
 *    |        |######|
 *    |        |######|
 * (-10,10)----+---(10,10)
 *
 * Only the (0,0) to (10,10) portion is visible.
 */
TEST test_fill_polygon_clip_top_left(void) {
    int16_t vertices[] = {-10, -10, 10, -10, 10, 10, -10, 10};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    /* Inside: visible portion */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 5, 5));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 0, 0));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 0));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 0, 10));

    /* Outside: beyond the polygon inside display */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 11, 5));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 5, 11));

    /* Total filled area: 11 x 11 = 121 pixels */
    ASSERT_EQ(121, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST test_fill_polygon_clip_top_left_regression(void) {
    int16_t vertices[] = {-10, -10, 10, -10, 10, 10, -10, 10};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x22DBDA01, crc);
    PASS();
}

/*
 * Square clipped by bottom-right corner of display (320x240):
 *
 * (310,230)-------(330,230)
 *    |######|      |
 *    |######|      |
 * (310,250)-+---(330,250)
 *           |
 *        .(319,239)
 *
 * Only the (310,230) to (319,239) portion is visible.
 */
TEST test_fill_polygon_clip_bottom_right(void) {
    int16_t vertices[] = {310, 230, 330, 230, 330, 250, 310, 250};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    /* Inside: visible portion */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 315, 235));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 310, 230));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 319, 239));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 319, 230));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 310, 239));

    /* Outside: beyond the polygon inside display */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 309, 235));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 315, 229));

    /* Total filled area: 10 x 10 = 100 pixels */
    ASSERT_EQ(100, count_pixels(&backend, 0xFFFF));

    PASS();
}

TEST test_fill_polygon_clip_bottom_right_regression(void) {
    int16_t vertices[] = {310, 230, 330, 230, 330, 250, 310, 250};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0xB46E0159, crc);
    PASS();
}

/*
 * Square entirely outside display:
 *
 * (-30,-30)-----(-10,-30)
 *    |              |
 *    |              |
 * (-30,-10)-----(-10,-10)
 *
 *                     .(0,0) display starts here
 */
TEST test_fill_polygon_clip_outside(void) {
    int16_t vertices[] = {-30, -30, -10, -30, -10, -10, -30, -10};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    ASSERT_EQ(0, count_pixels(&backend, 0xFFFF));
    PASS();
}

/*
 * Trapezoid with horizontal edges (exercises y0 == y1 code path):
 *
 *     (15,10)---(25,10)
 *    /################\
 *   /##################\
 * (10,30)---------(30,30)
 */
TEST test_fill_polygon_trapezoid(void) {
    int16_t vertices[] = {15, 10, 25, 10, 30, 30, 10, 30};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    /* Inside: center */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 20));

    /* Inside: vertices */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 15, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 25, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 10, 30));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 30, 30));

    /* Inside: midpoints of horizontal edges */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 10));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 20, 30));

    /* Inside: midpoints of diagonal edges */
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 13, 20));
    ASSERT_EQ(0xFFFF, hagl_get_pixel(&backend, 27, 20));

    /* Outside: above and below */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 20, 9));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 20, 31));

    /* Outside: beyond the narrow top edge */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 14, 10));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 26, 10));

    /* Outside: beyond the diagonal edges */
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 9, 30));
    ASSERT_EQ(0x0000, hagl_get_pixel(&backend, 31, 30));

    PASS();
}

TEST test_fill_polygon_trapezoid_regression(void) {
    int16_t vertices[] = {15, 10, 25, 10, 30, 30, 10, 30};
    hagl_fill_polygon(&backend, 4, vertices, 0xFFFF);

    size_t size = backend.width * backend.height * (backend.depth / 8);
    uint32_t crc = crc32(backend.buffer, size);

    ASSERT_EQ(0x88B26E34, crc);
    PASS();
}

SUITE(polygon_suite) {
    SET_SETUP(setup_callback, NULL);
    RUN_TEST(test_fill_polygon_square);
    RUN_TEST(test_fill_polygon_square_regression);
    RUN_TEST(test_fill_polygon_square_match_rectangle);
    RUN_TEST(test_fill_polygon_square_winding_order);
    RUN_TEST(test_fill_polygon_triangle);
    RUN_TEST(test_fill_polygon_triangle_regression);
    RUN_TEST(test_fill_polygon_triangle_match_fill_triangle);
    RUN_TEST(test_fill_polygon_triangle_winding_order);
    RUN_TEST(test_fill_polygon_concave);
    RUN_TEST(test_fill_polygon_concave_regression);
    RUN_TEST(test_fill_polygon_bowtie);
    RUN_TEST(test_fill_polygon_bowtie_regression);
    RUN_TEST(test_fill_polygon_clip_top_left);
    RUN_TEST(test_fill_polygon_clip_top_left_regression);
    RUN_TEST(test_fill_polygon_clip_bottom_right);
    RUN_TEST(test_fill_polygon_clip_bottom_right_regression);
    RUN_TEST(test_fill_polygon_clip_outside);
    RUN_TEST(test_fill_polygon_trapezoid);
    RUN_TEST(test_fill_polygon_trapezoid_regression);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(polygon_suite);
    GREATEST_MAIN_END();
}
