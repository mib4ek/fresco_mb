
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include_directories(
    ${WORKSPACE_PATH}/unit-test/mocks
    ${WORKSPACE_PATH}/include
    ${WORKSPACE_PATH}/code
    
)

# Compiler options
add_definitions(-g -D__packed__= -DUNIT_TEST)


# Library path
link_directories(
    ${WORKSPACE_PATH}/lib

)

# Define the CXX sources
set ( CXX_SRCS
	fifo_test.cpp
    ${WORKSPACE_PATH}/unit-test/mocks/Outside.cpp
)
# Define the C sources
set ( C_SRCS
    ${WORKSPACE_PATH}/code/fifoServ.c
    ${WORKSPACE_PATH}/code/fifoClient.c
    ${WORKSPACE_PATH}/code/parser.c
    ${WORKSPACE_PATH}/code/terminal.c
    ${WORKSPACE_PATH}/code/crc16.c
)
set_source_files_properties( ${C_SRCS} PROPERTIES LANGUAGE CXX )

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES
    COMPILE_FLAGS "-g -Dgtest_disable_pthreads=ON"
)

set_source_files_properties(
    ${C_SRCS} PROPERTIES
    COMPILE_FLAGS "-O1 -fprofile-arcs -ftest-coverage -fprofile-generate"
)



add_executable(UT_Fresco_TestApp ${CXX_SRCS} ${C_SRCS} )

target_link_libraries(UT_Fresco_TestApp
    ${WORKSPACE_PATH}/lib/libgtest.a
    ${WORKSPACE_PATH}/lib/libgmock.a
    ${WORKSPACE_PATH}/lib/libgtest_main.a
    ${WORKSPACE_PATH}/lib/libgmock_main.a
    gcov
    pthread
)
