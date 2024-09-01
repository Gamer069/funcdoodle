# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.29.6/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.29.6/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/illia/Desktop/proj/@c/funcdoodle

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/illia/Desktop/proj/@c/funcdoodle

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/3.29.6/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/3.29.6/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Install the project..."
	/usr/local/Cellar/cmake/3.29.6/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Install the project..."
	/usr/local/Cellar/cmake/3.29.6/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Installing only the local directory..."
	/usr/local/Cellar/cmake/3.29.6/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Installing only the local directory..."
	/usr/local/Cellar/cmake/3.29.6/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Installing the project stripped..."
	/usr/local/Cellar/cmake/3.29.6/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Installing the project stripped..."
	/usr/local/Cellar/cmake/3.29.6/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/illia/Desktop/proj/@c/funcdoodle/CMakeFiles /Users/illia/Desktop/proj/@c/funcdoodle//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/illia/Desktop/proj/@c/funcdoodle/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named FuncDoodle

# Build rule for target.
FuncDoodle: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 FuncDoodle
.PHONY : FuncDoodle

# fast build rule for target.
FuncDoodle/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/build
.PHONY : FuncDoodle/fast

#=============================================================================
# Target rules for targets named uninstall

# Build rule for target.
uninstall: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 uninstall
.PHONY : uninstall

# fast build rule for target.
uninstall/fast:
	$(MAKE) $(MAKESILENT) -f lib/glfw/CMakeFiles/uninstall.dir/build.make lib/glfw/CMakeFiles/uninstall.dir/build
.PHONY : uninstall/fast

#=============================================================================
# Target rules for targets named glfw

# Build rule for target.
glfw: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 glfw
.PHONY : glfw

# fast build rule for target.
glfw/fast:
	$(MAKE) $(MAKESILENT) -f lib/glfw/src/CMakeFiles/glfw.dir/build.make lib/glfw/src/CMakeFiles/glfw.dir/build
.PHONY : glfw/fast

#=============================================================================
# Target rules for targets named update_mappings

# Build rule for target.
update_mappings: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 update_mappings
.PHONY : update_mappings

# fast build rule for target.
update_mappings/fast:
	$(MAKE) $(MAKESILENT) -f lib/glfw/src/CMakeFiles/update_mappings.dir/build.make lib/glfw/src/CMakeFiles/update_mappings.dir/build
.PHONY : update_mappings/fast

lib/glad/src/glad.o: lib/glad/src/glad.c.o
.PHONY : lib/glad/src/glad.o

# target to build an object file
lib/glad/src/glad.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/glad/src/glad.c.o
.PHONY : lib/glad/src/glad.c.o

lib/glad/src/glad.i: lib/glad/src/glad.c.i
.PHONY : lib/glad/src/glad.i

# target to preprocess a source file
lib/glad/src/glad.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/glad/src/glad.c.i
.PHONY : lib/glad/src/glad.c.i

lib/glad/src/glad.s: lib/glad/src/glad.c.s
.PHONY : lib/glad/src/glad.s

# target to generate assembly for a file
lib/glad/src/glad.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/glad/src/glad.c.s
.PHONY : lib/glad/src/glad.c.s

lib/imgui/backends/imgui_impl_glfw.o: lib/imgui/backends/imgui_impl_glfw.cpp.o
.PHONY : lib/imgui/backends/imgui_impl_glfw.o

# target to build an object file
lib/imgui/backends/imgui_impl_glfw.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/backends/imgui_impl_glfw.cpp.o
.PHONY : lib/imgui/backends/imgui_impl_glfw.cpp.o

lib/imgui/backends/imgui_impl_glfw.i: lib/imgui/backends/imgui_impl_glfw.cpp.i
.PHONY : lib/imgui/backends/imgui_impl_glfw.i

# target to preprocess a source file
lib/imgui/backends/imgui_impl_glfw.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/backends/imgui_impl_glfw.cpp.i
.PHONY : lib/imgui/backends/imgui_impl_glfw.cpp.i

lib/imgui/backends/imgui_impl_glfw.s: lib/imgui/backends/imgui_impl_glfw.cpp.s
.PHONY : lib/imgui/backends/imgui_impl_glfw.s

