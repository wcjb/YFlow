# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\wcjb\Documents\CodeSpace\YFlow

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\wcjb\Documents\CodeSpace\YFlow\build

# Include any dependencies generated for this target.
include source/CMakeFiles/source.dir/depend.make

# Include the progress variables for this target.
include source/CMakeFiles/source.dir/progress.make

# Include the compile flags for this target's objects.
include source/CMakeFiles/source.dir/flags.make

source/CMakeFiles/source.dir/Random.cpp.obj: source/CMakeFiles/source.dir/flags.make
source/CMakeFiles/source.dir/Random.cpp.obj: ../source/Random.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\wcjb\Documents\CodeSpace\YFlow\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object source/CMakeFiles/source.dir/Random.cpp.obj"
	cd /d C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\source.dir\Random.cpp.obj -c C:\Users\wcjb\Documents\CodeSpace\YFlow\source\Random.cpp

source/CMakeFiles/source.dir/Random.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/source.dir/Random.cpp.i"
	cd /d C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\wcjb\Documents\CodeSpace\YFlow\source\Random.cpp > CMakeFiles\source.dir\Random.cpp.i

source/CMakeFiles/source.dir/Random.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/source.dir/Random.cpp.s"
	cd /d C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\wcjb\Documents\CodeSpace\YFlow\source\Random.cpp -o CMakeFiles\source.dir\Random.cpp.s

source/CMakeFiles/source.dir/tools.cpp.obj: source/CMakeFiles/source.dir/flags.make
source/CMakeFiles/source.dir/tools.cpp.obj: ../source/tools.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\wcjb\Documents\CodeSpace\YFlow\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object source/CMakeFiles/source.dir/tools.cpp.obj"
	cd /d C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\source.dir\tools.cpp.obj -c C:\Users\wcjb\Documents\CodeSpace\YFlow\source\tools.cpp

source/CMakeFiles/source.dir/tools.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/source.dir/tools.cpp.i"
	cd /d C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\wcjb\Documents\CodeSpace\YFlow\source\tools.cpp > CMakeFiles\source.dir\tools.cpp.i

source/CMakeFiles/source.dir/tools.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/source.dir/tools.cpp.s"
	cd /d C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\wcjb\Documents\CodeSpace\YFlow\source\tools.cpp -o CMakeFiles\source.dir\tools.cpp.s

# Object files for target source
source_OBJECTS = \
"CMakeFiles/source.dir/Random.cpp.obj" \
"CMakeFiles/source.dir/tools.cpp.obj"

# External object files for target source
source_EXTERNAL_OBJECTS =

source/libsource.a: source/CMakeFiles/source.dir/Random.cpp.obj
source/libsource.a: source/CMakeFiles/source.dir/tools.cpp.obj
source/libsource.a: source/CMakeFiles/source.dir/build.make
source/libsource.a: source/CMakeFiles/source.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\wcjb\Documents\CodeSpace\YFlow\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libsource.a"
	cd /d C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source && $(CMAKE_COMMAND) -P CMakeFiles\source.dir\cmake_clean_target.cmake
	cd /d C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\source.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
source/CMakeFiles/source.dir/build: source/libsource.a

.PHONY : source/CMakeFiles/source.dir/build

source/CMakeFiles/source.dir/clean:
	cd /d C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source && $(CMAKE_COMMAND) -P CMakeFiles\source.dir\cmake_clean.cmake
.PHONY : source/CMakeFiles/source.dir/clean

source/CMakeFiles/source.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\wcjb\Documents\CodeSpace\YFlow C:\Users\wcjb\Documents\CodeSpace\YFlow\source C:\Users\wcjb\Documents\CodeSpace\YFlow\build C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source C:\Users\wcjb\Documents\CodeSpace\YFlow\build\source\CMakeFiles\source.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : source/CMakeFiles/source.dir/depend

