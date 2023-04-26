cmake_minimum_required(VERSION 2.8.11)

# Project name
project(UT_Fresco_TestApp)

set(CMAKE_CXX_STANDARD 11)
set(WORKSPACE_PATH "${CMAKE_CURRENT_LIST_DIR}/")
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_host_ut)

add_compile_options(-Werror)

add_compile_options(-DUNITTEST) # pass argument to original C files

add_subdirectory(unit-test)