# target to generate assembly for a file
lib/imgui/backends/imgui_impl_glfw.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/backends/imgui_impl_glfw.cpp.s
.PHONY : lib/imgui/backends/imgui_impl_glfw.cpp.s

lib/imgui/backends/imgui_impl_opengl3.o: lib/imgui/backends/imgui_impl_opengl3.cpp.o
.PHONY : lib/imgui/backends/imgui_impl_opengl3.o

# target to build an object file
lib/imgui/backends/imgui_impl_opengl3.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/backends/imgui_impl_opengl3.cpp.o
.PHONY : lib/imgui/backends/imgui_impl_opengl3.cpp.o

lib/imgui/backends/imgui_impl_opengl3.i: lib/imgui/backends/imgui_impl_opengl3.cpp.i
.PHONY : lib/imgui/backends/imgui_impl_opengl3.i

# target to preprocess a source file
lib/imgui/backends/imgui_impl_opengl3.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/backends/imgui_impl_opengl3.cpp.i
.PHONY : lib/imgui/backends/imgui_impl_opengl3.cpp.i

lib/imgui/backends/imgui_impl_opengl3.s: lib/imgui/backends/imgui_impl_opengl3.cpp.s
.PHONY : lib/imgui/backends/imgui_impl_opengl3.s

# target to generate assembly for a file
lib/imgui/backends/imgui_impl_opengl3.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/backends/imgui_impl_opengl3.cpp.s
.PHONY : lib/imgui/backends/imgui_impl_opengl3.cpp.s

lib/imgui/imgui.o: lib/imgui/imgui.cpp.o
.PHONY : lib/imgui/imgui.o

# target to build an object file
lib/imgui/imgui.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui.cpp.o
.PHONY : lib/imgui/imgui.cpp.o

lib/imgui/imgui.i: lib/imgui/imgui.cpp.i
.PHONY : lib/imgui/imgui.i

# target to preprocess a source file
lib/imgui/imgui.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui.cpp.i
.PHONY : lib/imgui/imgui.cpp.i

lib/imgui/imgui.s: lib/imgui/imgui.cpp.s
.PHONY : lib/imgui/imgui.s

# target to generate assembly for a file
lib/imgui/imgui.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui.cpp.s
.PHONY : lib/imgui/imgui.cpp.s

lib/imgui/imgui_demo.o: lib/imgui/imgui_demo.cpp.o
.PHONY : lib/imgui/imgui_demo.o

# target to build an object file
lib/imgui/imgui_demo.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_demo.cpp.o
.PHONY : lib/imgui/imgui_demo.cpp.o

lib/imgui/imgui_demo.i: lib/imgui/imgui_demo.cpp.i
.PHONY : lib/imgui/imgui_demo.i

# target to preprocess a source file
lib/imgui/imgui_demo.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_demo.cpp.i
.PHONY : lib/imgui/imgui_demo.cpp.i

lib/imgui/imgui_demo.s: lib/imgui/imgui_demo.cpp.s
.PHONY : lib/imgui/imgui_demo.s

# target to generate assembly for a file
lib/imgui/imgui_demo.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_demo.cpp.s
.PHONY : lib/imgui/imgui_demo.cpp.s

lib/imgui/imgui_draw.o: lib/imgui/imgui_draw.cpp.o
.PHONY : lib/imgui/imgui_draw.o

# target to build an object file
lib/imgui/imgui_draw.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_draw.cpp.o
.PHONY : lib/imgui/imgui_draw.cpp.o

lib/imgui/imgui_draw.i: lib/imgui/imgui_draw.cpp.i
.PHONY : lib/imgui/imgui_draw.i

# target to preprocess a source file
lib/imgui/imgui_draw.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_draw.cpp.i
.PHONY : lib/imgui/imgui_draw.cpp.i

lib/imgui/imgui_draw.s: lib/imgui/imgui_draw.cpp.s
.PHONY : lib/imgui/imgui_draw.s

# target to generate assembly for a file
lib/imgui/imgui_draw.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_draw.cpp.s
.PHONY : lib/imgui/imgui_draw.cpp.s

