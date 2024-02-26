cmake_minimum_required(VERSION 3.0)

# Set the project name
project(MyAssemblerProject)


# Set the build type to Debug if it's not already set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Add an executable
add_executable(assembler main.c macro_extracter.c errors.c saved_words.c first_run.c)

# Here, 'my_executable_name' is the name of the executable you want to build,
# and 'main.c' is the main C file. If you have more source files, list them h
