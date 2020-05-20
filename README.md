# Hardware Agnostic Graphics Library

HAGL is a lightweight hardware agnostics graphics library. It supports basic geometric primitives, bitmaps, blitting, fixed width fonts and an optional framebuffer. Everything is still work in progress. Things will change without warning.

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE.md)

![Old school demo effects](https://appelsiini.net/img/2020/esp-effects.jpg)

## Hardware Abstraction Layer

To use HAGL you must provide a hardware absraction layer. HAL must provide atleast the `hagl_hal_put_pixel(x0, y0, color)` function. If nothing else is provided all higher level graphical functions will use this function to draw the primitive. There are other optional HAL functions. While proper documentation is lacking see the example HAL implementations for [libgd](https://github.com/tuupola/hagl_gd), [libsdl2](https://github.com/tuupola/hagl_sdl2) and display drivers for [ESP-IDF supporting MIPI Display Command Set](https://github.com/tuupola/esp_mipi).


## Usage

Guess what! Better docs to be written. High level functions are pretty self explanatory though. For example applications see [ESP GFX](https://github.com/tuupola/esp_gfx), [ESP Fire](https://github.com/tuupola/esp_fire), [M5Stack Mandelbrot](https://github.com/tuupola/esp-examples/tree/master/014-mandelbrot) and [ESP M5StickC](https://github.com/tuupola/esp_m5stick).

```c
void hagl_put_pixel(int16_t x0, int16_t y0, uint16_t color);
void hagl_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void hagl_draw_hline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
void hagl_draw_vline(int16_t x0, int16_t y0, uint16_t height, uint16_t color);
void hagl_draw_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void hagl_fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void hagl_draw_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);
void hagl_fill_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);
void hagl_draw_circle(int16_t xc, int16_t yc, int16_t r, uint16_t color);
void hagl_fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void hagl_draw_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, uint16_t color);
void hagl_fill_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, uint16_t color);
void hagl_draw_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void hagl_fill_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void hagl_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void hagl_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);


uint8_t hagl_put_char(char16_t code, int16_t x0, int16_t y0, uint16_t color, const unsigned char *font);
uint16_t hagl_put_text(const char16_t *str, int16_t x0, int16_t y0, uint16_t color, const unsigned char *font);

void hagl_clear_clip_window();
void hagl_clear_screen();
void hagl_set_clip_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

```

Blit copies a [bitmap](https://github.com/tuupola/hagl/blob/master/bitmap.c) to the screen. You can also copy a bitmap scaled up or down.

```c
void hagl_blit(int16_t x0, int16_t y0, bitmap_t *source);
void hagl_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source);
```

Depending on HAL, initialisation and flushing might be required. If HAL does not require them both functions are just a no-op.

```c
bitmap_t *hagl_init();
void hagl_flush();
void hagl_close();
```

## Speed

 First table numbers are operations per second with double buffering enabled. Bigger number is better. T-Display and M5StickC have higher numbers because they have smaller resolution. Smaller resolution means less bytes to push to the display.

|                               | T4     | T-Display | M5Stack | M5StickC |
|-------------------------------|--------|-----------|---------|----------|
| hagl_put_pixel()              | 304400 |    304585 |  340850 |   317094 |
| hagl_draw_line()              |  10485 |     14942 |   12145 |    31293 |
| hagl_draw_circle()            |  15784 |     16430 |   17730 |    18928 |
| hagl_fill_circle()            |   8712 |      9344 |    9982 |    13910 |
| hagl_draw_ellipse()           |   8187 |      8642 |    9168 |    10019 |
| hagl_fill_ellipse()           |   3132 |      3457 |    3605 |     5590 |
| hagl_draw_triangle()          |   3581 |      5137 |    4160 |    11186 |
| hagl_fill_triangle()          |   1246 |      1993 |    1654 |     6119 |
| hagl_draw_rectangle()         |  22759 |     30174 |   26910 |    64259 |
| hagl_fill_rectangle()         |   2191 |      4849 |    2487 |    16146 |
| hagl_draw_rounded_rectangle() |  17660 |     21993 |   20736 |    39102 |
| hagl_fill_rounded_rectangle() |   2059 |      4446 |    2313 |    13270 |
| hagl_draw_polygon()           |   2155 |      3096 |    2494 |     6763 |
| hagl_fill_polygon()           |    692 |      1081 |     938 |     3295 |
| hagl_put_char()               |  29457 |     29131 |   32429 |    27569 |
| hagl_flush()                  |     32 |        76 |      32 |       96 |

Second table numbers are operations per second with double buffering disabled.

|                               | T4    | T-Display | M5Stack | M5StickC |
|-------------------------------|-------|-----------|---------|----------|
| hagl_put_pixel()              | 16041 |     15252 |   16044 |    24067 |
| hagl_draw_line()              |   113 |       172 |     112 |      289 |
| hagl_draw_circle()            |   148 |       173 |     145 |      230 |
| hagl_fill_circle()            |   264 |       278 |     261 |      341 |
| hagl_draw_ellipse()           |    84 |       103 |      85 |      179 |
| hagl_fill_ellipse()           |   114 |       128 |     116 |      191 |
| hagl_draw_triangle()          |    37 |        54 |      37 |      114 |
| hagl_fill_triangle()          |    72 |       111 |      72 |      371 |
| hagl_draw_rectangle()         |  2378 |      2481 |    2374 |     3482 |
| hagl_fill_rectangle()         |    91 |       146 |      91 |      454 |
| hagl_draw_rounded_rectangle() |   458 |       535 |     459 |      808 |
| hagl_fill_rounded_rectangle() |    87 |       139 |      79 |      400 |
| hagl_draw_polygon()           |    21 |        33 |      19 |       71 |
| hagl_fill_polygon()           |    43 |        66 |      49 |      228 |
| hagl_put_char)                |  4957 |      4264 |    4440 |     2474 |
| hagl_flush()                  |     x |         x |       x |        x |

You can run the speed tests yourself by checking out the [speedtest repository](https://github.com/tuupola/esp_gfx).

## License

The MIT License (MIT). Please see [License File](LICENSE.md) for more information.
