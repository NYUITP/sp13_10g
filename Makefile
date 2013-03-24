# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pseudorandom/src/sp13_10g

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pseudorandom/src/sp13_10g

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running interactive CMake command-line interface..."
	/usr/bin/cmake -i .
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: install/local
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: install/strip
.PHONY : install/strip/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/pseudorandom/src/sp13_10g/CMakeFiles /home/pseudorandom/src/sp13_10g/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/pseudorandom/src/sp13_10g/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named breathing

# Build rule for target.
breathing: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 breathing
.PHONY : breathing

# fast build rule for target.
breathing/fast:
	$(MAKE) -f CMakeFiles/breathing.dir/build.make CMakeFiles/breathing.dir/build
.PHONY : breathing/fast

#=============================================================================
# Target rules for targets named mongoodbc

# Build rule for target.
mongoodbc: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 mongoodbc
.PHONY : mongoodbc

# fast build rule for target.
mongoodbc/fast:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/build
.PHONY : mongoodbc/fast

#=============================================================================
# Target rules for targets named gtest

# Build rule for target.
gtest: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 gtest
.PHONY : gtest

# fast build rule for target.
gtest/fast:
	$(MAKE) -f gtest-1.6.0/CMakeFiles/gtest.dir/build.make gtest-1.6.0/CMakeFiles/gtest.dir/build
.PHONY : gtest/fast

#=============================================================================
# Target rules for targets named gtest_main

# Build rule for target.
gtest_main: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 gtest_main
.PHONY : gtest_main

# fast build rule for target.
gtest_main/fast:
	$(MAKE) -f gtest-1.6.0/CMakeFiles/gtest_main.dir/build.make gtest-1.6.0/CMakeFiles/gtest_main.dir/build
.PHONY : gtest_main/fast

src/connection_handle.o: src/connection_handle.cpp.o
.PHONY : src/connection_handle.o

# target to build an object file
src/connection_handle.cpp.o:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/connection_handle.cpp.o
.PHONY : src/connection_handle.cpp.o

src/connection_handle.i: src/connection_handle.cpp.i
.PHONY : src/connection_handle.i

# target to preprocess a source file
src/connection_handle.cpp.i:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/connection_handle.cpp.i
.PHONY : src/connection_handle.cpp.i

src/connection_handle.s: src/connection_handle.cpp.s
.PHONY : src/connection_handle.s

# target to generate assembly for a file
src/connection_handle.cpp.s:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/connection_handle.cpp.s
.PHONY : src/connection_handle.cpp.s

src/environment_handle.o: src/environment_handle.cpp.o
.PHONY : src/environment_handle.o

# target to build an object file
src/environment_handle.cpp.o:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/environment_handle.cpp.o
.PHONY : src/environment_handle.cpp.o

src/environment_handle.i: src/environment_handle.cpp.i
.PHONY : src/environment_handle.i

# target to preprocess a source file
src/environment_handle.cpp.i:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/environment_handle.cpp.i
.PHONY : src/environment_handle.cpp.i

src/environment_handle.s: src/environment_handle.cpp.s
.PHONY : src/environment_handle.s

# target to generate assembly for a file
src/environment_handle.cpp.s:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/environment_handle.cpp.s
.PHONY : src/environment_handle.cpp.s

src/odbcintf.o: src/odbcintf.cpp.o
.PHONY : src/odbcintf.o

# target to build an object file
src/odbcintf.cpp.o:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/odbcintf.cpp.o
.PHONY : src/odbcintf.cpp.o

src/odbcintf.i: src/odbcintf.cpp.i
.PHONY : src/odbcintf.i

# target to preprocess a source file
src/odbcintf.cpp.i:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/odbcintf.cpp.i
.PHONY : src/odbcintf.cpp.i

src/odbcintf.s: src/odbcintf.cpp.s
.PHONY : src/odbcintf.s

# target to generate assembly for a file
src/odbcintf.cpp.s:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/odbcintf.cpp.s
.PHONY : src/odbcintf.cpp.s

src/statement_handle.o: src/statement_handle.cpp.o
.PHONY : src/statement_handle.o

# target to build an object file
src/statement_handle.cpp.o:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/statement_handle.cpp.o
.PHONY : src/statement_handle.cpp.o

src/statement_handle.i: src/statement_handle.cpp.i
.PHONY : src/statement_handle.i

# target to preprocess a source file
src/statement_handle.cpp.i:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/statement_handle.cpp.i
.PHONY : src/statement_handle.cpp.i

src/statement_handle.s: src/statement_handle.cpp.s
.PHONY : src/statement_handle.s

# target to generate assembly for a file
src/statement_handle.cpp.s:
	$(MAKE) -f CMakeFiles/mongoodbc.dir/build.make CMakeFiles/mongoodbc.dir/src/statement_handle.cpp.s
.PHONY : src/statement_handle.cpp.s

test/breathing.o: test/breathing.cpp.o
.PHONY : test/breathing.o

# target to build an object file
test/breathing.cpp.o:
	$(MAKE) -f CMakeFiles/breathing.dir/build.make CMakeFiles/breathing.dir/test/breathing.cpp.o
.PHONY : test/breathing.cpp.o

test/breathing.i: test/breathing.cpp.i
.PHONY : test/breathing.i

# target to preprocess a source file
test/breathing.cpp.i:
	$(MAKE) -f CMakeFiles/breathing.dir/build.make CMakeFiles/breathing.dir/test/breathing.cpp.i
.PHONY : test/breathing.cpp.i

test/breathing.s: test/breathing.cpp.s
.PHONY : test/breathing.s

# target to generate assembly for a file
test/breathing.cpp.s:
	$(MAKE) -f CMakeFiles/breathing.dir/build.make CMakeFiles/breathing.dir/test/breathing.cpp.s
.PHONY : test/breathing.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... breathing"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... mongoodbc"
	@echo "... rebuild_cache"
	@echo "... gtest"
	@echo "... gtest_main"
	@echo "... src/connection_handle.o"
	@echo "... src/connection_handle.i"
	@echo "... src/connection_handle.s"
	@echo "... src/environment_handle.o"
	@echo "... src/environment_handle.i"
	@echo "... src/environment_handle.s"
	@echo "... src/odbcintf.o"
	@echo "... src/odbcintf.i"
	@echo "... src/odbcintf.s"
	@echo "... src/statement_handle.o"
	@echo "... src/statement_handle.i"
	@echo "... src/statement_handle.s"
	@echo "... test/breathing.o"
	@echo "... test/breathing.i"
	@echo "... test/breathing.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

