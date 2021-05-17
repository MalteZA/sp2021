cmake_minimum_required(VERSION 3.19)
project(exam)

set(CMAKE_CXX_STANDARD 17)

add_executable(exam main.cpp vessel_t.h reactant_t.h rule_t.h tempRule_t.h graphMaker.h stochasticSimulator.h)

target_link_libraries(exam PUBLIC pthread)