[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/MacOS/badge.svg)](https://github.com/glauberss2007/modern-nutrition/actions)
[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/Windows/badge.svg)](https://github.com/glauberss2007/modern-nutrition/actions/actions)
[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/Ubuntu/badge.svg)](https://github.com/glauberss2007/modern-nutrition/actions/actions)

<p align="center">
  <img src="https://repository-images.githubusercontent.com/254842585/4dfa7580-7ffb-11ea-99d0-46b8fe2f4170" height="175" width="auto" />
</p>

# Modern Nutrition

Since the classic diet planning problem, several mathematical models and programming techniques have been shown to generate menus following several restrictions. However, a new trend not yet addressed is behavioral nutrition. This scientific approach, focused on nutritional counseling, proves to be more effective than the common restrictive diets, by adapting to the habits and preferences of users. Therefore, aiming to contribute to a non-restrictive and intuitive diet, in this project we propose a new dinamic optimization model considering preferences predictions.

## Features

- [Modern CMake practices](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/)
- Clean separation of library and executable code
- Continuous integration via [GitHub Actions](https://help.github.com/en/actions/)
- Reproducible dependency management via [CPM.cmake](https://github.com/TheLartians/CPM.cmake)

## Usage

### Adjust the model to your needs

- Change the enum Model ont ".../test/benchmark/main.cpp" replacing by one of availables "STATIC_CALORIC_DEVIATION", "STATIC_MEASLTIME_CALORIC_DEVIATION", "DINAMIC_ONLY_CALORIC_DEVIATION" and "DINAMIC_PREFERENCES_PREDICTION_CALORIC_DEVIATION".

### static_caloric_deviation

### static_mealstime_caloric_deviation

### dinamic_only_caloric_deviation

### dinamic_preferences_prediction_caloric_deviation

## Nutrition Facts

## Solver algorithm 

### Genetic

## Coming soon
- Script to automatically adjust the template for new projects
