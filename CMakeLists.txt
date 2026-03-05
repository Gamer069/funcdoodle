cmake_minimum_required(VERSION 3.10)

if(UNIX)
	set(CMAKE_C_COMPILER /usr/bin/clang)
	set(CMAKE_CXX_COMPILER /usr/bin/clang++)
endif()

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

if(APPLE)
	project(FuncDoodle C CXX OBJC)
else()
	project(FuncDoodle C CXX)
endif()

if(UNIX)
	option(ISTILING "Is using a tiling window manager?" ON)
endif()
option(BUILD_TESTS "Build and run tests from test/*.cc" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_MAKEFILE ON)

if(NOT MSVC)
	set(CMAKE_COLOR_DIAGNOSTICS ON)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-declarations -Werror")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FuncDoodleSources)
include(FuncDoodlePlatform)
include(FuncDoodleBuild)

funcdoodle_set_nfd_sources(NFD_SOURCES)
funcdoodle_set_portaudio_defaults()

add_subdirectory(lib/glfw)
file(GLOB SRC "src/*.cc")

add_executable(FuncDoodle
	${FUNCDOODLE_GLAD_SOURCES}
	${FUNCDOODLE_IMGUI_SOURCES}
	${NFD_SOURCES}
	${SRC}
)

if(BUILD_TESTS)
	file(GLOB TEST_SOURCES "test/*.cc")
	target_sources(FuncDoodle PRIVATE ${TEST_SOURCES})
	target_compile_definitions(FuncDoodle PRIVATE FUNCDOODLE_BUILD_TESTS)
	message(STATUS "BUILD_TESTS enabled -- test sources: ${TEST_SOURCES}")
endif()

target_include_directories(FuncDoodle PRIVATE
	lib/glfw/include/
	${PORTAUDIO_INCLDIR}
	lib/imgui
	lib/imgui/backends
	lib/glad/include
	lib/nfd/src/include
	lib/stb/
	lib/
	lib/toml++/
	src/
	test/
)

funcdoodle_apply_platform_links(FuncDoodle)

add_compile_definitions(__WINE__)
message(STATUS "CMAKE BUILD TYPE ${CMAKE_BUILD_TYPE}")
funcdoodle_apply_build_config(FuncDoodle)
