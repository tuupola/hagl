# Changelog

All notable changes to this project will be documented in this file, in reverse chronological order by release.

## [0.6.0-dev](https://github.com/tuupola/hagl/compare/0.5.0...master)

### Changed
- You can now have multiple fps counters ([#69](https://github.com/tuupola/hagl/pull/69)).
- You can now have multiple aps counters ([#70](https://github.com/tuupola/hagl/pull/70)).
- You must now pass drawing surface to all drawing functions ([#65](https://github.com/tuupola/hagl/pull/65)).
    ```c
    hagl_backend_t display = hagl_init();
    hagl_put_pixel(display, x, y, color);
    ```
- Function `hagl_clear_screen()` was renamed to `hagl_clear()` ([#76](https://github.com/tuupola/hagl/pull/76)).

## 0.5.0 - 2022-05-19

Initial release.
