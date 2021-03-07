[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/MacOS/badge.svg)](https://github.com/glauberss2007/modern-nutrition/actions)
[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/Windows/badge.svg)](https://github.com/glauberss2007/modern-nutrition/actions/actions)
[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/Ubuntu/badge.svg)](https://github.com/glauberss2007/modern-nutrition/actions/actions)

# Modern Nutrition

Since the classic diet planning problem, several mathematical models and programming techniques have been shown to generate menus following several restrictions. However, a new trend not yet addressed is behavioral nutrition. This scientific approach, focused on nutritional counseling, proves to be more effective than the common restrictive diets, by adapting to the habits and preferences of users. Therefore, aiming to contribute to a non-restrictive and intuitive diet, in this project we propose a new dinamic optimization model considering preferences predictions.

<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/model-fluxogram.png" height="350" width="auto" />
</p>

## Features

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
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/caloric-deviation.png" height="60" width="auto" />
</p>

- nutritional deviation values:
<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/nutritional-table.png" height="160" width="auto" />
</p>

- nutrition facts separated by categories with diferent portion size:
<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/taco-food-categories.png" height="200" width="auto" />
</p>

- penalties applied according to nutritional deviations intensity:
<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/penalties-static-model.png" height="90" width="auto" />
</p>

- result: a list of foods to be eat freely during the day.

### static_mealstime_caloric_deviation

- separate dayle recomendation menu into 6 diferents mealtimes:
<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/mealstime-division.png" height="150" width="auto" />
</p>

- penalties formulation applied according to nutritional deviations proportionally to mealstime (the penalties are applied according to mealstime caloric target inporcentage of total):

<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/mealstime-penalties.png" height="100" width="auto" />
</p>

- result: a list of food considering mealtimes proportionaly (caloric and nutrients).

### dinamic_only_caloric_deviation

- user interations after each mealstime inputing.caloric deviation consume
     diference of user consume input and mealstime menu recomended calculation:
<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/caloric-diference-calculation.png" height="60" width="auto" />
</p>

- compensatory system to equilibrate/recalculate further mealstimes considering current user consume:
<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/compensatory-calculation.png" height="60" width="auto" />
</p>

- result: dinamic menu that changes/recalculate according to user input on each mealstime

### dinamic_preferences_prediction_caloric_deviation

- prediciton instance (consume history database) based on mealstime category preferences
    instance generation rule:
<p align="center">
  <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/prediction-instance.png" height="110" width="auto" />
</p>
PS: The category preference was used due a limitation provided due no real consume database availability actually. It is necessary to create a real food consume per mealstime database in ten future...
    
- item-item score matrix created using colaborative filtering

    magnitude calculation:
    <p align="center">
    <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/prediction-magnitude.png" height="60" width="auto" />
    </p>
    
    item importance:
    <p align="center">
    <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/food-importance.png" height="60" width="auto" />
    </p>
    
    cossine similarity calculation:
    <p align="center">
    <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/prediction-similarity-cosseno-calculation.png" height="90" width="auto" />
    </p>
    
    final vector score of itens in a specific mealstime for an user:
    <p align="center">
    <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/food-score-based-on-item-mealstime-user.png" height="80" width="auto" />
    </p>

- first-fit algorithym to include high scored itens limited on 50% of mealstime caloric target.

### dinamic_preferences_prediction_cost_minimization

- populate TACO DB with prices based on ENAPE.
- (on going) change the objective function to maxmizing and avaliation function to cost minimization + nutritional deviation:
<p align="center">
    <img src="https://github.com/glauberss2007/modern-nutrition/blob/master/.github/workflows/images/cost-fitness-function.png" height="60" width="auto" />
    </p>

- caloric value as a nutrient range like others using 1600-2400 for womens and 2000-3000 for mens.

## Solver algorithm 

### Genetic
- initial random population
- pattern select scale/strategy: windows/tournament
- child select scale/strategy: windows/tournament
- mutation type: flip reseting
- mutation_strenght: 0.4
- crossover_probability: 0.6

## Coming soon
- easy database change using cereal lib for txt/csv nutrition facts serealize or sql DB conection for MYSQL ralational db query
- dinamic minimization cost considering
- diferents metaheuristcs solvers

## References

- Dissertation available on http:// (on going) 
