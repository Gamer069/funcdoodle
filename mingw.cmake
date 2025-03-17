if(__MINGW_TOOLCHAIN_LOADED)
    return()
endif()
set(__MINGW_TOOLCHAIN_LOADED TRUE)
set(WIN32 1)
set(APPLE 0)
set(UNIX 0)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10)
set(CMAKE_CROSS_COMPILING ON)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_LINKER x86_64-w64-mingw32-ld)

include_directories(./bin/incl)
link_directories(./bin)

set(PORTAUDIO_LIBDIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/")

link_libraries(ole32 uuid propsys)

message(STATUS nice)

set(CMAKE_C_FLAGS "-fno-stack-protector")
set(CMAKE_CXX_FLAGS "-fno-stack-protector")
set(CMAKE_C_FLAGS_INIT "-fno-stack-protector")
set(CMAKE_CXX_FLAGS_INIT "-fno-stack-protector")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
