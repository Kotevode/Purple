project(dirichlet_solver)
add_executable(dirichlet_solver src/main.cpp src/Job.cpp src/Job.h src/Input.h src/InputParser.h src/StreamParser.h src/StreamParser.cpp src/Processor.cpp src/Processor.h src/Result.h)
target_link_libraries(dirichlet_solver purple)
target_link_libraries(dirichlet_solver cxxopts)

add_executable(dirichlet_solver_tests test/input_parser_tests.cpp src/StreamParser.cpp src/Job.cpp test/processor_tests.cpp src/Processor.cpp)
target_link_libraries(dirichlet_solver_tests gtest gtest_main)
target_link_libraries(dirichlet_solver_tests purple)

if(MPI_COMPILE_FLAGS)
    set_target_properties(dirichlet_solver PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    set_target_properties(dirichlet_solver_tests PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(dirichlet_solver PROPERTIES
            COMPILE_FLAGS "${MPI_LINK_FLAGS}")
    set_target_properties(dirichlet_solver_tests PROPERTIES
            COMPILE_FLAGS "${MPI_LINK_FLAGS}")
endif()
