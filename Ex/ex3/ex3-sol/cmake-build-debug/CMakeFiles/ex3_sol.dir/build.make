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
CMAKE_SOURCE_DIR = /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ex3_sol.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ex3_sol.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ex3_sol.dir/flags.make

CMakeFiles/ex3_sol.dir/RBTree.c.o: CMakeFiles/ex3_sol.dir/flags.make
CMakeFiles/ex3_sol.dir/RBTree.c.o: ../RBTree.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ex3_sol.dir/RBTree.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ex3_sol.dir/RBTree.c.o   -c /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/RBTree.c

CMakeFiles/ex3_sol.dir/RBTree.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ex3_sol.dir/RBTree.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/RBTree.c > CMakeFiles/ex3_sol.dir/RBTree.c.i

CMakeFiles/ex3_sol.dir/RBTree.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ex3_sol.dir/RBTree.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/RBTree.c -o CMakeFiles/ex3_sol.dir/RBTree.c.s

CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.o: CMakeFiles/ex3_sol.dir/flags.make
CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.o: ../utilities/RBUtilities.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.o   -c /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/utilities/RBUtilities.c

CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/utilities/RBUtilities.c > CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.i

CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/utilities/RBUtilities.c -o CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.s

# Object files for target ex3_sol
ex3_sol_OBJECTS = \
"CMakeFiles/ex3_sol.dir/RBTree.c.o" \
"CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.o"

# External object files for target ex3_sol
ex3_sol_EXTERNAL_OBJECTS =

ex3_sol: CMakeFiles/ex3_sol.dir/RBTree.c.o
ex3_sol: CMakeFiles/ex3_sol.dir/utilities/RBUtilities.c.o
ex3_sol: CMakeFiles/ex3_sol.dir/build.make
ex3_sol: CMakeFiles/ex3_sol.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ex3_sol"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex3_sol.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ex3_sol.dir/build: ex3_sol

.PHONY : CMakeFiles/ex3_sol.dir/build

CMakeFiles/ex3_sol.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ex3_sol.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ex3_sol.dir/clean

CMakeFiles/ex3_sol.dir/depend:
	cd /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/cmake-build-debug /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/cmake-build-debug /mnt/c/Weisler/Study/StudySemesterB/C/ProgrammingWorkshopInC/Ex/ex3/ex3-sol/cmake-build-debug/CMakeFiles/ex3_sol.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ex3_sol.dir/depend

