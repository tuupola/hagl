# Changelog

All notable changes to this project will be documented in this file, in reverse chronological order by release.

## [0.6.0-dev](https://github.com/tuupola/hagl/compare/0.5.0...master)

### Changed
- You must now pass drawing surface to all drawing functions ([#65](https://github.com/tuupola/hagl/pull/65)).
    ```c
    hagl_backend_t *display = hagl_init();
    hagl_put_pixel(display, x, y, color);
- Primitive drawing code was splitted to separate files ([#79](https://github.com/tuupola/hagl/pull/79)).
- Rename `hagl_clear_screen()` to `hagl_clear()` ([#76](https://github.com/tuupola/hagl/pull/76)).
### Added
- Possibility to use multiple fps counters ([#69](https://github.com/tuupola/hagl/pull/69)).
    ```c
    fps_instance_t fps;
    fps_init(&fps);
    fps_update(&fps);
    fps_reset(&fps);
    ```
- Possibility to use multiple aps counter ([#69](https://github.com/tuupola/hagl/pull/70)).
    ```c
    aps_instance_t aps;
    aps_init(&aps);
    aps_update(&aps, 1024);
    aps_reset(&aps);
    ```
- XYX and XYW variants for hline ([#82](https://github.com/tuupola/hagl/pull/82)).
- XYY and XYH variants for vline ([#83](https://github.com/tuupola/hagl/pull/83)).

## 0.5.0 - 2022-05-19

Initial release.
