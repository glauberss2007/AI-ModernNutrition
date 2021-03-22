#include "dinamic_optimization.h"
#include <chrono>
#include <iostream>
#include <string>
#include <utility>

void iteration::Dinamic::setCaloriesTargetChangeFlag(bool m) { this->mudou = m; }

bool iteration::Dinamic::getCaloriesTargetChangeFlag() { return this->mudou; }
void iteration::Dinamic::clearEntireMatrix() {
    this->_matrixIDs.clear();
    this->_matrixPorcoes.clear();
}

void iteration::Dinamic::clearMatrix(int horario) {
    for (int i = 5; i > horario; i--) {
        this->_matrixIDs.pop_back();
        this->_matrixPorcoes.pop_back();
    }
}

void iteration::Dinamic::saveCurrentMenu(const std::vector<int>& _solucao_horario_atual,
                              const std::vector<int>& _solucaoPorcao_horario_atual) {
    this->_matrixIDs.push_back(_solucao_horario_atual);
    this->_matrixPorcoes.push_back(_solucaoPorcao_horario_atual);
}

void iteration::Dinamic::print_user_input(std::vector<int> _localIdInput,
                        std::vector<int> _portionInput, std::ofstream &fout){

    fout << "User inputs:" << std::endl;
    for (int i = 0; i < _localIdInput.size(); i++) {
        fout << "ID:" << _localIdInput[i] << ", Portion:" << _portionInput[i] << std::endl;
    };
}

std::vector<std::vector<int>> iteration::Dinamic::getMatrixIDs() {
    return this->_matrixIDs;
}

std::vector<std::vector<int>> iteration::Dinamic::getMatrixPortions() {
    return this->_matrixPorcoes;
}

