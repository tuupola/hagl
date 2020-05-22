idf_component_register(
    SRCS "src/bitmap.c" "src/clip.c" "src/fontx.c" "src/hagl.c"  "src/hsl.c"  "src/rgb565.c"  "src/rgb888.c"  "src/tjpgd.c"
    INCLUDE_DIRS "./include"
    REQUIRES hagl_hal
)