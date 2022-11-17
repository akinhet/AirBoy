idf_build_get_property(target IDF_TARGET)

set(srcs "src/airboy_math.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include")