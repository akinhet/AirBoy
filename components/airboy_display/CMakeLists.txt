idf_build_get_property(target IDF_TARGET)

set(srcs "src/airboy_display.c"
         "src/ili9341.c"
         "src/airboy_display_framebuffer.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    PRIV_INCLUDE_DIRS "private_include"
                    REQUIRES "soc" "driver" "esp_lcd" "airboy_viewport")