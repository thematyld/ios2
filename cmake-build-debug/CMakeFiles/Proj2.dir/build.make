# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /home/mabartos/Documents/CLion/CLion-2018.1.2/clion-2018.1.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/mabartos/Documents/CLion/CLion-2018.1.2/clion-2018.1.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Proj2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Proj2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Proj2.dir/flags.make

CMakeFiles/Proj2.dir/proj2.c.o: CMakeFiles/Proj2.dir/flags.make
CMakeFiles/Proj2.dir/proj2.c.o: ../proj2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Proj2.dir/proj2.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Proj2.dir/proj2.c.o   -c /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/proj2.c

CMakeFiles/Proj2.dir/proj2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Proj2.dir/proj2.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/proj2.c > CMakeFiles/Proj2.dir/proj2.c.i

CMakeFiles/Proj2.dir/proj2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Proj2.dir/proj2.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/proj2.c -o CMakeFiles/Proj2.dir/proj2.c.s

CMakeFiles/Proj2.dir/proj2.c.o.requires:

.PHONY : CMakeFiles/Proj2.dir/proj2.c.o.requires

CMakeFiles/Proj2.dir/proj2.c.o.provides: CMakeFiles/Proj2.dir/proj2.c.o.requires
	$(MAKE) -f CMakeFiles/Proj2.dir/build.make CMakeFiles/Proj2.dir/proj2.c.o.provides.build
.PHONY : CMakeFiles/Proj2.dir/proj2.c.o.provides

CMakeFiles/Proj2.dir/proj2.c.o.provides.build: CMakeFiles/Proj2.dir/proj2.c.o


# Object files for target Proj2
Proj2_OBJECTS = \
"CMakeFiles/Proj2.dir/proj2.c.o"

# External object files for target Proj2
Proj2_EXTERNAL_OBJECTS =

Proj2: CMakeFiles/Proj2.dir/proj2.c.o
Proj2: CMakeFiles/Proj2.dir/build.make
Proj2: CMakeFiles/Proj2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Proj2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Proj2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Proj2.dir/build: Proj2

.PHONY : CMakeFiles/Proj2.dir/build

CMakeFiles/Proj2.dir/requires: CMakeFiles/Proj2.dir/proj2.c.o.requires

.PHONY : CMakeFiles/Proj2.dir/requires

CMakeFiles/Proj2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Proj2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Proj2.dir/clean

CMakeFiles/Proj2.dir/depend:
	cd /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2 /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2 /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/cmake-build-debug /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/cmake-build-debug /home/mabartos/Documents/Private/VUT/IOS/Proj2/ios2/cmake-build-debug/CMakeFiles/Proj2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Proj2.dir/depend

