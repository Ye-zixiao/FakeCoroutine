# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/e/Desktop/FakeCoroutine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/Desktop/FakeCoroutine/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Coroutine.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Coroutine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Coroutine.dir/flags.make

CMakeFiles/Coroutine.dir/main.cpp.o: CMakeFiles/Coroutine.dir/flags.make
CMakeFiles/Coroutine.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/Desktop/FakeCoroutine/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Coroutine.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Coroutine.dir/main.cpp.o -c /mnt/e/Desktop/FakeCoroutine/main.cpp

CMakeFiles/Coroutine.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Coroutine.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/Desktop/FakeCoroutine/main.cpp > CMakeFiles/Coroutine.dir/main.cpp.i

CMakeFiles/Coroutine.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Coroutine.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/Desktop/FakeCoroutine/main.cpp -o CMakeFiles/Coroutine.dir/main.cpp.s

CMakeFiles/Coroutine.dir/Coroutine.cpp.o: CMakeFiles/Coroutine.dir/flags.make
CMakeFiles/Coroutine.dir/Coroutine.cpp.o: ../Coroutine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/Desktop/FakeCoroutine/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Coroutine.dir/Coroutine.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Coroutine.dir/Coroutine.cpp.o -c /mnt/e/Desktop/FakeCoroutine/Coroutine.cpp

CMakeFiles/Coroutine.dir/Coroutine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Coroutine.dir/Coroutine.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/Desktop/FakeCoroutine/Coroutine.cpp > CMakeFiles/Coroutine.dir/Coroutine.cpp.i

CMakeFiles/Coroutine.dir/Coroutine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Coroutine.dir/Coroutine.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/Desktop/FakeCoroutine/Coroutine.cpp -o CMakeFiles/Coroutine.dir/Coroutine.cpp.s

# Object files for target Coroutine
Coroutine_OBJECTS = \
"CMakeFiles/Coroutine.dir/main.cpp.o" \
"CMakeFiles/Coroutine.dir/Coroutine.cpp.o"

# External object files for target Coroutine
Coroutine_EXTERNAL_OBJECTS =

Coroutine: CMakeFiles/Coroutine.dir/main.cpp.o
Coroutine: CMakeFiles/Coroutine.dir/Coroutine.cpp.o
Coroutine: CMakeFiles/Coroutine.dir/build.make
Coroutine: CMakeFiles/Coroutine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/Desktop/FakeCoroutine/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Coroutine"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Coroutine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Coroutine.dir/build: Coroutine

.PHONY : CMakeFiles/Coroutine.dir/build

CMakeFiles/Coroutine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Coroutine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Coroutine.dir/clean

CMakeFiles/Coroutine.dir/depend:
	cd /mnt/e/Desktop/FakeCoroutine/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/Desktop/FakeCoroutine /mnt/e/Desktop/FakeCoroutine /mnt/e/Desktop/FakeCoroutine/cmake-build-debug /mnt/e/Desktop/FakeCoroutine/cmake-build-debug /mnt/e/Desktop/FakeCoroutine/cmake-build-debug/CMakeFiles/Coroutine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Coroutine.dir/depend
