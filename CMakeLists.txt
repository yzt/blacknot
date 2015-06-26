#-----------------------------------------------------------------------

cmake_minimum_required (VERSION 3.0)

project ("Blacknot" CXX)

#-----------------------------------------------------------------------

set (EXECUTABLE_OUTPUT_PATH $(PROJECT_BINARY_DIR)/bin)

include_directories ("include" "dep/include")
link_directories ("dep/lib/win")

if (MSVC)
	add_definitions (/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	add_definitions (-Wall)
endif ()

#-----------------------------------------------------------------------

set (BLACKNOT_HEADERS
	"include/blacknot/allocator.hpp"
	"include/blacknot/allocator.inline.hpp"
	"include/blacknot/array.hpp"
	"include/blacknot/array.inline.hpp"
	"include/blacknot/assert.hpp"
	"include/blacknot/common.hpp"
	"include/blacknot/config.hpp"
	"include/blacknot/configuration_file.hpp"
	"include/blacknot/engine.hpp"
	"include/blacknot/engine_common.hpp"
	"include/blacknot/gfx.hpp"
	"include/blacknot/macros.hpp"
	"include/blacknot/parsers.hpp"
	"include/blacknot/sdl_wrapper.hpp"
	"include/blacknot/string.hpp"
	"include/blacknot/string.inline.hpp"
	"include/blacknot/string_util.hpp"
	"include/blacknot/types.hpp"
	"include/blacknot/util.hpp"
)

set (BLACKNOT_SOURCES
	"src/blacknot/allocator.cpp"
	"src/blacknot/assert.cpp"
	"src/blacknot/configuration_file.cpp"
	"src/blacknot/engine.cpp"
	"src/blacknot/engine_common.cpp"
	"src/blacknot/gfx.cpp"
	"src/blacknot/main.cpp"
	"src/blacknot/parsers.cpp"
	"src/blacknot/sdl_wrapper.cpp"
	"src/blacknot/string_util.cpp"
	"src/blacknot/types.cpp"
	"src/blacknot/util.cpp"
)

add_executable ("blacknot_game"
	${BLACKNOT_HEADERS}
	${BLACKNOT_SOURCES}
)

#-----------------------------------------------------------------------

add_executable ("test_first"
	"src/tests/test_first_main.cpp"
)

#-----------------------------------------------------------------------

add_executable ("test_sdlblit"
	"src/tests/test_sdlblit_main.cpp"
)

target_link_libraries ("test_sdlblit" "SDL2")

#-----------------------------------------------------------------------
