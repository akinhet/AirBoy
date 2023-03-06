# idf_component_register(
# 	SRCS
# 	"fire-demo.c"
# 	"../../../src/display.c"
# 	"../../../src/input.c"
# 	"../../../src/sprite.c"
# 	"../../../src/array.c"
# 	"../../../src/text.c"
# 	INCLUDE_DIRS
# 	"../../../src"
# 	".")
idf_component_register(SRCS "fire-demo.c"
                    INCLUDE_DIRS "."
                    REQUIRES "airboy_viewport" "airboy_shapes" "airboy_math")
