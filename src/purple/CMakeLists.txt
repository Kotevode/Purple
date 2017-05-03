cmake_minimum_required(VERSION 3.6)
project(purple_library)
add_library(purple INTERFACE)

target_include_directories(purple INTERFACE include/)

target_link_libraries(purple INTERFACE ${MPI_LIBRARIES} ${Boost_LIBRARIES})

add_executable(purple_tests test/distribution_tests.cpp test/processing_tests.cpp)
target_link_libraries(purple_tests gtest gtest_main)
target_link_libraries(purple_tests purple)
add_test(purple_tests purple_tests)
if (MPI_COMPILE_FLAGS)
    set_target_properties(purple_tests PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif ()

if (MPI_LINK_FLAGS)
    set_target_properties(purple_tests PROPERTIES
            COMPILE_FLAGS "${MPI_LINK_FLAGS}")
endif ()