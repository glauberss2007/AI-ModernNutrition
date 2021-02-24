add_executable(dinamic-tests dinamicTests.cpp)
target_link_libraries(dinamic-tests PUBLIC modern-nutrition-solver)

add_executable(prediction-tests predictionsTests.cpp)
target_link_libraries(prediction-tests PUBLIC modern-nutrition-solver)

add_executable(evaluation-tests evaluationTests.cpp)
target_link_libraries(evaluation-tests PUBLIC modern-nutrition-solver)





