idf_build_get_property(target IDF_TARGET)

set(srcs "src/airboy_viewport.c"
         "src/airboy_viewport_shapes.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "airboy_display" "airboy_math" "airboy_shapes")