lib/imgui/imgui_tables.o: lib/imgui/imgui_tables.cpp.o
.PHONY : lib/imgui/imgui_tables.o

# target to build an object file
lib/imgui/imgui_tables.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_tables.cpp.o
.PHONY : lib/imgui/imgui_tables.cpp.o

lib/imgui/imgui_tables.i: lib/imgui/imgui_tables.cpp.i
.PHONY : lib/imgui/imgui_tables.i

# target to preprocess a source file
lib/imgui/imgui_tables.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_tables.cpp.i
.PHONY : lib/imgui/imgui_tables.cpp.i

lib/imgui/imgui_tables.s: lib/imgui/imgui_tables.cpp.s
.PHONY : lib/imgui/imgui_tables.s

# target to generate assembly for a file
lib/imgui/imgui_tables.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_tables.cpp.s
.PHONY : lib/imgui/imgui_tables.cpp.s

lib/imgui/imgui_widgets.o: lib/imgui/imgui_widgets.cpp.o
.PHONY : lib/imgui/imgui_widgets.o

# target to build an object file
lib/imgui/imgui_widgets.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_widgets.cpp.o
.PHONY : lib/imgui/imgui_widgets.cpp.o

lib/imgui/imgui_widgets.i: lib/imgui/imgui_widgets.cpp.i
.PHONY : lib/imgui/imgui_widgets.i

# target to preprocess a source file
lib/imgui/imgui_widgets.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_widgets.cpp.i
.PHONY : lib/imgui/imgui_widgets.cpp.i

lib/imgui/imgui_widgets.s: lib/imgui/imgui_widgets.cpp.s
.PHONY : lib/imgui/imgui_widgets.s

# target to generate assembly for a file
lib/imgui/imgui_widgets.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/imgui/imgui_widgets.cpp.s
.PHONY : lib/imgui/imgui_widgets.cpp.s

lib/nfd/src/nfd_cocoa.o: lib/nfd/src/nfd_cocoa.m.o
.PHONY : lib/nfd/src/nfd_cocoa.o

# target to build an object file
lib/nfd/src/nfd_cocoa.m.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/nfd/src/nfd_cocoa.m.o
.PHONY : lib/nfd/src/nfd_cocoa.m.o

lib/nfd/src/nfd_common.o: lib/nfd/src/nfd_common.c.o
.PHONY : lib/nfd/src/nfd_common.o

# target to build an object file
lib/nfd/src/nfd_common.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/nfd/src/nfd_common.c.o
.PHONY : lib/nfd/src/nfd_common.c.o

lib/nfd/src/nfd_common.i: lib/nfd/src/nfd_common.c.i
.PHONY : lib/nfd/src/nfd_common.i

# target to preprocess a source file
lib/nfd/src/nfd_common.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/nfd/src/nfd_common.c.i
.PHONY : lib/nfd/src/nfd_common.c.i

lib/nfd/src/nfd_common.s: lib/nfd/src/nfd_common.c.s
.PHONY : lib/nfd/src/nfd_common.s

# target to generate assembly for a file
lib/nfd/src/nfd_common.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/lib/nfd/src/nfd_common.c.s
.PHONY : lib/nfd/src/nfd_common.c.s

src/App.o: src/App.cc.o
.PHONY : src/App.o

# target to build an object file
src/App.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/App.cc.o
.PHONY : src/App.cc.o

src/App.i: src/App.cc.i
.PHONY : src/App.i

# target to preprocess a source file
src/App.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/App.cc.i
.PHONY : src/App.cc.i

src/App.s: src/App.cc.s
.PHONY : src/App.s

# target to generate assembly for a file
src/App.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/App.cc.s
.PHONY : src/App.cc.s

src/FrameRenderer.o: src/FrameRenderer.cc.o
.PHONY : src/FrameRenderer.o

# target to build an object file
src/FrameRenderer.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/FrameRenderer.cc.o
.PHONY : src/FrameRenderer.cc.o

src/FrameRenderer.i: src/FrameRenderer.cc.i
.PHONY : src/FrameRenderer.i

# target to preprocess a source file
src/FrameRenderer.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/FrameRenderer.cc.i
.PHONY : src/FrameRenderer.cc.i

