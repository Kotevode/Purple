project(dirichlet_solver)
add_executable(dirichlet_solver
        src/main.cpp
        src/StreamParser.cpp
        src/evaluation/Processor.cpp
        src/Solver.cpp
        src/evaluation/Section.h)
target_link_libraries(dirichlet_solver purple)
target_link_libraries(dirichlet_solver cxxopts)

add_executable(dirichlet_solver_tests
        test/input_parser_tests.cpp
        src/StreamParser.cpp
        test/evaluation_tests.cpp
        src/evaluation/Processor.cpp
        test/solver_tests.cpp
        src/Solver.cpp
        )
file(COPY
        test/test_input_300_1.in
        test/test_input_300_20.in
        test/test_output_300.out
        DESTINATION
        "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(dirichlet_solver_tests gtest gtest_main)
target_link_libraries(dirichlet_solver_tests purple)

if (MPI_COMPILE_FLAGS)
    set_target_properties(dirichlet_solver PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    set_target_properties(dirichlet_solver_tests PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif ()

if (MPI_LINK_FLAGS)
    set_target_properties(dirichlet_solver PROPERTIES
            COMPILE_FLAGS "${MPI_LINK_FLAGS}")
    set_target_properties(dirichlet_solver_tests PROPERTIES
            COMPILE_FLAGS "${MPI_LINK_FLAGS}")
endif ()
