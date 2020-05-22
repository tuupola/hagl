# Hardware Agnostic Graphics Library

HAGL is a lightweight hardware agnostics graphics library. It supports basic geometric primitives, bitmaps, blitting, fixed width fonts and an optional framebuffer. Everything is still work in progress. Things will change without warning.

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE.md)

![Old school demo effects](https://appelsiini.net/img/2020/esp-effects.jpg)

## Hardware Abstraction Layer

To use HAGL you must provide a hardware absraction layer. HAL must provide atleast the `hagl_hal_put_pixel(x0, y0, color)` function. If nothing else is provided all higher level graphical functions will use this function to draw the primitive. There are other optional HAL functions. While proper documentation is lacking see the example HAL implementations for [libgd](https://github.com/tuupola/hagl_gd), [libsdl2](https://github.com/tuupola/hagl_sdl2) and display drivers for [ESP-IDF supporting MIPI Display Command Set](https://github.com/tuupola/esp_mipi).


## Usage

Guess what! Better docs to be written. High level functions are pretty self explanatory though. For example applications see [ESP Effects](https://github.com/tuupola/esp_effects), [SDL2 Effects](https://github.com/tuupola/sdl2_effects), [ESP GFX](https://github.com/tuupola/esp_gfx), and [ESP M5StickC](https://github.com/tuupola/esp_m5stick).

### Lifecycle

Before you start drawing you should call `hagl_init()`. Some HAL configurations require you to call `hagl_flush()` to update the contents of the screen. Before exiting your program it is good idea to call `hagl_close()`to clean things up.

```c
hagl_init();

/* Main loop. */
while (1) {
    hagl_clear_screen();
    hagl_load_image(0, 0, "/sdcard/hello.jpg");
    hagl_flush();
};

hagl_close();
```

### Colors

HAL defines what kind of color is used. Most common is RGB565 which is represented by two bytes. If you are sure you will be using only RGB565 colors you could use something like following to create a random color.

```c
color_t color = rand() % 0xffff;
```

To write portable code which can be run with different color systems use the following instead.

```c
uint8_t r = rand() % 255;
uint8_t g = rand() % 255;
uint8_t b = rand() % 255;
color_t color = hagl_color(r, g, b);
```

### Put a pixel

```c
for (uint32_t i = 1; i < 100000; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;

    hagl_put_pixel(x0, y0, color);
}
```
![Random pixels](https://appelsiini.net/img/2020/hagl-put-pixel-gh.png)

### Draw a line


```c
for (uint16_t i = 1; i < 1000; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t x1 = rand() % DISPLAY_WIDTH;
    int16_t y1 = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;

    hagl_draw_line(x0, y0, x1, y1, color);
}
```

![Random lines](https://appelsiini.net/img/2020/pod-draw-line.png)

### Draw a line

```c
for (uint16_t i = 1; i < 1000; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t x1 = rand() % DISPLAY_WIDTH;
    int16_t y1 = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;

    hagl_draw_line(x0, y0, x1, y1, color);
}
```

![Random lines](https://appelsiini.net/img/2020/pod-draw-line.png)

### Draw a horizontal line

```c
for (uint16_t i = 1; i < 1000; i++) {
    int16_t x0 = rand() % (DISPLAY_WIDTH / 2);
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t width = rand() % (DISPLAY_WIDTH - x0);
    color_t color = rand() % 0xffff;

    hagl_draw_hline(x0, y0, width, color);
}
```

![Random horizontal lines](https://appelsiini.net/img/2020/pod-draw-hline.png)

### Draw a vertical line

```c
for (uint16_t i = 1; i < 1000; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % (DISPLAY_HEIGHT / 2);
    int16_t height = rand() % (DISPLAY_HEIGHT - y0);
    color_t color = rand() % 0xffff;

    hagl_draw_vline(x0, y0, height, color);
}
```

![Random vertical lines](https://appelsiini.net/img/2020/pod-draw-vline.png)

### Draw a circle

```c
for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = DISPLAY_WIDTH / 2;
    int16_t y0 = DISPLAY_HEIGHT / 2;
    int16_t radius = rand() % DISPLAY_WIDTH;
    color_t color = rand() % 0xffff;

    hagl_draw_circle(x0, y0, radius, color);
}
```

![Random circle](https://appelsiini.net/img/2020/pod-draw-circle.png)

### Draw a filled circle

```c
for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t radius = rand() % 100;
    color_t color = rand() % 0xffff;

    hagl_fill_circle(x0, y0, radius, color);
}
```

![Random filled circle](https://appelsiini.net/img/2020/pod-fill-circle.png)

### Draw an ellipse

```c
for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = DISPLAY_WIDTH / 2;
    int16_t y0 = DISPLAY_HEIGHT / 2;
    int16_t rx = rand() % DISPLAY_WIDTH;
    int16_t ry = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;

    hagl_draw_ellipse(x0, y0, rx, ry, color);
}
```

![Random ellipse](https://appelsiini.net/img/2020/hagl-draw-ellipse.png)

### Draw a filled ellipse

```c
for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t rx = rand() % DISPLAY_WIDTH / 4;
    int16_t ry = rand() % DISPLAY_HEIGHT / 4;
    color_t color = rand() % 0xffff;

    hagl_draw_ellipse(x0, y0, rx, ry, color);
}
```

![Random filled ellipse](https://appelsiini.net/img/2020/hagl-fill-ellipse.png)

### Draw a triangle

```c
int16_t x0 = rand() % DISPLAY_WIDTH;
int16_t y0 = rand() % DISPLAY_HEIGHT;
int16_t x1 = rand() % DISPLAY_WIDTH;
int16_t y1 = rand() % DISPLAY_HEIGHT;
int16_t x2 = rand() % DISPLAY_WIDTH;
int16_t y2 = rand() % DISPLAY_HEIGHT;
color_t color = rand() % 0xffff;

hagl_draw_triangle(x0, y0, x1, y1, x2, y2, color);
```

![Random triangle](https://appelsiini.net/img/2020/pod-draw-triangle.png)

### Draw a filled triangle

```c
int16_t x0 = rand() % DISPLAY_WIDTH;
int16_t y0 = rand() % DISPLAY_HEIGHT;
int16_t x1 = rand() % DISPLAY_WIDTH;
int16_t y1 = rand() % DISPLAY_HEIGHT;
int16_t x2 = rand() % DISPLAY_WIDTH;
int16_t y2 = rand() % DISPLAY_HEIGHT;
color_t color = rand() % 0xffff;

hagl_fill_triangle(x0, y0, x1, y1, x2, y2, color);
```

![Random filled triangle](https://appelsiini.net/img/2020/pod-fill-triangle.png)

### Draw a rectangle

```c
for (uint16_t i = 1; i < 50; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t x1 = rand() % DISPLAY_WIDTH;
    int16_t y1 = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;

    hagl_draw_rectangle(x0, y0, x1, y1, color);
}
```

![Random rectangle](https://appelsiini.net/img/2020/pod-draw-rectangle.png)

### Draw a filled rectangle

```c
for (uint16_t i = 1; i < 10; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t x1 = rand() % DISPLAY_WIDTH;
    int16_t y1 = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;

    hagl_fill_rectangle(x0, y0, x1, y1, color);
}
```

![Random filled rectangle](https://appelsiini.net/img/2020/pod-fill-rectangle.png)


### Draw a rounded rectangle

```c
for (uint16_t i = 1; i < 30; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t x1 = rand() % DISPLAY_WIDTH;
    int16_t y1 = rand() % DISPLAY_HEIGHT;
    int16_t r = 10
    color_t color = rand() % 0xffff;

    hagl_draw_rounded_rectangle(x0, y0, x1, y1, r, color);
}
```

![Random rounded rectangle](https://appelsiini.net/img/2020/hagl-draw-rounded-rectangle.png)

### Draw a filled rounded rectangle

```c
for (uint16_t i = 1; i < 30; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t x1 = rand() % DISPLAY_WIDTH;
    int16_t y1 = rand() % DISPLAY_HEIGHT;
    int16_t r = 10
    color_t color = rand() % 0xffff;

    hagl_fill_rounded_rectangle(x0, y0, x1, y1, r, color);
}
```

![Random filled rounded rectangle](https://appelsiini.net/img/2020/hagl-fill-rounded-rectangle.png)

### Draw a polygon

You can draw polygons with unlimited number of vertices which are passed as an array. Pass the number of vertices as the first argument.

```c
int16_t x0 = rand() % DISPLAY_WIDTH;
int16_t y0 = rand() % DISPLAY_HEIGHT;
int16_t x1 = rand() % DISPLAY_WIDTH;
int16_t y1 = rand() % DISPLAY_HEIGHT;
int16_t x2 = rand() % DISPLAY_WIDTH;
int16_t y2 = rand() % DISPLAY_HEIGHT;
int16_t x3 = rand() % DISPLAY_WIDTH;
int16_t y3 = rand() % DISPLAY_HEIGHT;
int16_t x4 = rand() % DISPLAY_WIDTH;
int16_t y4 = rand() % DISPLAY_HEIGHT;
color_t color = rand() % 0xffff;
int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};

hagl_draw_polygon(5, vertices, color);
```

![Random polygon](https://appelsiini.net/img/2020/pod-draw-polygon.png)

### Draw a filled polygon

You can draw filled polygons with up to 64 vertices which are passed as an array. First argument is the number of vertices. Polygon does **not** have to be concave.

```c
int16_t x0 = rand() % DISPLAY_WIDTH;
int16_t y0 = rand() % DISPLAY_HEIGHT;
int16_t x1 = rand() % DISPLAY_WIDTH;
int16_t y1 = rand() % DISPLAY_HEIGHT;
int16_t x2 = rand() % DISPLAY_WIDTH;
int16_t y2 = rand() % DISPLAY_HEIGHT;
int16_t x3 = rand() % DISPLAY_WIDTH;
int16_t y3 = rand() % DISPLAY_HEIGHT;
int16_t x4 = rand() % DISPLAY_WIDTH;
int16_t y4 = rand() % DISPLAY_HEIGHT;
color_t color = rand() % 0xffff;
int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};

hagl_fill_polygon(5, vertices, color);
```

![Random filled polygon](https://appelsiini.net/img/2020/pod-fill-polygon.png)


### Put a single char

The library supports Unicode fonts in fontx format. It only includes three fonts by default. You can find more at [tuupola/fonts](https://github.com/tuupola/fonts) repository.

```c
for (uint16_t i = 1; i < 10000; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;
    char ascii = rand() % 127;

    hagl_put_char(ascii, x0, y0, color, font8x8);
}
```

![Random chars](https://appelsiini.net/img/2020/hagl-put-char-gh.png)

### Put a string

The library supports Unicode fonts in fontx format. It only includes three fonts by default. You can find more at [tuupola/fonts](https://github.com/tuupola/fonts) repository.

```c
for (uint16_t i = 1; i < 10000; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;

    hagl_put_text(u"YO! MTV raps.", x0, y0, color, font6x9);
}
```

![Random strings](https://appelsiini.net/img/2020/hagl-put-text-gh.png)

### Blit a bitmap

Blit copies a [bitmap](https://github.com/tuupola/hagl/blob/master/bitmap.c) to the screen. This example uses a glyph bitmap which is extracted from a font.

```c
bitmap_t bitmap;
bitmap.buffer = (uint8_t *) malloc(6 * 9 * sizeof(color_t));

for (uint16_t i = 1; i < 20000; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;
    uint16_t code = rand() % 0xffff;
    hagl_get_glyph(code, color, &bitmap, font6x9);
    hagl_blit(x0, y0, &bitmap);
}
```

![Random blits](https://appelsiini.net/img/2020/hagl-blit.png)


### Blit a bitmap scaled up or down

Scale blit copies and scales a [bitmap](https://github.com/tuupola/hagl/blob/master/bitmap.c) to the screen. This example uses a glyph bitmap which is extracted from a font.

```c
bitmap_t bitmap;
bitmap.buffer = (uint8_t *) malloc(6 * 9 * sizeof(color_t));

for (uint16_t i = 1; i < 20000; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    color_t color = rand() % 0xffff;
    uint16_t code = rand() % 0xffff;
    hagl_get_glyph(code, color, &bitmap, font6x9);
    hagl_scale_blit(x0, y0, 24, 36, &bitmap);
}
```

![Random blits](https://appelsiini.net/img/2020/hagl-scale-blit.png)

### Clip window

You can restrict the area of drawing by setting a clip window.

```c
hagl_set_clip_window(0, 40, DISPLAY_WIDTH, DISPLAY_HEIGHT - 40);

for (uint16_t i = 1; i < 500; i++) {
    int16_t x0 = rand() % DISPLAY_WIDTH;
    int16_t y0 = rand() % DISPLAY_HEIGHT;
    int16_t radius = rand() % 100;
    color_t color = rand() % 0xffff;

    hagl_fill_circle(x0, y0, radius, color);
}
````

![Clipped windows](https://appelsiini.net/img/2020/hagl-set-clip-window.png)

If you want to cleant the contents of the clip window instead of clearing the whole screen call:

```c
hagl_clear_clip_window()
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
