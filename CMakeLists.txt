if (ESP_PLATFORM)

    idf_component_register(
        SRCS "src/bitmap.c" "src/clip.c" "src/fontx.c" "src/hagl.c"  "src/hsl.c"  "src/rgb565.c"  "src/rgb888.c"  "src/tjpgd.c"
        INCLUDE_DIRS "./include"
        REQUIRES hagl_esp_mipi
    )
    add_definitions("-DHAGL_INCLUDE_SDKCONFIG_H")

else()

    add_library(hagl INTERFACE)

    target_sources(hagl INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/bitmap.c
        ${CMAKE_CURRENT_LIST_DIR}/src/clip.c
        ${CMAKE_CURRENT_LIST_DIR}/src/fontx.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hagl.c
        ${CMAKE_CURRENT_LIST_DIR}/src/hsl.c
        ${CMAKE_CURRENT_LIST_DIR}/src/rgb565.c
        ${CMAKE_CURRENT_LIST_DIR}/src/rgb888.c
        ${CMAKE_CURRENT_LIST_DIR}/src/tjpgd.c
    )

    target_include_directories(hagl INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

    target_link_libraries(hagl INTERFACE hagl_hal)

endif()
