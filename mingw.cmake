# Toolchain file for Clang with MinGW-w64 cross-compilation from Linux to Windows

# Set Clang as the compiler
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

# Set the target system to Windows
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Set the MinGW-w64 root path for cross-compiling
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Ensure that only libraries and includes are found in the MinGW-w64 directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set Clang flags for cross-compiling with MinGW-w64
set(CMAKE_CXX_FLAGS "-D__USE_MINGW_ANSI_STDIO=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Iincl -L.")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Iincl -L. -fno-stack-protector -D_FORTIFY_SOURCE=2")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
