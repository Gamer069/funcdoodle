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
include lib/openal-soft/CMakeFiles/openal-info.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/openal-soft/CMakeFiles/openal-info.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/openal-soft/CMakeFiles/openal-info.dir/progress.make

# Include the compile flags for this target's objects.
include lib/openal-soft/CMakeFiles/openal-info.dir/flags.make

lib/openal-soft/CMakeFiles/openal-info.dir/utils/openal-info.c.o: lib/openal-soft/CMakeFiles/openal-info.dir/flags.make
lib/openal-soft/CMakeFiles/openal-info.dir/utils/openal-info.c.o: /Users/antzhdanov/code/funcdoodle/lib/openal-soft/utils/openal-info.c
lib/openal-soft/CMakeFiles/openal-info.dir/utils/openal-info.c.o: lib/openal-soft/CMakeFiles/openal-info.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/antzhdanov/code/funcdoodle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/openal-soft/CMakeFiles/openal-info.dir/utils/openal-info.c.o"
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/openal-soft/CMakeFiles/openal-info.dir/utils/openal-info.c.o -MF CMakeFiles/openal-info.dir/utils/openal-info.c.o.d -o CMakeFiles/openal-info.dir/utils/openal-info.c.o -c /Users/antzhdanov/code/funcdoodle/lib/openal-soft/utils/openal-info.c

lib/openal-soft/CMakeFiles/openal-info.dir/utils/openal-info.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/openal-info.dir/utils/openal-info.c.i"
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/antzhdanov/code/funcdoodle/lib/openal-soft/utils/openal-info.c > CMakeFiles/openal-info.dir/utils/openal-info.c.i

lib/openal-soft/CMakeFiles/openal-info.dir/utils/openal-info.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/openal-info.dir/utils/openal-info.c.s"
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/antzhdanov/code/funcdoodle/lib/openal-soft/utils/openal-info.c -o CMakeFiles/openal-info.dir/utils/openal-info.c.s

# Object files for target openal-info
openal__info_OBJECTS = \
"CMakeFiles/openal-info.dir/utils/openal-info.c.o"

# External object files for target openal-info
openal__info_EXTERNAL_OBJECTS =

lib/openal-soft/openal-info: lib/openal-soft/CMakeFiles/openal-info.dir/utils/openal-info.c.o
lib/openal-soft/openal-info: lib/openal-soft/CMakeFiles/openal-info.dir/build.make
lib/openal-soft/openal-info: lib/openal-soft/libopenal.1.23.1.dylib
lib/openal-soft/openal-info: lib/openal-soft/CMakeFiles/openal-info.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/antzhdanov/code/funcdoodle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable openal-info"
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/openal-info.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/openal-soft/CMakeFiles/openal-info.dir/build: lib/openal-soft/openal-info
.PHONY : lib/openal-soft/CMakeFiles/openal-info.dir/build

lib/openal-soft/CMakeFiles/openal-info.dir/clean:
	cd /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft && $(CMAKE_COMMAND) -P CMakeFiles/openal-info.dir/cmake_clean.cmake
.PHONY : lib/openal-soft/CMakeFiles/openal-info.dir/clean

lib/openal-soft/CMakeFiles/openal-info.dir/depend:
	cd /Users/antzhdanov/code/funcdoodle/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/antzhdanov/code/funcdoodle /Users/antzhdanov/code/funcdoodle/lib/openal-soft /Users/antzhdanov/code/funcdoodle/build /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft /Users/antzhdanov/code/funcdoodle/build/lib/openal-soft/CMakeFiles/openal-info.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : lib/openal-soft/CMakeFiles/openal-info.dir/depend

