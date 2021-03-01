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

- Change the enum Model (.../test/benchmark/main.cpp) replacing by one of current availables:

### static_caloric_deviation

- nutritional facts based on TACO (http://www.nepa.unicamp.br/taco/contar/taco_4_edicao_ampliada_e_revisada.pdf?arquivo=taco_4_versao_ampliada_e_revisada.pdf)
- objective function: minimize caloric difference against target:
<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/caloric-deviation.png" height="175" width="auto" />
</p>

- penalties applied according to nutritional deviations intensity:
<p align="center">
  <img src="https://repository-images.githubusercontent.com/254842585/4dfa7580-7ffb-11ea-99d0-46b8fe2f4170" height="175" width="auto" />
</p>

- result: a list of foods to be eat freely during the day:
<p align="center">
  <img src="https://repository-images.githubusercontent.com/254842585/4dfa7580-7ffb-11ea-99d0-46b8fe2f4170" height="175" width="auto" />
</p>

### static_mealstime_caloric_deviation

- separate dayle recomendation menu into 6 diferents mealtimes
- penalties applied according to nutritional deviations proportionally to mealstime
- result: a list of food considering mealtimes proportionaly (caloric and nutrients)

### dinamic_only_caloric_deviation

- user interations after each mealstime inputing
- compensatory system to equilibrate caloric recomendation during the day
- result: dinamic menu that changes/recalculate according to user input on each mealstime

### dinamic_preferences_prediction_caloric_deviation

- nutrition facts separated by categories
- prediciton instance (consume history database) based on mealstime category preferences
- item-item score matrix created using colaborative filtering
- first-fit algorithym to include high scored itens limited on 50% of mealstime caloric target

### dinamic_preferences_prediction_cost_minimization

- change avaliation function to cost minimization + nutritional deviation
- caloric value as a nutrient range like others

## Solver algorithm 

### Genetic

- initial random population
- pattern select strategy:
- child select strategy:
- cross-over strategy:
- others parameters:

## Coming soon
- easy database change using cereal lib for txt/csv nutrition facts serealize or sql DB conection for MYSQL ralational db 
- dinamic minimization cost considering
- diferents metaheuristcs solvers

## Latest references used

- avaliation function mathematics formulation:
- penaltyes formualtion:
- nutrition facts:
- predictions:
- dinamic interations:
