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

add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})

add_library(boost-mpi SHARED IMPORTED)
set_property(TARGET boost-mpi PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET boost-mpi PROPERTY IMPORTED_LOCATION ${Boost_MPI_LIBRARY})

add_library(boost-serialization SHARED IMPORTED)
set_property(TARGET boost-serialization PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET boost-serialization PROPERTY IMPORTED_LOCATION ${Boost_SERIALIZATION_LIBRARY})


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