src/FrameRenderer.s: src/FrameRenderer.cc.s
.PHONY : src/FrameRenderer.s

# target to generate assembly for a file
src/FrameRenderer.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/FrameRenderer.cc.s
.PHONY : src/FrameRenderer.cc.s

src/Manager.o: src/Manager.cc.o
.PHONY : src/Manager.o

# target to build an object file
src/Manager.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/Manager.cc.o
.PHONY : src/Manager.cc.o

src/Manager.i: src/Manager.cc.i
.PHONY : src/Manager.i

# target to preprocess a source file
src/Manager.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/Manager.cc.i
.PHONY : src/Manager.cc.i

src/Manager.s: src/Manager.cc.s
.PHONY : src/Manager.s

# target to generate assembly for a file
src/Manager.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/Manager.cc.s
.PHONY : src/Manager.cc.s

src/Project.o: src/Project.cc.o
.PHONY : src/Project.o

# target to build an object file
src/Project.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/Project.cc.o
.PHONY : src/Project.cc.o

src/Project.i: src/Project.cc.i
.PHONY : src/Project.i

# target to preprocess a source file
src/Project.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/Project.cc.i
.PHONY : src/Project.cc.i

src/Project.s: src/Project.cc.s
.PHONY : src/Project.s

# target to generate assembly for a file
src/Project.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/Project.cc.s
.PHONY : src/Project.cc.s

src/main.o: src/main.cc.o
.PHONY : src/main.o

# target to build an object file
src/main.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/main.cc.o
.PHONY : src/main.cc.o

src/main.i: src/main.cc.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/main.cc.i
.PHONY : src/main.cc.i

src/main.s: src/main.cc.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/FuncDoodle.dir/build.make CMakeFiles/FuncDoodle.dir/src/main.cc.s
.PHONY : src/main.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... uninstall"
	@echo "... update_mappings"
	@echo "... FuncDoodle"
	@echo "... glfw"
	@echo "... lib/glad/src/glad.o"
	@echo "... lib/glad/src/glad.i"
	@echo "... lib/glad/src/glad.s"
	@echo "... lib/imgui/backends/imgui_impl_glfw.o"
	@echo "... lib/imgui/backends/imgui_impl_glfw.i"
	@echo "... lib/imgui/backends/imgui_impl_glfw.s"
	@echo "... lib/imgui/backends/imgui_impl_opengl3.o"
	@echo "... lib/imgui/backends/imgui_impl_opengl3.i"
	@echo "... lib/imgui/backends/imgui_impl_opengl3.s"
	@echo "... lib/imgui/imgui.o"
	@echo "... lib/imgui/imgui.i"
	@echo "... lib/imgui/imgui.s"
	@echo "... lib/imgui/imgui_demo.o"
	@echo "... lib/imgui/imgui_demo.i"
	@echo "... lib/imgui/imgui_demo.s"
	@echo "... lib/imgui/imgui_draw.o"
	@echo "... lib/imgui/imgui_draw.i"
	@echo "... lib/imgui/imgui_draw.s"
	@echo "... lib/imgui/imgui_tables.o"
	@echo "... lib/imgui/imgui_tables.i"
	@echo "... lib/imgui/imgui_tables.s"
	@echo "... lib/imgui/imgui_widgets.o"
	@echo "... lib/imgui/imgui_widgets.i"
	@echo "... lib/imgui/imgui_widgets.s"
	@echo "... lib/nfd/src/nfd_cocoa.o"
	@echo "... lib/nfd/src/nfd_common.o"
	@echo "... lib/nfd/src/nfd_common.i"
	@echo "... lib/nfd/src/nfd_common.s"
	@echo "... src/App.o"
	@echo "... src/App.i"
	@echo "... src/App.s"
	@echo "... src/FrameRenderer.o"
	@echo "... src/FrameRenderer.i"
	@echo "... src/FrameRenderer.s"
	@echo "... src/Manager.o"
	@echo "... src/Manager.i"
	@echo "... src/Manager.s"
	@echo "... src/Project.o"
	@echo "... src/Project.i"
	@echo "... src/Project.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

