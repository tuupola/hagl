# Hardware Agnostic Graphics Library

HAGL is a lightweight hardware agnostics graphics library. It supports basic geometric primitives, bitmaps, blitting, fixed width fonts and an optional framebuffer. Everything is still work in progress. Things will change without warning.

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE.md)

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

|                              | T4     | T-Display | M5Stack | M5StickC |
|------------------------------|--------|-----------|---------|----------|
| hagl_put_pixel()              | 176091 | 176175    | 176913  | 180684   |
| hagl_draw_line()              | 9286   | 13092     | 9683    | 26672    |
| hagl_draw_circle()            | 13803  | 14351     | 13992   | 16460    |
| hagl_fill_circle()            | 7692   | 8224      | 8009    | 12216    |
| hagl_draw_ellipse()           | 7254   | 7655      | 7310    | 8855     |
| hagl_fill_ellipse()           | 2814   | 3086      | 2900    | 4998     |
| hagl_draw_triangle()          | 3204   | 4589      | 3394    | 9875     |
| hagl_fill_triangle()          | 1126   | 1787      | 1334    | 5447     |
| hagl_draw_rectangle()         | 19638  | 25672     | 20861   | 51851    |
| hagl_fill_rectangle()         | 1940   | 4308      | 2036    | 14182    |
| hagl_draw_rounded_rectangle() | 15344  | 19040     | 16188   | 32773    |
| hagl_fill_rounded_rectangle() | 1834   | 3982      | 1865    | 11647    |
| hagl_draw_polygon()           | 1933   | 2784      | 2015    | 6023     |
| hagl_fill_polygon()           | 623    | 971       | 755     | 2961     |
| hagl_put_char()               | 30473  | 22128     | 29852   | 25658    |
| hagl_flush()                  | 29     | 68        | 29      | 86       |

Second table numbers are operations per second with double buffering disabled.

|                              | T4    | T-Display | M5Stack | M5StickC |
|------------------------------|-------|-----------|---------|----------|
| hagl_put_pixel()              | 12610 | 13341     | 13361   | 12324    |
| hagl_draw_line()              | 95    | 134       | 93      | 203      |
| hagl_draw_circle()            | 123   | 76        | 66      | 79       |
| hagl_fill_circle()            | 207   | 228       | 223     | 197      |
| hagl_draw_ellipse()           | 70    |           |         |          |
| hagl_fill_ellipse()           | 96    |           |         |          |
| hagl_draw_triangle()          | 31    | 46        | 31      | 67       |
| hagl_fill_triangle()          | 61    | 94        | 79      | 141      |
| hagl_draw_rectangle()         | 1997  | 2336      | 2076    | 2092     |
| hagl_fill_rectangle()         | 75    | 127       | 99      | 171      |
| hagl_draw_rounded_rectangle() | 380   |           |         |          |
| hagl_fill_rounded_rectangle() | 78    |           |         |          |
| hagl_draw_polygon()           | 19    | 27        | 19      | 42       |
| hagl_fill_polygon()           | 36    | 57        | 48      | 85       |
| hagl_put_char)                | 3093  | 2316      | 3028    | 1585     |
| hagl_flush()                  | x     | x         | x       | x        |

You can run the speed tests yourself by checking out the [speedtest repository](https://github.com/tuupola/esp_gfx).

## License

The MIT License (MIT). Please see [License File](LICENSE.md) for more information.
