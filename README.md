# Copepod

Copepod is a lightweight hardware agnostics graphics library. It supports basic geometric primitives, bitmaps, blitting, fixed width fonts and an optional framebuffer. Everything is still work in progress. Things will change without warning.

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE.md)

## Hardware Abstraction Layer

To use Copepod you must provide a hardware absraction layer ie. HAL. It must provide atleast the `pod_hal_putpixel(x0, y0, color)` function. If nothing else is provided all higher level graphical functions will use this function to draw the primitive. There are other optional HAL functions. While proper documentation is lacking see the example HAL implementations for [libgd](https://github.com/tuupola/copepod-libgd), [libsdl2](https://github.com/tuupola/copepod-libsdl2) and display drivers for [ESP-IDF supporting MIPI Display Command Set](https://github.com/tuupola/esp_mipi).


## Usage

Guess what! Better docs to be written. High level functions are pretty self explanatory though. For example applications see [M5Stack GFX](https://github.com/tuupola/esp-examples/tree/master/009-m5stack-gfx), [M5Stack Fire](https://github.com/tuupola/esp-examples/tree/master/010-m5stack-fire), [M5Stack Mandelbrot](https://github.com/tuupola/esp-examples/tree/master/014-mandelbrot) and [M5Stick GFX](https://github.com/tuupola/esp-examples/tree/master/015-m5stick-gfx).

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
I (63631) main: 35541.7 characters per second, FB 29.0 FPS
I (68681) main: 172289.4 pixels per second, FB 29.0 FPS
I (73731) main: 1056.8 filled triangles per second, FB 29.0 FPS
I (78781) main: 3280.4 triangles per second, FB 29.0 FPS
I (83831) main: 1856.0 filled rectangles per second, FB 29.0 FPS
I (88881) main: 20051.7 rectangles per second, FB 29.0 FPS
I (93931) main: 9457.7 lines per second, FB 29.0 FPS
```

## License

The MIT License (MIT). Please see [License File](LICENSE.md) for more information.
