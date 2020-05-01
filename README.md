# Copepod

Copepod is a lightweight hardware agnostics graphics library. It supports basic geometric primitives, bitmaps, blitting, fixed width fonts and an optional framebuffer. Everything is still work in progress. Things will change without warning.

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE.md)

## Hardware Abstraction Layer

To use Copepod you must provide a hardware absraction layer ie. HAL. It must provide atleast the `pod_hal_put_pixel(x0, y0, color)` function. If nothing else is provided all higher level graphical functions will use this function to draw the primitive. There are other optional HAL functions. While proper documentation is lacking see the example HAL implementations for [libgd](https://github.com/tuupola/copepod-libgd), [libsdl2](https://github.com/tuupola/copepod-libsdl2) and display drivers for [ESP-IDF supporting MIPI Display Command Set](https://github.com/tuupola/esp_mipi).


## Usage

Guess what! Better docs to be written. High level functions are pretty self explanatory though. For example applications see [ESP GFX](https://github.com/tuupola/esp_gfx), [ESP Fire](https://github.com/tuupola/esp_fire), [M5Stack Mandelbrot](https://github.com/tuupola/esp-examples/tree/master/014-mandelbrot) and [ESP M5StickC](https://github.com/tuupola/esp_m5stick).

```c
void pod_put_pixel(int16_t x0, int16_t y0, uint16_t color);
void pod_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pod_draw_hline(int16_t x0, int16_t y0, uint16_t width, uint16_t color);
void pod_draw_vline(int16_t x0, int16_t y0, uint16_t height, uint16_t color);
void pod_draw_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pod_fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pod_draw_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);
void pod_fill_rounded_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);
void pod_draw_circle(int16_t xc, int16_t yc, int16_t r, uint16_t color);
void pod_fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void pod_draw_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, uint16_t color);
void pod_fill_ellipse(int16_t x0, int16_t y0, int16_t a, int16_t b, uint16_t color);
void pod_draw_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void pod_fill_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void pod_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void pod_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);


uint8_t pod_put_char(char16_t code, int16_t x0, int16_t y0, uint16_t color, const unsigned char *font);
uint16_t pod_put_text(const char16_t *str, int16_t x0, int16_t y0, uint16_t color, const unsigned char *font);

void pod_clear_clip_window();
void pod_clear_screen();
void pod_set_clip_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

```

Blit copies a [bitmap](https://github.com/tuupola/copepod/blob/master/bitmap.c) to the screen. You can also copy a bitmap scaled up or down.

```c
void pod_blit(int16_t x0, int16_t y0, bitmap_t *source);
void pod_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source);
```

Depending on HAL, initialisation and flushing might be required. If HAL does not require them both functions are just a no-op.

```c
bitmap_t *pod_init();
void pod_flush();
void pod_close();
```

## Speed

 First table numbers are operations per second with double buffering enabled. Bigger number is better. T-Display and M5StickC have higher numbers because they have smaller resolution. Smaller resolution means less bytes to push to the display.

|                              | T4    | T-Display | M5Stack | M5StickC |
|------------------------------|-------|-----------|---------|----------|
| pod_put_pixel()              | 77695 | 79762     | 80017   | 84496    |
| pod_draw_line()              | 897   | 1308      | 932     | 2107     |
| pod_draw_circle()            | 1170  | 725       | 679     | 891      |
| pod_fill_circle()            | 1870  | 2080      | 2064    | 2325     |
| pod_draw_ellipse()           | 668   |           |         |          |
| pod_fill_ellipse()           | 875   |           |         |          |
| pod_draw_triangle()          | 297   | 438       | 306     | 707      |
| pod_fill_triangle()          | 472   | 683       | 600     | 1104     |
| pod_draw_rectangle()         | 12313 | 14625     | 12942   | 17703    |
| pod_fill_rectangle()         | 751   | 1193      | 913     | 2031     |
| pod_draw_rounded_rectangle() | 3220  |           |         |          |
| pod_fill_rounded_rectangle() | 701   |           |         |          |
| pod_draw_polygon()           | 181   | 265       | 188     | 425      |
| pod_fill_polygon()           | 274   | 397       | 349     | 633      |
| pod_put_char)                | 16790 | 14302     | 16192   | 11656    |
| pod_flush()                  | 29    | 68        | 29      | 86       |

Second table numbers are operations per second with double buffering disabled.

|                              | T4    | T-Display | M5Stack | M5StickC |
|------------------------------|-------|-----------|---------|----------|
| pod_put_pixel()              | 12610 | 13341     | 13361   | 12324    |
| pod_draw_line()              | 95    | 134       | 93      | 203      |
| pod_draw_circle()            | 123   | 76        | 66      | 79       |
| pod_fill_circle()            | 207   | 228       | 223     | 197      |
| pod_draw_ellipse()           | 70    |           |         |          |
| pod_fill_ellipse()           | 96    |           |         |          |
| pod_draw_triangle()          | 31    | 46        | 31      | 67       |
| pod_fill_triangle()          | 61    | 94        | 79      | 141      |
| pod_draw_rectangle()         | 1997  | 2336      | 2076    | 2092     |
| pod_fill_rectangle()         | 75    | 127       | 99      | 171      |
| pod_draw_rounded_rectangle() | 380   |           |         |          |
| pod_fill_rounded_rectangle() | 78    |           |         |          |
| pod_draw_polygon()           | 19    | 27        | 19      | 42       |
| pod_fill_polygon()           | 36    | 57        | 48      | 85       |
| pod_put_char)                | 3093  | 2316      | 3028    | 1585     |
| pod_flush()                  | x     | x         | x       | x        |

You can run the speed tests yourself by checking out the [speedtest repository](https://github.com/tuupola/esp_gfx).

## License

The MIT License (MIT). Please see [License File](LICENSE.md) for more information.
