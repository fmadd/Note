# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /snap/cmake/1399/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1399/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/Victoriya/Desktop/Study/good/Note

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build

# Utility rule file for glad-generate-files.

# Include any custom commands dependencies for this target.
include _deps/glad-build/CMakeFiles/glad-generate-files.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/glad-build/CMakeFiles/glad-generate-files.dir/progress.make

_deps/glad-build/CMakeFiles/glad-generate-files: _deps/glad-build/src/glad.c
_deps/glad-build/CMakeFiles/glad-generate-files: _deps/glad-build/include/glad/glad.h

_deps/glad-build/src/glad.c:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating GLAD"
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-src && /usr/bin/python3.10 -m glad --profile=compatibility --out-path=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build --api= --generator=c --extensions= --spec=gl

_deps/glad-build/include/glad/glad.h: _deps/glad-build/src/glad.c
	@$(CMAKE_COMMAND) -E touch_nocreate _deps/glad-build/include/glad/glad.h

glad-generate-files: _deps/glad-build/CMakeFiles/glad-generate-files
glad-generate-files: _deps/glad-build/include/glad/glad.h
glad-generate-files: _deps/glad-build/src/glad.c
glad-generate-files: _deps/glad-build/CMakeFiles/glad-generate-files.dir/build.make
.PHONY : glad-generate-files

# Rule to build all files generated by this target.
_deps/glad-build/CMakeFiles/glad-generate-files.dir/build: glad-generate-files
.PHONY : _deps/glad-build/CMakeFiles/glad-generate-files.dir/build

_deps/glad-build/CMakeFiles/glad-generate-files.dir/clean:
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build && $(CMAKE_COMMAND) -P CMakeFiles/glad-generate-files.dir/cmake_clean.cmake
.PHONY : _deps/glad-build/CMakeFiles/glad-generate-files.dir/clean

_deps/glad-build/CMakeFiles/glad-generate-files.dir/depend:
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Victoriya/Desktop/Study/good/Note /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-src /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build/CMakeFiles/glad-generate-files.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : _deps/glad-build/CMakeFiles/glad-generate-files.dir/depend

