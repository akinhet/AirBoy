idf_build_get_property(target IDF_TARGET)

set(srcs "src/primitives.c" 
        "src/airboy_display.c"
        "src/ili9341.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "soc" "driver" "esp_lcd")