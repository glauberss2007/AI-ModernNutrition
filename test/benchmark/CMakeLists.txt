cmake_minimum_required(VERSION 3.17)

add_executable(optimal-nutrition main.cpp experiment.h experiment.cpp)
target_link_libraries(optimal-nutrition PUBLIC modern-nutrition-solver)
