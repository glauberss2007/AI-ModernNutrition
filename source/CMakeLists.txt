add_library(modern-nutrition-solver
        # The optimization problem
        nutrition_facts.cpp
        nutrition_facts.h
        # A candidate solution to the problem
        user_diet.cpp
        user_diet.h
        # Evolutionary algorithm
        evolutionary_algorithm.h
        evolutionary_algorithm.cpp
        # User diet preferences
        user_preferences.cpp
        user_preferences.h
        # Dynamic
        dinamic_optimization.cpp
        dinamic_optimization.h
        # Static model
        static_model.h
        static_model.cpp
        # Static mealstime-model
        static_mealstime_model.h
        static_mealstime_model.cpp
        # Dinamic model
        dinamic_model.h
        dinamic_model.cpp
        # Dinamic preferences model
        dinamic_preferences_prediction_model.h
        dinamic_preferences_prediction_model.cpp
        )
target_include_directories(modern-nutrition-solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(modern-nutrition-solver PUBLIC matplot cereal)

#######################################################
### Apply options                                   ###
#######################################################
# Set warning levels
if (BUILD_WITH_PEDANTIC_WARNINGS)
    if (MSVC)
        target_compile_options(modern-nutrition-solver PUBLIC /W4 /WX)
    else()
        # -Wno-self-assign-overloaded has a false positive that prevents pybind11
        # from working
        target_compile_options(modern-nutrition-solver PUBLIC -Wall -Wextra -pedantic -Werror -Wno-self-assign-overloaded)
    endif()
endif()
