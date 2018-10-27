cmake_minimum_required(VERSION 3.3)
project(DPHPC)

# Option
option(BUILD_TDEBUG "Debug mode" OFF)

# Requirements check

## OMP
FIND_PACKAGE(OpenMP)
if(OPENMP_FOUND)
		message("OPENMP FOUND")
endif()

## MPI Support
find_package(MPI) 
if(OPENMP_FOUND)
		message("MPI FOUND")
endif()

# Set variables

## Set compile flags
set(CMAKE_CXX_FLAGS "-std=c++11 ${OpenMP_CXX_FLAGS}")
if(BUILD_TDEBUG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif(BUILD_TDEBUG)

## Support for vim YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Set directory fo binaries 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

## LibLSB
if(NOT EXISTS $ENV{LIBLSB_PATH})
    set(LIBLSB_PATH "/usr/local/lib")
    message("LIBLSB_PATH set to default")
else()
    set(LIBLSB_PATH $ENV{LIBLSB_PATH})
endif()

if(NOT EXISTS "${LIBLSB_PATH}/liblsb.a")
    message(WARNING "LIBLSB NOT FOUND IN LOCATION $ENV{LIBLSB_PATH}/liblsb.a")
endif()


# Define files to compile

## Precise includes
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
include_directories("include/")

## Gather source for main executable 
file(GLOB SRCS src/*)

## Add executable 
add_executable(exec ${SRCS})

target_link_libraries(exec ${MPI_CXX_LIBRARIES} ${LIBLSB_PATH}/liblsb.a)
