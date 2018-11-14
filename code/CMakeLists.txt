cmake_minimum_required(VERSION 3.3)
project(PMST)

# Option
option(BUILD_TDEBUG "Debug mode" OFF)

# Requirements check

## OMP
FIND_PACKAGE(OpenMP)
if(OPENMP_FOUND)
	message("OPENMP FOUND")
endif()

## TBB
if(EXISTS $ENV{TBB_PATH})
	find_package(TBB REQUIRED tbb)
else()
	find_package(TBB)
endif()
if(TBB_FOUND)
	message("TBB FOUND")
endif()

## MPI Support
find_package(MPI) 
if(OPENMP_FOUND)
	message("MPI FOUND")
endif()

# Set variables

## Set compile flags
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  ${OpenMP_EXE_LINKER_FLAGS}")
if(BUILD_TDEBUG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DDEBUG")
endif(BUILD_TDEBUG)

## Support for vim YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Set directory fo binaries 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

## LibLSB
if(NOT EXISTS $ENV{LIBLSB_PATH})
    set(LIBLSB_PATH "/usr/local")
    message("LIBLSB_PATH set to default")
else()
    set(LIBLSB_PATH $ENV{LIBLSB_PATH})
endif()

## TBB
link_libraries(tbb)
if(EXISTS $ENV{TBB_PATH})
	include_directories("$ENV{TBB_PATH}/include")
endif()

if(EXISTS $ENV{BOOST_PATH})
	include_directories("$ENV{BOOST_PATH}")
endif()

if(NOT EXISTS "${LIBLSB_PATH}/lib/liblsb.a")
    message(WARNING "LIBLSB NOT FOUND IN LOCATION ${LIBLSB_PATH}")
endif()


# Define files to compile

## Precise includes
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
include_directories(SYSTEM "${LIBLSB_PATH}/include")
include_directories("include/")


## Gather source for main executable 
file(GLOB SRCS src/* src/*/*)

## Add executable 
add_executable(exec ${SRCS})

target_link_libraries(exec ${MPI_CXX_LIBRARIES} ${LIBLSB_PATH}/lib/liblsb.a)
if(EXISTS $ENV{TBB_PATH})
	target_link_libraries(exec "$ENV{TBB_PATH}/lib/intel64/gcc4.7/libtbb.so")
endif()