void iteration::Dinamic::printScreenMenu(int numberOfRecipies,
                              std::vector<int> _mealstimeHour,
                              std::vector<std::string> _recipe,
                              int hour) {
    for (int i = hour; i < numberOfRecipies; i++) {
        std::cout << "Horario: " << _mealstimeHour[i] << std::endl;
        for (unsigned long j = 0; j < this->_matrixPorcoes[i].size(); j++) {
            std::cout << this->_matrixPorcoes[i][j] << " porcoes do alimento "
                      << _recipe[this->_matrixIDs[i][j] - 1] << " de ID"
                      << this->_matrixIDs[i][j];
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
void iteration::Dinamic::printFileMenu(int numberOfMeals,
                            std::vector<int> _mealstimes,
                            std::vector<std::string> _recipes,
                            int horarioAtual, std::ofstream &fout) {
    for (int i = horarioAtual; i < numberOfMeals; i++) {
        fout << "Horario: " << _mealstimes[i] << std::endl;
        for (unsigned long j = 0; j < this->_matrixPorcoes[i].size(); j++) {
            fout << this->_matrixPorcoes[i][j] << " porcoes do alimento "
                 << _recipes[this->_matrixIDs[i][j] - 1] << " de ID"
                 << this->_matrixIDs[i][j];
            fout << std::endl;
        }
        fout << std::endl;
    }
}

//
void iteration::Dinamic::parameter_input(int mealtime,size_t size){
    ///Simulate user input according mealtime for dinamic experiments purpose
    switch (mealtime) {
        case 0: {
            // distinct food
            int number_of_distinct_food = ((rand()%2)+1);
            if(number_of_distinct_food == 0){
                break;
            }else{
                for (int i = 0; i < number_of_distinct_food; i++) {
                    // ingested food id
                    int food_id = (rand() % size);
                    // portions of each food
                    int portions_of_same_food = ((rand()% 2) + 1);
                    // save chosed values
                    this->_idInput.push_back(food_id);
                    this->_portionsInput.push_back(portions_of_same_food);
                }
            }
        } break;
        case 1: {
            // distinct food
            int number_of_distinct_food = ((rand()%1)+1);
            if(number_of_distinct_food == 0){
                break;
            }else{
                for (int i = 0; i < number_of_distinct_food; i++) {
                    // ingested food id
                    int food_id = (rand() % size);
                    // portions of each food
                    int portions_of_same_food = ((rand() % 2)+1);
                    // save chosed values
                    this->_idInput.push_back(food_id);
                    this->_portionsInput.push_back(portions_of_same_food);
                }
            }
        } break;
        case 2: {
            // distinct food
            int number_of_distinct_food = ((rand()%2)+1);
            if(number_of_distinct_food == 0){
                break;
            }else{
                for (int i = 0; i < number_of_distinct_food; i++) {
                    // ingested food id
                    int food_id = (rand() % size);
                    // portions of each food
                    int portions_of_same_food = ((rand() % 2)+1);
                    // save chosed values
                    this->_idInput.push_back(food_id);
                    this->_portionsInput.push_back(portions_of_same_food);
                }
            }
        } break;
        case 3: {
            // distinct food
            int number_of_distinct_food = ((rand()%1)+1);
            if(number_of_distinct_food == 0){
                break;
            }else{
                for (int i = 0; i < number_of_distinct_food; i++) {
                    // ingested food id
                    int food_id = (rand() % size);
                    // portions of each food
                    int portions_of_same_food = ((rand() % 2)+1);
                    // save chosed values
                    this->_idInput.push_back(food_id);
                    this->_portionsInput.push_back(portions_of_same_food);
                }
            }
        } break;
        case 4: {
            // distinct food
            int number_of_distinct_food = ((rand()%2)+1);
            if(number_of_distinct_food == 0){
                break;
            }else{
                for (int i = 0; i < number_of_distinct_food; i++) {
                    // ingested food id
                    int food_id = (rand() % size);
                    // portions of each food
                    int portions_of_same_food = ((rand() % 2)+1);
                    // save chosed values
                    this->_idInput.push_back(food_id);
                    this->_portionsInput.push_back(portions_of_same_food);
                }
            }
        } break;
        case 5: {
            // distinct food
            int number_of_distinct_food = ((rand()%1)+1);
            if(number_of_distinct_food == 0){
                break;
            }else{
                for (int i = 0; i < number_of_distinct_food; i++) {
                    // ingested food id
                    int food_id = (rand() % size);
                    // portions of each food
                    int portions_of_same_food = ((rand() % 2)+1);
                    // save chosed values
                    this->_idInput.push_back(food_id);
                    this->_portionsInput.push_back(portions_of_same_food);
                }
            }
        } break;
    }
}

void iteration::Dinamic::userItemInput(int mealtime) {
    int aux;
    std::cout << "Entre com a quantidade de alimentos ingeridos as " << mealtime
              << " horas." << std::endl;
    std::cin >> aux;

    for (int i = 0; i < aux; i++) {
        int numberOfPortions = 0;
        int idItem = 0;

        std::cout << "Insira o ID do " << i + 1
                  << "º alimento ingerido:" << std::endl;
        std::cin >> idItem;
        std::cout << std::endl;

        std::cout << "Insira a quantidade do alimento de ID " << idItem
                  << " ingerido:" << std::endl;
        std::cin >> numberOfPortions;
        std::cout << std::endl;

        this->_portionsInput.push_back(numberOfPortions);
        this->_idInput.push_back(idItem);
    }
}

bool iteration::Dinamic::compareMealstimeItens(std::vector<int> _IDInput,
                                    std::vector<int> _localPortionsInput, int mealtime) {

    bool resultIDs = std::is_permutation(this->_matrixIDs[mealtime].begin(),
                                         this->_matrixIDs[mealtime].end(),
                                         _IDInput.begin());

    bool resultPorcoes = std::is_permutation(
            this->_matrixPorcoes[mealtime].begin(),
            this->_matrixPorcoes[mealtime].end(), _localPortionsInput.begin());

    std::cout << "Teste compara dieta 1" << std::endl;
    if (resultIDs && resultPorcoes) {
        for (unsigned long i = 0; i < _IDInput.size(); i++) {
            for (unsigned long j = 0; j < _matrixIDs[mealtime].size(); j++) {
                if (_IDInput[i] == _matrixIDs[mealtime][j]) {
                    if (_localPortionsInput[i] != _matrixPorcoes[mealtime][j])
                        return true;
                }
            }
        }
        return false;
    } else
        return true;
}

void iteration::Dinamic::setNextMealsNewCaloriesTarget(double calculated) { this->currentBalanced = calculated; }

double iteration::Dinamic::getCurrentBalance() { return this->currentBalanced; }

double iteration::Dinamic::caloricDiferenceCalculation(int mealtime, std::ofstream &fout,
                                     std::vector<int> _localIdInput,
                                     std::vector<int> _portionInput,
                                     std::vector<double> _caloriesInput,
                                     double mealtimeCaloriesTarget) {
    std::cout << "Os alimentos ou quantidades ingeridos na refeiçao das "
              << mealtime
              << " horas foi diferente do plano alimentar atual. Seu "
                 "planejamento para as proximas refeiçoes serao recalculados."
              << std::endl;
    std::cout << std::endl;

    fout << "Os alimentos ou quantidades ingeridos na refeiçao das " << mealtime
         << " horas foi diferente do plano alimentar atual. Seu planejamento "
            "para as proximas refeiçoes serao recalculados."
         << std::endl;
    fout << std::endl;

    double inputCalories = 0;
    double caloriesDiference;
    for (unsigned long i = 0; i < _localIdInput.size(); i++) {
        inputCalories =
                inputCalories + (_portionInput[i] * _caloriesInput[_localIdInput[i] - 1]);
    }

    // Calcula a diferenca
    caloriesDiference = inputCalories - mealtimeCaloriesTarget;

    std::cout << "A diferença calorica e de " << caloriesDiference << ","
              << std::endl;

    return caloriesDiference;
}

void iteration::Dinamic::clearInputs() {
    this->_idInput.clear();
    this->_portionsInput.clear();
}

int iteration::Dinamic::measltimeLeftCalc(int currentMealtime,
                               int numberOfRecipes) {
    return (numberOfRecipes - (currentMealtime + 1));
}

double iteration::Dinamic::applyCaloriesBalance(double localCaloricDiference,
                                     int leftRecipes,
                                     std::ofstream &fout) {
    double caloricBalanceValue = localCaloricDiference / leftRecipes;

    std::cout << "Suas demais refeiçoes serao alteradas em  " << -caloricBalanceValue
              << " calorias." << std::endl;

    fout << "Suas demais refeiçoes serao alteradas em  " << -caloricBalanceValue
         << " calorias." << std::endl;

    return -caloricBalanceValue;
}

void iteration::Dinamic::setCurrentSolution(std::vector<int> _currentSolution) {
    this->_solution_current_mealtime = std::move(_currentSolution);
}

void iteration::Dinamic::setCurrentMealtimePortionSolution(
    std::vector<int> _currentPortionSolution) {
    this->_solution_portion_current_mealtime = std::move(_currentPortionSolution);
}

std::vector<int> iteration::Dinamic::getCurrentSolution() {
    return this->_solution_current_mealtime;
}

std::vector<int> iteration::Dinamic::getCurrentPortions() {
    return this->_solution_portion_current_mealtime;
}

std::vector<int> iteration::Dinamic::getInputID() { return this->_idInput; }

std::vector<int> iteration::Dinamic::getInputPortions() {
    return this->_portionsInput;
}

double iteration::Dinamic::getCaloricDiference() { return this->caloricDiference; }

void iteration::Dinamic::setCaloricDiference(double diferenceResult) {
    this->caloricDiference = diferenceResult;
}

void iteration::Dinamic::setMeasltimeLeft(int measltimeLeft) {
    this->mealstimesLeft = measltimeLeft;
}

double iteration::Dinamic::getDiferenceForNextMeasltimes() {
    return this->diferenceForNextMealstime;
}

void iteration::Dinamic::setDiferenceForNextMeasltimes(
    double diferencePerLeftMeasltime) {
    this->diferenceForNextMealstime = diferencePerLeftMeasltime;
}

int iteration::Dinamic::getMeasltimesLeft() { return this->mealstimesLeft; }
