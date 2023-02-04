if (ESP_PLATFORM)

    idf_component_register(
        SRCS
            "src/hagl.c"
            "src/hagl_blit.c"
            "src/hagl_char.c"
            "src/hagl_circle.c"
            "src/hagl_clip.c"
            "src/hagl_color.c"
            "src/hagl_ellipse.c"
            "src/hagl_hline.c"
            "src/hagl_image.c"
            "src/hagl_line.c"
            "src/hagl_pixel.c"
            "src/hagl_polygon.c"
            "src/hagl_rectangle.c"
            "src/hagl_triangle.c"
            "src/hagl_vline.c"
            "src/hagl_bitmap.c"
            "src/fontx.c"
            "src/hsl.c"
            "src/rgb565.c"
            "src/rgb888.c"
            "src/tjpgd.c"
        INCLUDE_DIRS "./include"
        REQUIRES hagl_hal
    )
    add_definitions("-DHAGL_INCLUDE_SDKCONFIG_H")

else()

    add_library(hagl INTERFACE)

    target_sources(hagl INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_blit.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_char.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_circle.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_clip.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_color.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_ellipse.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_hline.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_image.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_line.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_pixel.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_polygon.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_rectangle.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_triangle.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_vline.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl_bitmap.c
        ${CMAKE_CURRENT_LIST_DIR}/src/fontx.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hsl.c
        ${CMAKE_CURRENT_LIST_DIR}/src/rgb565.c
        ${CMAKE_CURRENT_LIST_DIR}/src/rgb888.c
        ${CMAKE_CURRENT_LIST_DIR}/src/tjpgd.c
    )

    target_include_directories(hagl INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

    target_link_libraries(hagl INTERFACE hagl_hal)

endif()
