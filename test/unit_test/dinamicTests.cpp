
#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "dinamic_optimization.h"
#include "vector"

//creates dynamic object to test its methods
iteration::Dinamic dinamictest;

// Test 1 (true if diets are different)
std::vector<int> _idsInput = {1,2,3,4,5,6};
std::vector<int> _portionsInput = {1,1,2,1,1,3};

std::vector<int> _proposedId = {1,2,3,4,5,6};
std::vector<int> _proposedPortion = {1,2,2,1,1,3};

TEST_CASE( "Compare Menus", "[single-file]" ) {
    dinamictest.saveCurrentMenu(_proposedId, _proposedPortion);
    REQUIRE(dinamictest.compareMealstimeItens(_idsInput, _portionsInput, 0) == true);
}

// Test 2 (calculates mealstime difference)
std::ofstream fout;
std::vector<int> _inputs = {1,2,3};
std::vector<int> _portions = {1,2,1};
std::vector<double> _calories = {50,10,25};
int revanueCaloriesTarget = 200;
int mealtimeShift = 3;

TEST_CASE( "Caloric difference calculation", "[single-file]" ) {
    REQUIRE(dinamictest.caloricDiferenceCalculation(
                mealtimeShift, fout, _inputs, _portions, _calories,
                revanueCaloriesTarget) == -105);
}

// Test 3 (Calculates remaining meals (passed))
int currentMelatimeShift = 3;
int numberOfMealstime = 6;

TEST_CASE( "Calculates remaining meals") {
    REQUIRE(dinamictest.measltimeLeftCalc(currentMelatimeShift,
                                          numberOfMealstime) == 2 );
}

// Test 4 (Calculates remaining meals (fail))
double caloricDiference = 90;
int mealstimeLeft = 2;

TEST_CASE("Calculates remaining meals 2") {
    REQUIRE(dinamictest.applyCaloriesBalance(caloricDiference, mealstimeLeft, fout) != 45 );
}

// Test 5 (ENABLES / DISABLES the compensatory interactive system)
TEST_CASE("ENABLES / DISABLES the compensatory interactive system"){

    dinamictest.setCaloriesTargetChangeFlag(true);

    REQUIRE(dinamictest.getCaloriesTargetChangeFlag() == true);

    SECTION("Desativa"){
        dinamictest.setCaloriesTargetChangeFlag(false);

        REQUIRE(dinamictest.getCaloriesTargetChangeFlag() == false);
    }
}

// Test 6 (Stores food consumed by the user at different times)
TEST_CASE("Stores food consumed by the user at different times"){

    dinamictest.clearEntireMatrix();

    std::vector<int> _current_mealstime_solution = {2,3,5};
    std::vector<int> _current_measltimePortions_solution = {1,1,1};

    size_t mt = dinamictest.getMatrixIDs().size();
    dinamictest.saveCurrentMenu(_current_mealstime_solution,
                                _current_measltimePortions_solution);

    REQUIRE(dinamictest.getMatrixIDs().size() == mt+1);
    REQUIRE(dinamictest.getMatrixPortions().size() == mt+1);

    SECTION("Inserting the next meal"){
        _current_mealstime_solution = {6,7};
        _current_measltimePortions_solution = {3,4};

        mt = dinamictest.getMatrixIDs().size();
        dinamictest.saveCurrentMenu(_current_mealstime_solution,
                                    _current_measltimePortions_solution);

        REQUIRE(dinamictest.getMatrixIDs().size() == mt+1);
        REQUIRE(dinamictest.getMatrixPortions().size() == mt+1);

        std::vector<std::vector<int>> _matrixIDTest =
            dinamictest.getMatrixIDs();
        std::vector<std::vector<int>> _matrixPortionsTest =
            dinamictest.getMatrixPortions();

        REQUIRE(_matrixIDTest[0][2] == 5);
        REQUIRE(_matrixPortionsTest[0][1] == 1);

        REQUIRE(_matrixIDTest[1][1] == 7);
        REQUIRE(_matrixPortionsTest[1][0] == 3);
    }
}

// Test 7 (Compare recommended diet VS food eaten ())
TEST_CASE("Compare recommended diet VS food eaten"){

    std::vector<int> _proposedID = {3,1,2,5,6};
    std::vector<int> _proposedPortions = {1,1,2,3,1};

    dinamictest.saveCurrentMenu(_proposedID, _proposedPortions);

    // IDs e porçoes diferentes
    std::vector<int> _inputID = {4,5,2};
    std::vector<int> _inputPortions = {1,2,3};

    int horario = 0;

    REQUIRE(dinamictest.compareMealstimeItens(_inputID, _inputPortions, horario) == true);

}

//Test 8 (Calculates the caloric difference correctly)
TEST_CASE("Calculates the caloric difference correctly"){

    std::ofstream fout_test_8;
    std::vector<int> _input = {1,2,3,4};
    std::vector<int> _portions_test_8 = {1,2,1,1};
    std::vector<double> _calories_test_8 = {80,100,20,40};
    int measltime_target = 200;
    int mealtime = 0;

    REQUIRE(dinamictest.caloricDiferenceCalculation(mealtime, fout_test_8, _input, _portions_test_8,
                _calories_test_8,
                                                    measltime_target) == 140);

    SECTION("Calculates quantity of remaining meals"){
        int currentMealtime = 3;
        int numberOfRecipes = 6;

        REQUIRE(dinamictest.measltimeLeftCalc(currentMealtime,
                                              numberOfRecipes) == 2 );
        }


    SECTION("Distribution of caloric difference"){
        double caloric_diference_test_8 = 90;
        int leftRecipes = 2;
        std::ofstream fout_test_8_2;

        REQUIRE(dinamictest.applyCaloriesBalance(caloric_diference_test_8, leftRecipes, fout) == -45);

        }
}

//Test 9 (Validation of getters and setters)
TEST_CASE("Validation of getters and setters"){

    std::vector<int> _input = {1,2,3};
    std::vector<int> _iputPortions = {1,1,2};

    dinamictest.setCurrentSolution(_input);
    dinamictest.setCurrentMealtimePortionSolution(_iputPortions);

    SECTION("Assign current solution and its portions"){
        REQUIRE(dinamictest.getCurrentSolution() == _input);
        REQUIRE(dinamictest.getCurrentPortions() == _iputPortions);
    }

    SECTION("Assigns / receives heat difference"){
        dinamictest.setCaloricDiference(72);
        REQUIRE(dinamictest.getCaloricDiference() == 72);
    }

    SECTION("Assigns / receives the remaining meals"){
        dinamictest.setMeasltimeLeft(4);
        REQUIRE(dinamictest.getMeasltimesLeft() == 4);
    }

    SECTION("Assign / receive calculated differences"){
        dinamictest.setDiferenceForNextMeasltimes(36);
        REQUIRE(dinamictest.getDiferenceForNextMeasltimes() == 36);
    }
}