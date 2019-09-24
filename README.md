# Copepod

Copepod is a lightweight hardware agnostics graphics library. It supports basic geometric primitives, bitmaps, blitting, fixed width fonts and an optional framebuffer. Everything is still work in progress. Things will change without warning. I'm doing this to learn c.

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE.md)

## Hardware Abstraction Layer

To use Copepod you must provide a hardware absraction layer ie. HAL. It must provide atleast the `pod_hal_putpixel(x0, y0, color)` function. If nothing else is provided all higher level graphical functions will use this function to draw the primitive. There are other optional HAL functions. While proper documentation is lacking see the [ESP ILI9341 HAL](https://github.com/tuupola/copepod-esp-ili9341).

## Usage

Guess what! Better docs to be written. High level functions are pretty self explanatory though.

```c
void pod_put_pixel(uint16_t x0, uint16_t y0, uint16_t color);
void pod_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void pod_draw_hline(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color);
void pod_draw_vline(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color);
void pod_draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void pod_fill_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void pod_draw_circle(int16_t xc, int16_t yc, int16_t r, uint16_t color);
void pod_fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void pod_draw_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void pod_fill_polygon(int16_t amount, int16_t *vertices, uint16_t color);
void pod_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void pod_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void pod_clear_screen();

```

For text ouput the fonts must be of same format as [dhepper/font8x8](https://github.com/dhepper/font8x8).

```c
void pod_put_char(char ascii, uint16_t x0, uint16_t y0, uint16_t color, char font[128][8]);
void pod_put_text(char *str, uint16_t x0, uint16_t y0, uint16_t color, char font[128][8]);
```

Blit copies a [bitmap](https://github.com/tuupola/copepod/blob/master/bitmap.c) to the screen. You can also copy a bitmap scaled up or down.

```c
void pod_blit(uint16_t x0, uint16_t y0, bitmap_t *source);
void pod_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source);
```

Depending on HAL, initialisation and flushing might be required. If HAL does not require them both functions are just a no-op.

```c
pod_init();
pod_put_pixel(x0, y0, color);
pod_flush();
pod_destroy();
```

## Speed

Tests done with ESP32 and ST7735S. Framebuffer FPS was capped to 30.

```
I (218846) main: 8106.7 circles per second, FB 30.0 FPS
I (223896) main: 7516.8 filled circles per second, FB 30.0 FPS
I (228946) main: 3968.8 five sided polygons per second, FB 30.0 FPS
I (233996) main: 1296.2 five sided filled polygons per second, FB 30.0 FPS
I (244096) main: 32504.7 characters per second, FB 30.0 FPS
I (249146) main: 121879.9 pixels per second, FB 30.0 FPS
I (254196) main: 2133.3 filled triangles per second, FB 30.0 FPS
I (259246) main: 6486.4 triangles per second, FB 30.0 FPS
I (264296) main: 7628.1 filled rectangles per second, FB 30.0 FPS
I (270356) main: 30674.7 rectangles per second, FB 30.0 FPS
I (275406) main: 17581.6 lines per second, FB 30.0 FPS
```

Tests done with ESP32 and ILI9341.

```
I (37371) main: 7810.0 circles per second, FB 29.0 FPS
I (42421) main: 7463.6 filled circles per second, FB 29.0 FPS
I (47471) main: 1980.0 five sided polygons per second, FB 29.0 FPS
I (52521) main: 663.0 five sided filled polygons per second, FB 29.0 FPS



## License

The MIT License (MIT). Please see [License File](LICENSE.md) for more information.
