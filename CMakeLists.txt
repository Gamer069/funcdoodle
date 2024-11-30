cmake_minimum_required(VERSION 3.10)
if(NOT WIN32)
project(FuncDoodle C CXX OBJC)
else()
project(FuncDoodle C CXX)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set build configurations
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}")

# Glad sources
set(GLAD_SOURCES
    lib/glad/src/glad.c
)

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

if(APPLE)
set(NFD_SOURCES lib/nfd/src/nfd_cocoa.m lib/nfd/src/nfd_common.c)
elseif(WIN32)
set(NFD_SOURCES lib/nfd/src/nfd_win.cpp lib/nfd/src/nfd_common.c)
elseif(UNIX AND NOT APPLE)
set(NFD_SOURCES lib/nfd/src/nfd_zenity.cpp lib/nfd/src/nfd_common.c)
endif()

set(PORTAUDIO_LIBDIR "" CACHE STRING "required")
set(PORTAUDIO_INCLDIR "" CACHE STRING "required")

if(NOT PORTAUDIO_LIBDIR)
    if(APPLE)
        set(PORTAUDIO_LIBDIR "/usr/local/opt/portaudio/lib/")
    elseif(UNIX AND NOT APPLE)
        set(PORTAUDIO_LIBDIR "/usr/local/lib/")
    elseif(WIN32)
        set(PORTAUDIO_LIBDIR "C:\\Program Files\\PortAudio\\lib\\")
    endif()
endif()

if(NOT PORTAUDIO_INCLDIR)
    if(APPLE)
        set(PORTAUDIO_INCLDIR "/usr/local/opt/portaudio/include")
    elseif(UNIX AND NOT APPLE)
        set(PORTAUDIO_INCLDIR "/usr/local/include")
    elseif(WIN32)
        set(PORTAUDIO_INCLDIR "C:\\portaudio\\include")
    endif()
endif()

add_subdirectory(lib/glfw)

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
    lib/portaudio/include
    lib/imgui
    lib/imgui/backends
    lib/glad/include
    lib/nfd/src/include
    lib/stb/
    lib/glm/
    ${PORTAUDIO_INCLDIR}
)

target_link_libraries(FuncDoodle
    glfw
    ${CMAKE_DL_LIBS}
    m
    pthread
    dl
)


# Link libraries based on platform
if(APPLE)
    target_link_libraries(FuncDoodle
        ${PORTAUDIO_LIBDIR}libportaudio.dylib
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework AppKit"
   )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(FuncDoodle
        ${PORTAUDIO_LIBDIR}libportaudio.so
    )
elseif(WIN32)
    target_link_libraries(FuncDoodle
        ${PORTAUDIO_LIBDIR}portaudio.lib
    )
endif()

# Build options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(FuncDoodle PRIVATE DEBUG ALSOFT_EAX)
    target_compile_options(FuncDoodle PRIVATE -g -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(FuncDoodle PRIVATE -fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(FuncDoodle PRIVATE NDEBUG ALSOFT_EAX)
    target_compile_options(FuncDoodle PRIVATE -O3)
endif()

# Extra flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
