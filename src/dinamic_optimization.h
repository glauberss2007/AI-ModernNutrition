#ifndef NUTRITION_DINAMICO_H
#define NUTRITION_DINAMICO_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

namespace iteration {
class Dinamic {
  public:
    // compares the meal and portions eaten with that recommended
    bool compareMealstimeItens(std::vector<int> _IDInput,
                               std::vector<int> _localPortionsInput, int mealtime);

    // calculates the number of meals still available
    int measltimeLeftCalc(int currentMealtime, int numberOfRecipes);

    // calculates caloric difference between the ingested and recommended menu
    double caloricDiferenceCalculation(int mealtime, std::ofstream &fout,
                                std::vector<int> _localIdInput,
                                std::vector<int> _portionInput,
                                std::vector<double> _caloriesInput,
                                double mealtimeCaloriesTarget);

    // print inputed itens
    void print_user_input(std::vector<int> _localIdInput,
                            std::vector<int> _portionInput,
                            std::ofstream &fout);

    // redistributes the caloric difference
    double applyCaloriesBalance(double localCaloricDiference,
                                int leftRecipes,
                                std::ofstream &fout);

    // releases the vector of entries
    void clearInputs();

    // removes the next recommended diets
    void clearMatrix(int horario);

    // stores the recomendation menu
    void saveCurrentMenu(const std::vector<int>& _solucao_horario_atual,
                         const std::vector<int>& _solucaoPorcao_horario_atual);

    // receives user input from external device
    void userItemInput(int mealtime);

    // receives user input from established parameters
    void parameter_input(int mealtime,size_t size);

    // prints the diet on the screen
    void printScreenMenu(int numberOfRecipies, std::vector<int> _mealstimeHour,
                         std::vector<std::string> _recipe, int hour);

    // prints the diet in an external text file
    void printFileMenu(int numberOfMeals,
                       std::vector<int> _mealstimes,
                       std::vector<std::string> _recipes,
                       int horarioAtual, std::ofstream &fout);

    // Removes all recommended day menu
    void clearEntireMatrix();

    // Setters e getters
    bool getCaloriesTargetChangeFlag();
    int getMeasltimesLeft();
    double getCaloricDiference();
    double getDiferenceForNextMeasltimes();
    double getCurrentBalance();
    std::vector<int> getCurrentSolution();
    std::vector<int> getCurrentPortions();
    std::vector<int> getInputID();
    std::vector<int> getInputPortions();
    std::vector<std::vector<int>> getMatrixIDs();
    std::vector<std::vector<int>> getMatrixPortions();
    void setMeasltimeLeft(int measltimeLeft);
    void setDiferenceForNextMeasltimes(double diferencePerLeftMeasltime);
    void setCaloricDiference(double diferenceResult);
    void setCaloriesTargetChangeFlag(bool m);
    void setCurrentSolution(std::vector<int> _currentSolution);
    void setCurrentMealtimePortionSolution(std::vector<int> _currentPortionSolution);
    void setNextMealsNewCaloriesTarget(double calculated);

  private:
    double caloricDiference;
    std::vector<int> _portionsInput;
    bool mudou;
    int mealstimesLeft;
    double currentBalanced;
    double diferenceForNextMealstime;
    std::vector<int> _solution_current_mealtime;
    std::vector<int> _solution_portion_current_mealtime;

    std::vector<int> _idInput;
    std::vector<std::vector<int>> _matrixIDs;
    std::vector<std::vector<int>> _matrixPorcoes;
};

}
#endif // NUTRITION_DINAMICO_H
