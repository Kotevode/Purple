cmake_minimum_required(VERSION 3.6)
project(monitoring)

add_library(monitoring include/monitoring/Group.h src/ConsoleLogger.cpp include/monitoring/ConsoleLogger.h)
target_include_directories(monitoring
        PUBLIC include
        PRIVATE src)
target_link_libraries(monitoring purple)

add_executable(monitoring_tests test/monitoring_tests.cpp test/wait.h)
target_link_libraries(monitoring_tests monitoring)
target_link_libraries(monitoring_tests gtest gtest_main)