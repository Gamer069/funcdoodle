cmake_minimum_required(VERSION 3.10)
project(FuncDoodle C CXX OBJC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set build configurations
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

# Glad sources
set(GLAD_SOURCES
    lib/glad/src/glad.c
)

file(GLOB OPENAL_SOURCES "lib/openal-soft/**.cpp" "lib/openal-soft/core/**.cpp" "lib/openal-soft/alc/**.cpp" "lib/openal-soft/al/**.cpp")

message(STATUS ${OPENAL_SOURCES})

# ImGui sources
set(IMGUI_SOURCES
    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)

set(NFD_SOURCES lib/nfd/src/nfd_cocoa.m lib/nfd/src/nfd_common.c)

add_subdirectory(lib/glfw)
add_subdirectory(lib/openal-soft)

file(GLOB SRC "src/*.cc")

# Add executable
add_executable(FuncDoodle
    ${GLAD_SOURCES}
    ${IMGUI_SOURCES}
    ${NFD_SOURCES}
    ${OPENAL_SOURCES}
    ${SRC}
)

# Include directories
target_include_directories(FuncDoodle PRIVATE
    lib/glfw/include/
    lib/openal-soft/include
    lib/imgui
    lib/imgui/backends
    lib/glad/include
    lib/nfd/src/include
)

include_directories(lib/openal-soft/common lib/openal-soft/alc lib/openal-soft/al lib/openal-soft)

# Link libraries based on platform
if(APPLE)
    target_link_libraries(FuncDoodle
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework AppKit"
        glfw
        ${CMAKE_DL_LIBS}
        m
        pthread
        dl
    )
endif()

# Build options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(FuncDoodle PRIVATE DEBUG ALSOFT_EAX)
    target_compile_options(FuncDoodle PRIVATE -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(FuncDoodle PRIVATE NDEBUG ALSOFT_EAX)
    target_compile_options(FuncDoodle PRIVATE -O3)
endif()

# Extra flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

