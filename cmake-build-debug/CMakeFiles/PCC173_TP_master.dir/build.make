# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /home/bernardo/clion-2017.3.4/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/bernardo/clion-2017.3.4/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/PCC173_TP_master.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/PCC173_TP_master.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PCC173_TP_master.dir/flags.make

CMakeFiles/PCC173_TP_master.dir/Main.cpp.o: CMakeFiles/PCC173_TP_master.dir/flags.make
CMakeFiles/PCC173_TP_master.dir/Main.cpp.o: ../Main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PCC173_TP_master.dir/Main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PCC173_TP_master.dir/Main.cpp.o -c "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/Main.cpp"

CMakeFiles/PCC173_TP_master.dir/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PCC173_TP_master.dir/Main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/Main.cpp" > CMakeFiles/PCC173_TP_master.dir/Main.cpp.i

CMakeFiles/PCC173_TP_master.dir/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PCC173_TP_master.dir/Main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/Main.cpp" -o CMakeFiles/PCC173_TP_master.dir/Main.cpp.s

CMakeFiles/PCC173_TP_master.dir/Main.cpp.o.requires:

.PHONY : CMakeFiles/PCC173_TP_master.dir/Main.cpp.o.requires

CMakeFiles/PCC173_TP_master.dir/Main.cpp.o.provides: CMakeFiles/PCC173_TP_master.dir/Main.cpp.o.requires
	$(MAKE) -f CMakeFiles/PCC173_TP_master.dir/build.make CMakeFiles/PCC173_TP_master.dir/Main.cpp.o.provides.build
.PHONY : CMakeFiles/PCC173_TP_master.dir/Main.cpp.o.provides

CMakeFiles/PCC173_TP_master.dir/Main.cpp.o.provides.build: CMakeFiles/PCC173_TP_master.dir/Main.cpp.o


# Object files for target PCC173_TP_master
PCC173_TP_master_OBJECTS = \
"CMakeFiles/PCC173_TP_master.dir/Main.cpp.o"

# External object files for target PCC173_TP_master
PCC173_TP_master_EXTERNAL_OBJECTS =

PCC173_TP_master: CMakeFiles/PCC173_TP_master.dir/Main.cpp.o
PCC173_TP_master: CMakeFiles/PCC173_TP_master.dir/build.make
PCC173_TP_master: CMakeFiles/PCC173_TP_master.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable PCC173_TP_master"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PCC173_TP_master.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PCC173_TP_master.dir/build: PCC173_TP_master

.PHONY : CMakeFiles/PCC173_TP_master.dir/build

CMakeFiles/PCC173_TP_master.dir/requires: CMakeFiles/PCC173_TP_master.dir/Main.cpp.o.requires

.PHONY : CMakeFiles/PCC173_TP_master.dir/requires

CMakeFiles/PCC173_TP_master.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PCC173_TP_master.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PCC173_TP_master.dir/clean

CMakeFiles/PCC173_TP_master.dir/depend:
	cd "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master" "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master" "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/cmake-build-debug" "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/cmake-build-debug" "/home/bernardo/Desktop/bernardo/UFOP/Otimizacao em Redes/PCC173-TP-master/cmake-build-debug/CMakeFiles/PCC173_TP_master.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/PCC173_TP_master.dir/depend

