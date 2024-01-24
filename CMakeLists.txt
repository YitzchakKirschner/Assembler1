cmake_minimum_required(VERSION 3.0)

# Set the project name
project(MyAssemblerProject)

# Add an executable
add_executable(assembler main.c macro_extracter.c errors.c)

# Here, 'my_executable_name' is the name of the executable you want to build,
# and 'main.c' is the main C file. If you have more source files, list them here.
