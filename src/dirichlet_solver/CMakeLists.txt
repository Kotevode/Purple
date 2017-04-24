project(dirichlet_solver)
add_executable(dirichlet_solver src/main.cpp src/DirichletJob.cpp src/DirichletJob.h src/Input.h src/InputParser.h src/StreamParser.h src/StreamParser.cpp)
set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(dirichlet_solver purple)
target_link_libraries(dirichlet_solver cxxopts)

add_executable(dirichlet_solver_tests test/input_parser_tests.cpp src/StreamParser.cpp src/DirichletJob.cpp)
target_link_libraries(dirichlet_solver_tests gtest gtest_main)
target_link_libraries(dirichlet_solver_tests purple)