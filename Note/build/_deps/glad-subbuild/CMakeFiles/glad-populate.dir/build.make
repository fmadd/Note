# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild

# Utility rule file for glad-populate.

# Include any custom commands dependencies for this target.
include CMakeFiles/glad-populate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/glad-populate.dir/progress.make

CMakeFiles/glad-populate: CMakeFiles/glad-populate-complete

CMakeFiles/glad-populate-complete: glad-populate-prefix/src/glad-populate-stamp/glad-populate-install
CMakeFiles/glad-populate-complete: glad-populate-prefix/src/glad-populate-stamp/glad-populate-mkdir
CMakeFiles/glad-populate-complete: glad-populate-prefix/src/glad-populate-stamp/glad-populate-download
CMakeFiles/glad-populate-complete: glad-populate-prefix/src/glad-populate-stamp/glad-populate-update
CMakeFiles/glad-populate-complete: glad-populate-prefix/src/glad-populate-stamp/glad-populate-patch
CMakeFiles/glad-populate-complete: glad-populate-prefix/src/glad-populate-stamp/glad-populate-configure
CMakeFiles/glad-populate-complete: glad-populate-prefix/src/glad-populate-stamp/glad-populate-build
CMakeFiles/glad-populate-complete: glad-populate-prefix/src/glad-populate-stamp/glad-populate-install
CMakeFiles/glad-populate-complete: glad-populate-prefix/src/glad-populate-stamp/glad-populate-test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'glad-populate'"
	/usr/bin/cmake -E make_directory /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles
	/usr/bin/cmake -E touch /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles/glad-populate-complete
	/usr/bin/cmake -E touch /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-done

glad-populate-prefix/src/glad-populate-stamp/glad-populate-update:
.PHONY : glad-populate-prefix/src/glad-populate-stamp/glad-populate-update

glad-populate-prefix/src/glad-populate-stamp/glad-populate-build: glad-populate-prefix/src/glad-populate-stamp/glad-populate-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'glad-populate'"
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build && /usr/bin/cmake -E echo_append
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build && /usr/bin/cmake -E touch /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-build

glad-populate-prefix/src/glad-populate-stamp/glad-populate-configure: glad-populate-prefix/tmp/glad-populate-cfgcmd.txt
glad-populate-prefix/src/glad-populate-stamp/glad-populate-configure: glad-populate-prefix/src/glad-populate-stamp/glad-populate-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'glad-populate'"
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build && /usr/bin/cmake -E echo_append
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build && /usr/bin/cmake -E touch /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-configure

glad-populate-prefix/src/glad-populate-stamp/glad-populate-download: glad-populate-prefix/src/glad-populate-stamp/glad-populate-gitinfo.txt
glad-populate-prefix/src/glad-populate-stamp/glad-populate-download: glad-populate-prefix/src/glad-populate-stamp/glad-populate-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'glad-populate'"
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps && /usr/bin/cmake -P /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/tmp/glad-populate-gitclone.cmake
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps && /usr/bin/cmake -E touch /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-download

glad-populate-prefix/src/glad-populate-stamp/glad-populate-install: glad-populate-prefix/src/glad-populate-stamp/glad-populate-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'glad-populate'"
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build && /usr/bin/cmake -E echo_append
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build && /usr/bin/cmake -E touch /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-install

glad-populate-prefix/src/glad-populate-stamp/glad-populate-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'glad-populate'"
	/usr/bin/cmake -E make_directory /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-src
	/usr/bin/cmake -E make_directory /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build
	/usr/bin/cmake -E make_directory /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix
	/usr/bin/cmake -E make_directory /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/tmp
	/usr/bin/cmake -E make_directory /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp
	/usr/bin/cmake -E make_directory /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src
	/usr/bin/cmake -E make_directory /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp
	/usr/bin/cmake -E touch /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-mkdir

glad-populate-prefix/src/glad-populate-stamp/glad-populate-patch: glad-populate-prefix/src/glad-populate-stamp/glad-populate-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'glad-populate'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-patch

glad-populate-prefix/src/glad-populate-stamp/glad-populate-update:
.PHONY : glad-populate-prefix/src/glad-populate-stamp/glad-populate-update

glad-populate-prefix/src/glad-populate-stamp/glad-populate-test: glad-populate-prefix/src/glad-populate-stamp/glad-populate-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'glad-populate'"
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build && /usr/bin/cmake -E echo_append
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-build && /usr/bin/cmake -E touch /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-test

glad-populate-prefix/src/glad-populate-stamp/glad-populate-update: glad-populate-prefix/src/glad-populate-stamp/glad-populate-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Performing update step for 'glad-populate'"
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-src && /usr/bin/cmake -P /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/glad-populate-prefix/tmp/glad-populate-gitupdate.cmake

glad-populate: CMakeFiles/glad-populate
glad-populate: CMakeFiles/glad-populate-complete
glad-populate: glad-populate-prefix/src/glad-populate-stamp/glad-populate-build
glad-populate: glad-populate-prefix/src/glad-populate-stamp/glad-populate-configure
glad-populate: glad-populate-prefix/src/glad-populate-stamp/glad-populate-download
glad-populate: glad-populate-prefix/src/glad-populate-stamp/glad-populate-install
glad-populate: glad-populate-prefix/src/glad-populate-stamp/glad-populate-mkdir
glad-populate: glad-populate-prefix/src/glad-populate-stamp/glad-populate-patch
glad-populate: glad-populate-prefix/src/glad-populate-stamp/glad-populate-test
glad-populate: glad-populate-prefix/src/glad-populate-stamp/glad-populate-update
glad-populate: CMakeFiles/glad-populate.dir/build.make
.PHONY : glad-populate

# Rule to build all files generated by this target.
CMakeFiles/glad-populate.dir/build: glad-populate
.PHONY : CMakeFiles/glad-populate.dir/build

CMakeFiles/glad-populate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glad-populate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glad-populate.dir/clean

CMakeFiles/glad-populate.dir/depend:
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild /mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/glad-subbuild/CMakeFiles/glad-populate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glad-populate.dir/depend

