# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

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
CMAKE_SOURCE_DIR = /home/basiliac/Desktop/College/DB/Practics

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/basiliac/Desktop/College/DB/Practics/build

# Utility rule file for Practics_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/Practics_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Practics_autogen.dir/progress.make

CMakeFiles/Practics_autogen: Practics_autogen/timestamp

Practics_autogen/timestamp: /usr/bin/moc
Practics_autogen/timestamp: /usr/bin/uic
Practics_autogen/timestamp: CMakeFiles/Practics_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/basiliac/Desktop/College/DB/Practics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target Practics"
	/usr/bin/cmake -E cmake_autogen /home/basiliac/Desktop/College/DB/Practics/build/CMakeFiles/Practics_autogen.dir/AutogenInfo.json ""
	/usr/bin/cmake -E touch /home/basiliac/Desktop/College/DB/Practics/build/Practics_autogen/timestamp

CMakeFiles/Practics_autogen.dir/codegen:
.PHONY : CMakeFiles/Practics_autogen.dir/codegen

Practics_autogen: CMakeFiles/Practics_autogen
Practics_autogen: Practics_autogen/timestamp
Practics_autogen: CMakeFiles/Practics_autogen.dir/build.make
.PHONY : Practics_autogen

# Rule to build all files generated by this target.
CMakeFiles/Practics_autogen.dir/build: Practics_autogen
.PHONY : CMakeFiles/Practics_autogen.dir/build

CMakeFiles/Practics_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Practics_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Practics_autogen.dir/clean

CMakeFiles/Practics_autogen.dir/depend:
	cd /home/basiliac/Desktop/College/DB/Practics/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/basiliac/Desktop/College/DB/Practics /home/basiliac/Desktop/College/DB/Practics /home/basiliac/Desktop/College/DB/Practics/build /home/basiliac/Desktop/College/DB/Practics/build /home/basiliac/Desktop/College/DB/Practics/build/CMakeFiles/Practics_autogen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Practics_autogen.dir/depend

