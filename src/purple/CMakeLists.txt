cmake_minimum_required(VERSION 3.6)
project(purple_library)
add_library(purple src/Cluster.cpp)

target_include_directories(purple PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_link_libraries(purple ${MPI_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS purple EXPORT PurpleConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT PurpleConfig DESTINATION share/Purple/cmake)

export(TARGETS purple FILE PurpleConfig.cmake)

add_executable(purple_tests test/distribution_tests.cpp test/processing_tests.cpp)
target_link_libraries(purple_tests gtest gtest_main)
target_link_libraries(purple_tests purple)
add_test(purple_tests purple_tests)