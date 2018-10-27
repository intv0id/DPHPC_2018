cmake_minimum_required(VERSION 3.3)
project(DPHPC)

# Option
option(BUILD_TDEBUG "Debug mode" OFF)

# Set compile flags
set(CMAKE_CXX_FLAGS "-std=c++11 ${OpenMP_CXX_FLAGS}")
if(BUILD_TDEBUG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif(BUILD_TDEBUG)


#OMP
FIND_PACKAGE(OpenMP)
if(OPENMP_FOUND)
		message("OPENMP FOUND")
endif()

# MPI Support
find_package(MPI) 
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# Support for vim YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set directory fo binaries 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Precise includes
include_directories("include/")

# Gather source for main executable 
file(GLOB SRCS src/*)

# Add executable 
add_executable(exec ${SRCS})

target_link_libraries(exec ${MPI_CXX_LIBRARIES} /usr/local/lib/liblsb.a)
