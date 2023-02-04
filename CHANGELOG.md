# Changelog

All notable changes to this project will be documented in this file, in reverse chronological order by release.

## [0.6.0](https://github.com/tuupola/hagl/compare/0.5.0...0.6.0) - 2023-02-04

### Changed
- You must now pass drawing surface to all drawing functions ([#65](https://github.com/tuupola/hagl/pull/65)).
    ```c
    hagl_backend_t *display = hagl_init();
    hagl_put_pixel(display, x, y, color);
- Split primitive drawing code to separate files ([#79](https://github.com/tuupola/hagl/pull/79)).
- Rename `hagl_clear_screen()` to `hagl_clear()` ([#76](https://github.com/tuupola/hagl/pull/76)).
- Rename `hagl_scale_blit()` to `hagl_blit_xywh()` ([#86](https://github.com/tuupola/hagl/pull/86)).
- Rename `hagl_set_clip_window()` to `hagl_set_clip()` ([#87](https://github.com/tuupola/hagl/pull/87)).

### Removed

- Remove `hagl_clear_clip_window()` ([#88](https://github.com/tuupola/hagl/pull/88)).
- Internal use of `DISPLAY_*` defines ([#94](https://github.com/tuupola/hagl/pull/94)).


### Added
- Possibility to use multiple fps counters ([#69](https://github.com/tuupola/hagl/pull/69)).
    ```c
    fps_instance_t fps;
    fps_init(&fps);
    fps_update(&fps);
    fps_reset(&fps);
    ```
- Possibility to use multiple aps counters ([#69](https://github.com/tuupola/hagl/pull/70)).
    ```c
    aps_instance_t aps;
    aps_init(&aps);
    aps_update(&aps, 1024);
    aps_reset(&aps);
    ```
- XYX and XYW variants for hline ([#82](https://github.com/tuupola/hagl/pull/82)).
- XYY and XYH variants for vline ([#83](https://github.com/tuupola/hagl/pull/83)).
- XYY and XYH variants for rectangle ([#84](https://github.com/tuupola/hagl/pull/84)).
- XYY and XYH variants for rounded rectangle ([#85](https://github.com/tuupola/hagl/pull/85)).
- XY, XYXY and XYWH variants for blit ([#86](https://github.com/tuupola/hagl/pull/86)).


## 0.5.0 - 2022-05-19

Initial release.
