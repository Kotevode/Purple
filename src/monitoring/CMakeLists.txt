cmake_minimum_required(VERSION 3.6)
project(monitoring)

add_library(monitoring include/ src/console/Logger.cpp src/web/Logger.cpp src/console/Formatter.cpp src/console/Formatter.h src/web/Formatter.cpp src/web/Formatter.h)
target_include_directories(monitoring
        PUBLIC include
        PRIVATE src)
target_link_libraries(monitoring purple json)

add_executable(console_logger_tests test/console_logger_tests.cpp)
target_link_libraries(console_logger_tests monitoring)
target_link_libraries(console_logger_tests gtest gtest_main)

add_executable(web_logger_tests test/web_logger_tests.cpp)
target_link_libraries(web_logger_tests monitoring)
target_link_libraries(web_logger_tests gtest gtest_main)