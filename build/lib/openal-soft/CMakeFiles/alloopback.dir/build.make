# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/antzhdanov/code/funcdoodle

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/antzhdanov/code/funcdoodle/build

# Include any dependencies generated for this target.
include lib/openal-soft/CMakeFiles/alloopback.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/openal-soft/CMakeFiles/alloopback.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/openal-soft/CMakeFiles/alloopback.dir/progress.make

# Include the compile flags for this target's objects.
include lib/openal-soft/CMakeFiles/alloopback.dir/flags.make

lib/openal-soft/CMakeFiles/alloopback.dir/examples/alloopback.c.o: lib/openal-soft/CMakeFiles/alloopback.dir/flags.make
lib/openal-soft/CMakeFiles/alloopback.dir/examples/alloopback.c.o: /Users/antzhdanov/code/funcdoodle/lib/openal-soft/examples/alloopback.c
lib/openal-soft/CMakeFiles/alloopback.dir/examples/alloopback.c.o: lib/openal-soft/CMakeFiles/alloopback.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/antzhdanov/code/funcdoodle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/openal-soft/CMakeFiles/alloopback.dir/examples/alloopback.c.o"
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/openal-soft/CMakeFiles/alloopback.dir/examples/alloopback.c.o -MF CMakeFiles/alloopback.dir/examples/alloopback.c.o.d -o CMakeFiles/alloopback.dir/examples/alloopback.c.o -c /Users/antzhdanov/code/funcdoodle/lib/openal-soft/examples/alloopback.c

lib/openal-soft/CMakeFiles/alloopback.dir/examples/alloopback.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/alloopback.dir/examples/alloopback.c.i"
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/antzhdanov/code/funcdoodle/lib/openal-soft/examples/alloopback.c > CMakeFiles/alloopback.dir/examples/alloopback.c.i

lib/openal-soft/CMakeFiles/alloopback.dir/examples/alloopback.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/alloopback.dir/examples/alloopback.c.s"
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/antzhdanov/code/funcdoodle/lib/openal-soft/examples/alloopback.c -o CMakeFiles/alloopback.dir/examples/alloopback.c.s

# Object files for target alloopback
alloopback_OBJECTS = \
"CMakeFiles/alloopback.dir/examples/alloopback.c.o"

# External object files for target alloopback
alloopback_EXTERNAL_OBJECTS =

lib/openal-soft/alloopback: lib/openal-soft/CMakeFiles/alloopback.dir/examples/alloopback.c.o
lib/openal-soft/alloopback: lib/openal-soft/CMakeFiles/alloopback.dir/build.make
lib/openal-soft/alloopback: /opt/homebrew/Cellar/sdl2/2.30.0/lib/libSDL2.dylib
lib/openal-soft/alloopback: lib/openal-soft/libal-excommon.a
lib/openal-soft/alloopback: lib/openal-soft/libopenal.1.23.1.dylib
lib/openal-soft/alloopback: lib/openal-soft/CMakeFiles/alloopback.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/antzhdanov/code/funcdoodle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable alloopback"
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/alloopback.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/openal-soft/CMakeFiles/alloopback.dir/build: lib/openal-soft/alloopback
.PHONY : lib/openal-soft/CMakeFiles/alloopback.dir/build

lib/openal-soft/CMakeFiles/alloopback.dir/clean:
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && $(CMAKE_COMMAND) -P CMakeFiles/alloopback.dir/cmake_clean.cmake
.PHONY : lib/openal-soft/CMakeFiles/alloopback.dir/clean

lib/openal-soft/CMakeFiles/alloopback.dir/depend:
	cd /Users/antzhdanov/code/funcdoodle/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/antzhdanov/code/funcdoodle /Users/antzhdanov/code/funcdoodle/lib/openal-soft /Users/antzhdanov/code/funcdoodle/build /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft/CMakeFiles/alloopback.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : lib/openal-soft/CMakeFiles/alloopback.dir/depend

