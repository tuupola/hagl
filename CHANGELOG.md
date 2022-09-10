# Changelog

All notable changes to this project will be documented in this file, in reverse chronological order by release.

## [0.6.0-dev](https://github.com/tuupola/hagl/compare/0.5.0...master)

### Changed
- The fps counter was refactored ([#69](https://github.com/tuupola/hagl/pull/69)).
- The aps counter was refactored ([#70](https://github.com/tuupola/hagl/pull/70)).
- The drawing functions can now be used on both display and a bitmap ([#65](https://github.com/tuupola/hagl/pull/65)). This also means user must pass the surface (display or bitmap) for the various functions.
    ```c
    hagl_backend_t display = hagl_init();
    hagl_put_pixel(display, x, y, color);
    ```

## 0.5.0 - 2022-05-19

Initial release.
