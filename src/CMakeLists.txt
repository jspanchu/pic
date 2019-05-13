cmake_minimum_required(VERSION 2.6)
include_directories(${CMAKE_BINARY_DIR})

#Get compiler id and set flags
if(CMAKE_CXX_COMPILER_ID MATCHES Intel)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -w")
    set(CMAKE_CXX_FLAGS_DEBUG   "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -xhost")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -w")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native")
endif()

# set default to Release build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

add_executable(tsi vel_dist.cpp file_io.cpp num_density.cpp poisson_solver.cpp particle_mover.cpp driver.cpp)

# Find MKL
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
find_package(MKL)
if(MKL_FOUND)
    include_directories(${MKL_INCLUDE_DIRS})
    target_link_libraries(tsi mkl_intel_lp64 mkl_sequential mkl_core)
endif()

# Create Output directory and remove existing one
if(EXISTS ${CMAKE_BINARY_DIR}/output)
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/output)
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)
