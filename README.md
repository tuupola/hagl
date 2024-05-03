# Hardware Agnostic Graphics Library

HAGL is a lightweight hardware agnostics graphics library. It supports basic geometric primitives, bitmaps, blitting, fixed width fonts. Library tries to stay lightweight but targets reasonably powerful microchips such as ESP32. There is no dynamic allocation.

This can still be considered work in progress. API should be 90% stable.

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE)

![Old school demo effects](https://appelsiini.net/img/2020/esp-effects.jpg)

## Backend

To use HAGL you must provide a backend. The backend must provide atleast a function for putting a pixel. If nothing else is provided all higher level graphical functions will use this function to draw the primitives. While proper documentation is lacking see the example backend implementations for [GD](https://github.com/tuupola/hagl_gd), [SDL2](https://github.com/tuupola/hagl_sdl2), [ESP-IDF (Ilitek, Sitronix, Galaxycore)](https://github.com/tuupola/hagl_esp_mipi), [ESP-IDF (Solomon)](https://github.com/tuupola/hagl_esp_solomon), [Nuclei RISC-V SDK](https://github.com/tuupola/hagl_gd32v_mipi), [Raspberry Pi Pico SDK](https://github.com/tuupola/hagl_pico_mipi) and [Raspberry Pi Pico VGA board](https://github.com/CHiPs44/hagl_pico_vgaboard).

## Usage

High level functions are pretty self explanatory. For example applications see [Pico Effects](https://github.com/tuupola/pico_effects), [ESP Effects](https://github.com/tuupola/esp_effects), [SDL2 Effects](https://github.com/tuupola/sdl2_effects), [ESP GFX](https://github.com/tuupola/esp_gfx), and [GD32V Effects](https://github.com/tuupola/gd32v_effects/).

### Lifecycle

Before you start drawing you should call `hagl_init()`. Some HAL configurations require you to call `hagl_flush()` to update the contents of the screen. Before exiting your program it is good idea to call `hagl_close()`to clean things up.

```c
#include <hagl_hal.h>
#include <hagl.h>

hagl_backend_t *display = hagl_init();

/* Main loop. */
while (1) {
    hagl_clear(display);
    hagl_load_image(display, 0, 0, "/sdcard/hello.jpg");
    hagl_flush(display);
};

hagl_close(display);
```

### Colors

HAL defines what kind of pixel format is used. Most common is RGB565 which is represented by two bytes. If you are sure you will be using only RGB565 colors you could use the following shortcut to create a random color.

```c
hagl_color_t color = rand() % 0xffff;
```

To write portable code which can be run with different pixel formats use the following instead.

```c
uint8_t r = rand() % 255;
uint8_t g = rand() % 255;
uint8_t b = rand() % 255;
hagl_color_t color = hagl_color(display, r, g, b);
```

### Put a pixel

```c
for (uint32_t i = 1; i < 100000; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    hagl_color_t color = rand() % 0xffff;

    hagl_put_pixel(display, x0, y0, color);
}
```

![Random pixels](https://appelsiini.net/img/2020/hagl-put-pixel-gh.png)

## Get a pixel

```c
int16_t x0 = rand() % display->width;
int16_t y0 = rand() % display->height;

hagl_color_t pixel = hagl_get_pixel(display, x0, y0);
```

Note that if requesting coordinates outside the clip window color black is returned. This behaviour is unoptimal and might change in the future.

### Draw a line

```c
for (uint16_t i = 1; i < 1000; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    int16_t x1 = rand() % display->width;
    int16_t y1 = rand() % display->height;
    hagl_color_t color = rand() % 0xffff;

    hagl_draw_line(display, x0, y0, x1, y1, color);
}
```

![Random lines](https://www.appelsiini.net/img/2020/pod-draw-line.png)

### Draw a horizontal line

```c
for (uint16_t i = 1; i < 1000; i++) {
    int16_t x0 = rand() % (display->width / 2);
    int16_t y0 = rand() % display->height;
    int16_t x1 = rand() % (display->width / 2);
    int16_t width = rand() % (display->width - x0);
    hagl_color_t color = rand() % 0xffff;

    /* First two are aliases. */
    hagl_draw_hline(display, x0, y0, width, color);
    hagl_draw_hline_xyw(display, x0, y0, width, color);

    hagl_draw_hline_xyx(display, x0, y0, x1, color);
}
```

![Random horizontal lines](https://appelsiini.net/img/2020/pod-draw-hline.png)

### Draw a vertical line

```c
for (uint16_t i = 1; i < 1000; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % (display->height / 2);
    int16_t y1 = rand() % (display->height / 2);
    int16_t height = rand() % (display->height - y0);
    hagl_color_t color = rand() % 0xffff;

    /* First two are aliases. */
    hagl_draw_vline(display, x0, y0, height, color);
    hagl_draw_vline_xyh(display, x0, y0, height, color);

    hagl_draw_vline_xyy(display, x0, y0, y1, color);
}
```

![Random vertical lines](https://appelsiini.net/img/2020/pod-draw-vline.png)

### Draw a circle

```c
for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = display->width / 2;
    int16_t y0 = display->height / 2;
    int16_t radius = rand() % display->width;
    hagl_color_t color = rand() % 0xffff;

    hagl_draw_circle(display, x0, y0, radius, color);
}
```

![Random circle](https://appelsiini.net/img/2020/pod-draw-circle.png)

### Draw a filled circle

```c
for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    int16_t radius = rand() % 100;
    hagl_color_t color = rand() % 0xffff;

    hagl_fill_circle(display, x0, y0, radius, color);
}
```

![Random filled circle](https://appelsiini.net/img/2020/pod-fill-circle.png)

### Draw an ellipse

```c
for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = display->width / 2;
    int16_t y0 = display->height / 2;
    int16_t rx = rand() % display->width;
    int16_t ry = rand() % display->height;
    hagl_color_t color = rand() % 0xffff;

    hagl_draw_ellipse(display, x0, y0, rx, ry, color);
}
```

![Random ellipse](https://appelsiini.net/img/2020/hagl-draw-ellipse.png)

### Draw a filled ellipse

```c
for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    int16_t rx = rand() % display->width / 4;
    int16_t ry = rand() % display->height / 4;
    hagl_color_t color = rand() % 0xffff;

    hagl_fill_ellipse(display, x0, y0, rx, ry, color);
}
```

![Random filled ellipse](https://appelsiini.net/img/2020/hagl-fill-ellipse.png)

### Draw a triangle

```c
int16_t x0 = rand() % display->width;
int16_t y0 = rand() % display->height;
int16_t x1 = rand() % display->width;
int16_t y1 = rand() % display->height;
int16_t x2 = rand() % display->width;
int16_t y2 = rand() % display->height;
hagl_color_t color = rand() % 0xffff;

hagl_draw_triangle(display, x0, y0, x1, y1, x2, y2, color);
```

![Random triangle](https://appelsiini.net/img/2020/pod-draw-triangle.png)

### Draw a filled triangle

```c
int16_t x0 = rand() % display->width;
int16_t y0 = rand() % display->height;
int16_t x1 = rand() % display->width;
int16_t y1 = rand() % display->height;
int16_t x2 = rand() % display->width;
int16_t y2 = rand() % display->height;
hagl_color_t color = rand() % 0xffff;

hagl_fill_triangle(display, x0, y0, x1, y1, x2, y2, color);
```

![Random filled triangle](https://appelsiini.net/img/2020/pod-fill-triangle.png)

### Draw a rectangle

```c
for (uint16_t i = 1; i < 50; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    int16_t x1 = rand() % display->width;
    int16_t y1 = rand() % display->height;
    int16_t w = rand() % display->width / 2;
    int16_t h = rand() % display->height / 2;
    hagl_color_t color = rand() % 0xffff;

    /* First two are aliases. */
    hagl_draw_rectangle(display, x0, y0, x1, y1, color);
    hagl_draw_rectangle_xyxy(display, x0, y0, x1, y1, color);

    hagl_draw_rectangle_xywh(display, x0, y0, w, h, color);
}
```

![Random rectangle](https://appelsiini.net/img/2020/pod-draw-rectangle.png)

### Draw a filled rectangle

```c
for (uint16_t i = 1; i < 10; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    int16_t x1 = rand() % display->width;
    int16_t y1 = rand() % display->height;
    int16_t w = rand() % display->width / 2;
    int16_t h = rand() % display->height / 2;
    hagl_color_t color = rand() % 0xffff;

    /* First two are aliases. */
    hagl_fill_rectangle(display, x0, y0, x1, y1, color);
    hagl_fill_rectangle_xyxy(display, x0, y0, x1, y1, color);

    hagl_fill_rectangle_xywh(display, x0, y0, w, h, color);
}
```

![Random filled rectangle](https://appelsiini.net/img/2020/pod-fill-rectangle.png)

### Draw a rounded rectangle

```c
for (uint16_t i = 1; i < 30; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    int16_t x1 = rand() % display->width;
    int16_t y1 = rand() % display->height;
    int16_t w = rand() % display->width / 2;
    int16_t h = rand() % display->height / 2;
    int16_t r = 10
    hagl_color_t color = rand() % 0xffff;

    /* First two are aliases. */
    hagl_draw_rounded_rectangle(display, x0, y0, x1, y1, r, color);
    hagl_draw_rounded_rectangle_xyxy(display, x0, y0, x1, y1, r, color);

    hagl_draw_rounded_rectangle_xywh(display, x0, y0, w, h, r, color);
}
```

![Random rounded rectangle](https://appelsiini.net/img/2020/hagl-draw-rounded-rectangle.png)

### Draw a filled rounded rectangle

```c
for (uint16_t i = 1; i < 30; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    int16_t x1 = rand() % display->width;
    int16_t y1 = rand() % display->height;
    int16_t w = rand() % display->width / 2;
    int16_t h = rand() % display->height / 2;
    int16_t r = 10
    hagl_color_t color = rand() % 0xffff;

    /* First two are aliases. */
    hagl_fill_rounded_rectangle(display, x0, y0, x1, y1, r, color);
    hagl_fill_rounded_rectangle_xyxy(display, x0, y0, x1, y1, r, color);

    hagl_fill_rounded_rectangle_xyxy(display, x0, y0, w, h, r, color);
}
```

![Random filled rounded rectangle](https://appelsiini.net/img/2020/hagl-fill-rounded-rectangle.png)

### Draw a polygon

You can draw polygons with unlimited number of vertices which are passed as an array. Pass the number of vertices as the first argument.

```c
int16_t x0 = rand() % display->width;
int16_t y0 = rand() % display->height;
int16_t x1 = rand() % display->width;
int16_t y1 = rand() % display->height;
int16_t x2 = rand() % display->width;
int16_t y2 = rand() % display->height;
int16_t x3 = rand() % display->width;
int16_t y3 = rand() % display->height;
int16_t x4 = rand() % display->width;
int16_t y4 = rand() % display->height;
hagl_color_t color = rand() % 0xffff;
int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};

hagl_draw_polygon(display, 5, vertices, color);
```

![Random polygon](https://appelsiini.net/img/2020/pod-draw-polygon.png)

### Draw a filled polygon

You can draw filled polygons with up to 64 vertices which are passed as an array. First argument is the number of vertices. Polygon does **not** have to be concave.

```c
int16_t x0 = rand() % display->width;
int16_t y0 = rand() % display->height;
int16_t x1 = rand() % display->width;
int16_t y1 = rand() % display->height;
int16_t x2 = rand() % display->width;
int16_t y2 = rand() % display->height;
int16_t x3 = rand() % display->width;
int16_t y3 = rand() % display->height;
int16_t x4 = rand() % display->width;
int16_t y4 = rand() % display->height;
hagl_color_t color = rand() % 0xffff;
int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};

hagl_fill_polygon(display, 5, vertices, color);
```

![Random filled polygon](https://appelsiini.net/img/2020/pod-fill-polygon.png)

### Put a single char

The library supports Unicode fonts in fontx format. It only includes three fonts by default. You can find more at [tuupola/embedded-fonts](https://github.com/tuupola/embedded-fonts) and [CHiPs44/fontx2-fonts](https://github.com/CHiPs44/fontx2-fonts) repositories.

```c
for (uint16_t i = 1; i < 10000; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    hagl_color_t color = rand() % 0xffff;
    char code = rand() % 255;

    hagl_put_char(display, code, x0, y0, color, font8x8);
}
```

![Random chars](https://appelsiini.net/img/2020/hagl-put-char-gh.png)

### Put a string

The library supports Unicode fonts in fontx format. It only includes three fonts by default. You can find more at [tuupola/embedded-fonts](https://github.com/tuupola/embedded-fonts) repository.

```c
for (uint16_t i = 1; i < 10000; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    hagl_color_t color = rand() % 0xffff;

    hagl_put_text(display, u"YO! MTV raps.", x0, y0, color, font6x9);
}
```

![Random strings](https://appelsiini.net/img/2020/hagl-put-text-gh.png)

### Blit a bitmap

Blit copies a [bitmap](https://github.com/tuupola/hagl/blob/master/bitmap.c) to the screen. This example uses a glyph bitmap which is extracted from a font.

```c
hagl_bitmap_t bitmap;
bitmap.buffer = (uint8_t *) malloc(6 * 9 * sizeof(hagl_color_t));

for (uint16_t i = 1; i < 20000; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    hagl_color_t color = rand() % 0xffff;
    uint16_t code = rand() % 0xffff;
    hagl_get_glyph(display, code, color, &bitmap, font6x9);

    /* These two are aliases. */
    hagl_blit(display, x0, y0, &bitmap);
    hagl_blit_xy(display, x0, y0, &bitmap);
}
```

![Random blits](https://appelsiini.net/img/2020/hagl-blit.png)

### Blit a bitmap scaled up or down

Scale blit copies and scales a [bitmap](https://github.com/tuupola/hagl/blob/master/bitmap.c) to the surface. This example uses a glyph bitmap which is extracted from a font.

```c
hagl_bitmap_t bitmap;
bitmap.buffer = (uint8_t *) malloc(6 * 9 * sizeof(hagl_color_t));

for (uint16_t i = 1; i < 20000; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    hagl_color_t color = rand() % 0xffff;
    uint16_t code = rand() % 0xffff;
    hagl_get_glyph(display, code, color, &bitmap, font6x9);

    /* These two examples do the same thing. */
    hagl_blit_xywh(display, x0, y0, 24, 36, &bitmap);
    hagl_blit_xyxy(display, x0, y0, x0 + 23, y0 + 35, &bitmap);
}
```

![Random blits](https://appelsiini.net/img/2020/hagl-scale-blit.png)

### Clip window

You can restrict the area of drawing by setting a clip window.

```c
hagl_set_clip(display, 0, 40, display->width, display->height - 40);

for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = rand() % display->width;
    int16_t y0 = rand() % display->height;
    int16_t radius = rand() % 100;
    hagl_color_t color = rand() % 0xffff;

    hagl_fill_circle(display, x0, y0, radius, color);
}
````

![Clipped windows](https://appelsiini.net/img/2020/hagl-set-clip-window.png)

## License

The MIT License (MIT). Please see [License File](LICENSE.md) for more information.
