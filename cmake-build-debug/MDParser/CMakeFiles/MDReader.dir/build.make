# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /cygdrive/c/Users/laure/AppData/Local/JetBrains/CLion2020.3/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/laure/AppData/Local/JetBrains/CLion2020.3/cygwin_cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug"

# Include any dependencies generated for this target.
include MDParser/CMakeFiles/MDReader.dir/depend.make

# Include the progress variables for this target.
include MDParser/CMakeFiles/MDReader.dir/progress.make

# Include the compile flags for this target's objects.
include MDParser/CMakeFiles/MDReader.dir/flags.make

MDParser/CMakeFiles/MDReader.dir/MDReader.cpp.o: MDParser/CMakeFiles/MDReader.dir/flags.make
MDParser/CMakeFiles/MDReader.dir/MDReader.cpp.o: ../MDParser/MDReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object MDParser/CMakeFiles/MDReader.dir/MDReader.cpp.o"
	cd "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/MDParser" && /usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MDReader.dir/MDReader.cpp.o -c "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/MDParser/MDReader.cpp"

MDParser/CMakeFiles/MDReader.dir/MDReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MDReader.dir/MDReader.cpp.i"
	cd "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/MDParser" && /usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/MDParser/MDReader.cpp" > CMakeFiles/MDReader.dir/MDReader.cpp.i

MDParser/CMakeFiles/MDReader.dir/MDReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MDReader.dir/MDReader.cpp.s"
	cd "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/MDParser" && /usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/MDParser/MDReader.cpp" -o CMakeFiles/MDReader.dir/MDReader.cpp.s

# Object files for target MDReader
MDReader_OBJECTS = \
"CMakeFiles/MDReader.dir/MDReader.cpp.o"

# External object files for target MDReader
MDReader_EXTERNAL_OBJECTS =

MDParser/libMDReader.a: MDParser/CMakeFiles/MDReader.dir/MDReader.cpp.o
MDParser/libMDReader.a: MDParser/CMakeFiles/MDReader.dir/build.make
MDParser/libMDReader.a: MDParser/CMakeFiles/MDReader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libMDReader.a"
	cd "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/MDParser" && $(CMAKE_COMMAND) -P CMakeFiles/MDReader.dir/cmake_clean_target.cmake
	cd "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/MDParser" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MDReader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
MDParser/CMakeFiles/MDReader.dir/build: MDParser/libMDReader.a

.PHONY : MDParser/CMakeFiles/MDReader.dir/build

MDParser/CMakeFiles/MDReader.dir/clean:
	cd "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/MDParser" && $(CMAKE_COMMAND) -P CMakeFiles/MDReader.dir/cmake_clean.cmake
.PHONY : MDParser/CMakeFiles/MDReader.dir/clean

MDParser/CMakeFiles/MDReader.dir/depend:
	cd "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project" "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/MDParser" "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug" "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/MDParser" "/cygdrive/c/Users/laure/OneDrive - The University of Chicago/2021 - (5) Spring Semester/FINM 32700 - Advance Computing for Finance/final_project/cmake-build-debug/MDParser/CMakeFiles/MDReader.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : MDParser/CMakeFiles/MDReader.dir/depend
