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
CMAKE_SOURCE_DIR = /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/build

# Include any dependencies generated for this target.
include CMakeFiles/stream_tst.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/stream_tst.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/stream_tst.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stream_tst.dir/flags.make

CMakeFiles/stream_tst.dir/test_receive_streams.cpp.o: CMakeFiles/stream_tst.dir/flags.make
CMakeFiles/stream_tst.dir/test_receive_streams.cpp.o: ../test_receive_streams.cpp
CMakeFiles/stream_tst.dir/test_receive_streams.cpp.o: CMakeFiles/stream_tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/stream_tst.dir/test_receive_streams.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/stream_tst.dir/test_receive_streams.cpp.o -MF CMakeFiles/stream_tst.dir/test_receive_streams.cpp.o.d -o CMakeFiles/stream_tst.dir/test_receive_streams.cpp.o -c /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/test_receive_streams.cpp

CMakeFiles/stream_tst.dir/test_receive_streams.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stream_tst.dir/test_receive_streams.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/test_receive_streams.cpp > CMakeFiles/stream_tst.dir/test_receive_streams.cpp.i

CMakeFiles/stream_tst.dir/test_receive_streams.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stream_tst.dir/test_receive_streams.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/test_receive_streams.cpp -o CMakeFiles/stream_tst.dir/test_receive_streams.cpp.s

CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.o: CMakeFiles/stream_tst.dir/flags.make
CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.o: /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp
CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.o: CMakeFiles/stream_tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.o -MF CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.o.d -o CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.o -c /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp

CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp > CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.i

CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp -o CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.s

CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.o: CMakeFiles/stream_tst.dir/flags.make
CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.o: /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp
CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.o: CMakeFiles/stream_tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.o -MF CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.o.d -o CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.o -c /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp

CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp > CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.i

CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp -o CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.s

# Object files for target stream_tst
stream_tst_OBJECTS = \
"CMakeFiles/stream_tst.dir/test_receive_streams.cpp.o" \
"CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.o" \
"CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.o"

# External object files for target stream_tst
stream_tst_EXTERNAL_OBJECTS =

stream_tst: CMakeFiles/stream_tst.dir/test_receive_streams.cpp.o
stream_tst: CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/log/logger_debug_stdout.cpp.o
stream_tst: CMakeFiles/stream_tst.dir/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/communication/stream_requester_client.cpp.o
stream_tst: CMakeFiles/stream_tst.dir/build.make
stream_tst: CMakeFiles/stream_tst.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable stream_tst"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stream_tst.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/stream_tst.dir/build: stream_tst
.PHONY : CMakeFiles/stream_tst.dir/build

CMakeFiles/stream_tst.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/stream_tst.dir/cmake_clean.cmake
.PHONY : CMakeFiles/stream_tst.dir/clean

CMakeFiles/stream_tst.dir/depend:
	cd /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/build /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/build /home/cristiano/Documents/Projects/Mestrado/Project/driveless/vision/tests/streaming/build/CMakeFiles/stream_tst.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/stream_tst.dir/depend

