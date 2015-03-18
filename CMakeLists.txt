#-----------------------------------------------------------------------

cmake_minimum_required (VERSION 3.0)

project ("Blacknot" CXX)

#-----------------------------------------------------------------------

set (EXECUTABLE_OUTPUT_PATH $(PROJECT_BINARY_DIR)/bin)

include_directories ("include" "dep/include")

if (MSVC)
	add_definitions (/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	add_definitions (-Wall)
endif ()

#-----------------------------------------------------------------------

set (BLACKNOT_HEADERS
	"include/blacknot/config.hpp"
)

set (BLACKNOT_SOURCES
	"src/blacknot/main.cpp"
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
