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
CMAKE_SOURCE_DIR = /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/build

# Include any dependencies generated for this target.
include CMakeFiles/Project_ImGui.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Project_ImGui.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Project_ImGui.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Project_ImGui.dir/flags.make

CMakeFiles/Project_ImGui.dir/src/main.cpp.o: CMakeFiles/Project_ImGui.dir/flags.make
CMakeFiles/Project_ImGui.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/Project_ImGui.dir/src/main.cpp.o: CMakeFiles/Project_ImGui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Project_ImGui.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Project_ImGui.dir/src/main.cpp.o -MF CMakeFiles/Project_ImGui.dir/src/main.cpp.o.d -o CMakeFiles/Project_ImGui.dir/src/main.cpp.o -c /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/src/main.cpp

CMakeFiles/Project_ImGui.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project_ImGui.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/src/main.cpp > CMakeFiles/Project_ImGui.dir/src/main.cpp.i

CMakeFiles/Project_ImGui.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project_ImGui.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/src/main.cpp -o CMakeFiles/Project_ImGui.dir/src/main.cpp.s

CMakeFiles/Project_ImGui.dir/src/crpt.cpp.o: CMakeFiles/Project_ImGui.dir/flags.make
CMakeFiles/Project_ImGui.dir/src/crpt.cpp.o: ../src/crpt.cpp
CMakeFiles/Project_ImGui.dir/src/crpt.cpp.o: CMakeFiles/Project_ImGui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Project_ImGui.dir/src/crpt.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Project_ImGui.dir/src/crpt.cpp.o -MF CMakeFiles/Project_ImGui.dir/src/crpt.cpp.o.d -o CMakeFiles/Project_ImGui.dir/src/crpt.cpp.o -c /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/src/crpt.cpp

CMakeFiles/Project_ImGui.dir/src/crpt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project_ImGui.dir/src/crpt.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/src/crpt.cpp > CMakeFiles/Project_ImGui.dir/src/crpt.cpp.i

CMakeFiles/Project_ImGui.dir/src/crpt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project_ImGui.dir/src/crpt.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/src/crpt.cpp -o CMakeFiles/Project_ImGui.dir/src/crpt.cpp.s

# Object files for target Project_ImGui
Project_ImGui_OBJECTS = \
"CMakeFiles/Project_ImGui.dir/src/main.cpp.o" \
"CMakeFiles/Project_ImGui.dir/src/crpt.cpp.o"

# External object files for target Project_ImGui
Project_ImGui_EXTERNAL_OBJECTS =

Project_ImGui: CMakeFiles/Project_ImGui.dir/src/main.cpp.o
Project_ImGui: CMakeFiles/Project_ImGui.dir/src/crpt.cpp.o
Project_ImGui: CMakeFiles/Project_ImGui.dir/build.make
Project_ImGui: libs/cryptopp-cmake/cryptopp-cmake/cryptopp/libcryptopp.a
Project_ImGui: libimgui.a
Project_ImGui: /usr/lib/x86_64-linux-gnu/libGL.so
Project_ImGui: _deps/glfw-build/src/libglfw3.a
Project_ImGui: /usr/lib/x86_64-linux-gnu/librt.a
Project_ImGui: /usr/lib/x86_64-linux-gnu/libm.so
Project_ImGui: /usr/lib/x86_64-linux-gnu/libX11.so
Project_ImGui: _deps/glad-build/libglad.a
Project_ImGui: CMakeFiles/Project_ImGui.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Project_ImGui"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Project_ImGui.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Project_ImGui.dir/build: Project_ImGui
.PHONY : CMakeFiles/Project_ImGui.dir/build

CMakeFiles/Project_ImGui.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Project_ImGui.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Project_ImGui.dir/clean

CMakeFiles/Project_ImGui.dir/depend:
	cd /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/build /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/build /mnt/c/Users/Victoriya/Desktop/Study/good/FINALE/build/CMakeFiles/Project_ImGui.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Project_ImGui.dir/depend

