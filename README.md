# Copepod

Copepod is a lightweight hardware agnostics graphics library. It supports basic geometric primitives, bitmaps, blitting, fixed width fonts and an optional framebuffer. Everything is still work in progress. Things will change without warning. I'm doing this to learn c.

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE.md)

## Hardware Abstraction Layer

To use Copepod you must provide a hardware absraction layer ie. HAL. It must provide atleast the `pod_hal_putpixel(x0, y0, color)` function. If nothing else is provided all higher level graphical functions will use this function to draw the primitive. There are other optional HAL functions. While proper documentation is lacking see the [ESP ILI9341 HAL](https://github.com/tuupola/copepod/blob/master/esp-ili9341-framebuffer.c).

## Usage

Guess what! Better docs to be written. High level functions are pretty self explanatory though.

```c
pod_putpixel(uint16_t x0, uint16_t y0, uint16_t color);
pod_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
pod_hline(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color);
pod_vline(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color);
pod_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
pod_fillrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
```

For text ouput the fonts must be of same format as [dhepper/font8x8](https://github.com/dhepper/font8x8).

```c
pod_putchar(char ascii, uint16_t x0, uint16_t y0, uint16_t color, char font[128][8]);
pod_puttext(char *str, uint16_t x0, uint16_t y0, uint16_t color, char font[128][8]);
```

Blit copies a [bitmap](https://github.com/tuupola/copepod/blob/master/bitmap.c) to the screen. You can also copy a bitmap scaled up or down.

```c
pod_blit(uint16_t x0, uint16_t y0, bitmap_t *source);
pod_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *source);
```

Depending on HAL, initialisation and flushing might be required. If HAL does not require them both functions are just a no-op.

```c
pod_init();
pod_putpixel(x0, y0, color);
pod_flush();
```

## License

The MIT License (MIT). Please see [License File](LICENSE.md) for more information.
