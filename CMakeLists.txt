cmake_minimum_required(VERSION 3.6)
project(purple_distribution)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

############################################################
# MPI
############################################################

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

############################################################
# Boost
############################################################

find_package(Boost REQUIRED mpi serialization)
include_directories(${Boost_INCLUDE_PATH})

############################################################
# Google Test
############################################################

add_subdirectory(libs/googletest)
include_directories(${gtest_INCLUDE_DIRS})

############################################################
# Purple
############################################################

include(GNUInstallDirs)
enable_testing()
add_subdirectory(purple)

############################################################
# Dirichlet Solver
############################################################

add_subdirectory(dirichlet_solver)