//
// Created by glauber on 09/06/2020.
//
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "user_preferences.h"

predictions::user_preferences testUser;

//tests about user information entries
// Test 1
TEST_CASE( "User input", "[set/get]" ) {
    testUser.setName("Alice");
    std::string str = "Alice";

    CHECK_THAT(testUser.getName(), Catch::Matchers::Equals(str));
}

// Test 2
TEST_CASE( "Age input.", "[set/get]" ) {
    testUser.setAge(21);

    REQUIRE(testUser.getAge() == 21);
}

// Test 3
TEST_CASE( "Weight input.", "[set/get]" ) {
    testUser.setWeight(56);

    REQUIRE(testUser.getWeight() == 56);
}

// Teste 4
TEST_CASE( "High input.", "[set/ger]" ) {
    testUser.setHigh(1.60);

    REQUIRE(testUser.getHigh() == Approx(1.60));
}

// Test 5
TEST_CASE( "Energetic burn input.", "[set/get]" ) {
    testUser.setCaloriesBurn(3);

    REQUIRE(testUser.getCaloriesBurn() == 3);
}

// Test 6
TEST_CASE( "Hobbyes input.", "[set/get]" ) {
    std::vector<std::string> _teste = {"Maravilharce","Passear","Cantar"};
    testUser.setHobbyes(_teste);

    REQUIRE(_teste == testUser.getHobbies());
    REQUIRE(_teste[1] == testUser.getHobbie(1));
}

// Test 7
TEST_CASE( "Preferences test.", "[set/get]" ) {
    std::vector<int> _teste1 = {1,2};
    std::vector<int> _teste2 = {1,2,3};
    std::vector<int> _teste3 = {1,2,3,4};
    std::vector<int> _teste4 = {1,2,3,4,5};
    std::vector<int> _teste5 = {1,2,3,4,5,6};
    std::vector<int> _teste6 = {1,2,3,4,5,6,7};

    std::vector<int> _testePortions1 = {1,2};
    std::vector<int> _testePortions2 = {1,2,3};
    std::vector<int> _testePortions3 = {1,2,3,4};
    std::vector<int> _testePortions4 = {1,2,3,4,5};
    std::vector<int> _testePortions5 = {1,2,3,4,5,6};
    std::vector<int> _testePortions6 = {1,2,3,4,5,6,7};


    SECTION("Breakfast preferences.", "[set/get]"){
        testUser.setBreakfastPreferences(_teste1);
        testUser.setBreakfastPreferencesPortions(_testePortions1);
        REQUIRE(_teste1 == testUser.getBreakfastPreferences());
        REQUIRE(_testePortions1 == testUser.getBreakfastPreferencesPortions());
    }

    SECTION("Snack 1 preferences.", "[set/ger]"){
        testUser.setSnack1Preferences(_teste2);
        testUser.setSnack1PortionPreferences(_testePortions2);
        REQUIRE(_teste2 == testUser.getSnack1Preferences());
        REQUIRE(_testePortions2 == testUser.getSnack1PortionPreferences());
    }

    SECTION("Lunch preferences", "[set/ger]"){
        testUser.setLunchPreferences(_teste3);
        testUser.setLunchPortionsPreferences(_testePortions3);
        REQUIRE(_teste3 == testUser.getLunchPreferences());
        REQUIRE(_testePortions3 == testUser.getLunchPortionsPreferences());
    }

    SECTION("Snack 2 preferences", "[set/ger]"){
        testUser.setSnack2Preferences(_teste4);
        testUser.setSnack2PreferencesPortions(_testePortions4);
        REQUIRE(_teste4 == testUser.getSnack2Preferences());
        REQUIRE(_testePortions4 == testUser.getSnack2PreferencesPortions());
    }

    SECTION("Dinner preferences", "[set/ger]"){
        testUser.setDinnerPreferences(_teste5);
        testUser.setDinnerPreferencesPortions(_testePortions5);
        REQUIRE(_teste5 == testUser.getDinnerPreferences());
        REQUIRE(_testePortions5 == testUser.getDinnerPreferencesPortions());
    }

    SECTION("Supper preferences", "[set/ger]"){
        testUser.setSupperPreferences(_teste6);
        testUser.setSupperPreferencesPortions(_testePortions6);
        REQUIRE(_teste6 == testUser.getPreferenciasCeia());
        REQUIRE(_testePortions6 == testUser.getPorcaoPreferenciasCeia());
    }
}

// Test 8
TEST_CASE( "Food input", "[set/get]" ) {
    std::vector<int> _mealstimeTest = {9,13,16,19,21,23};
    testUser.setMealsTime(_mealstimeTest);

    REQUIRE(testUser.getMealsTime() == _mealstimeTest);
    REQUIRE(testUser.getMealsTime() == _mealstimeTest);
    REQUIRE(testUser.getMealstimeHour(2) == 16);
    REQUIRE(testUser.getNumberOfMealsTime() == 6);
}

// Test 9
TEST_CASE( "Calories target", "[set/get]" ) {

    testUser.SetCaloriesTarget(2000);
    REQUIRE(testUser.getTarget() == 2000);

    SECTION("Alteraçao da caloriesTarget"){
        int novaMeta = 2300;
        testUser.SetCaloriesTarget(novaMeta);
        REQUIRE(testUser.getTarget() == 2300);
    }
}

// Test 10
TEST_CASE( "Gender input", "[set/get]" ) {

    testUser.setGender(0);
    REQUIRE(testUser.getGender() == 0);

    SECTION("Gender change"){
        testUser.setGender(1);
        REQUIRE(testUser.getGender() == 1);
    }

    SECTION("IMC Calc."){
        testUser.setHigh(181);
        testUser.setWeight(100);

        REQUIRE(testUser.getWeight() == 100);
        REQUIRE(testUser.getHigh() == 181);

        REQUIRE(testUser.getImc() == Approx(30.5241));
    }
}

// Test 11
TEST_CASE("Item based preferences predictions"){
    //The test instance validates the calculations based on the spreadsheet template at:
    //https://docs.google.com/spreadsheets/d/1P3JleD8_4ITPVdFnjgkv8XkKTgNmXDiCR_HrOY7nOBk/edit?usp=sharing
    std::vector<std::vector<int>> _preferencesMatrixTest = {{1,2,7},{3,6,8,10},{1},{1,5},{1,3,4,10},{1,6,9},{1,4,5,6,10}};
    std::vector<std::vector<int>> _portionPreferencesMatrixTest = {{2,1,1},{2,1,3,2},{1},{3,2},{1,1,1,1},{3,3,2},{1,3,1,1,3}};

    std::vector<std::vector<double>> _matrizRankTest = testUser.normalize(_preferencesMatrixTest, _portionPreferencesMatrixTest);
    std::vector<double> _L2 = testUser.euclidean_distance(_preferencesMatrixTest, _matrizRankTest, 10);
    std::vector<std::vector<double>> _matrixDistanciaItens = testUser.similarity_cosines(_matrizRankTest, _preferencesMatrixTest, _L2, 10);
    std::vector<double> _testScore = testUser.score_calc(_matrizRankTest, _matrixDistanciaItens, _preferencesMatrixTest,
                                                 3, 10);

    SECTION("Euclidean distance calculation"){

        REQUIRE(0.8164965809 == Approx(_matrizRankTest[0][0]));
    }

    SECTION("Magnetude calculation"){
        //Magnetude = Square root of the sum of the squares of all portions and items of that user:
        //=SQRT(SUMSQ(ratings!B2:K2))

        REQUIRE(Approx(0.6871842709) == _L2[2]);
        REQUIRE(Approx(0.5960799907) == _L2[4]);
        REQUIRE(Approx(0.4082482905) == _L2[6]);
    }

    SECTION("Itens distance matrix calculation"){
        //Construction of NxN matrix (itemXitem) each item will have its distance calculated in relation to the other items
        // Method: Calculation of cosine similarity
        //=(SUMPRODUCT(OFFSET('ratings-norm'!$B$2:$B$10,0,ROW()-2),'ratings-norm'!B$2:B$10)) / ('ratings-norm'!B$11 * OFFSET('ratings-norm'!$B$11,0,ROW()-2))
        // Maximum distance = 0 (no relation)
        // Minimum distance = 1 (maximum ratio, ie ratio of the same item)

        REQUIRE(Approx(1) == _matrixDistanciaItens[0][0]);
        REQUIRE(Approx(0) == _matrixDistanciaItens[1][9]);
        REQUIRE(Approx(0.8936394963) == _matrixDistanciaItens[8][5]);
        REQUIRE(Approx(1) == _matrixDistanciaItens[9][9]);
        REQUIRE(Approx(0) == _matrixDistanciaItens[4][7]);
    }

    SECTION("Validates recommendations for user 3"){
        // Calculates the user's score for each list item
        //=(SUMPRODUCT('item-matrix'!$B2:$K2,'ratings-norm'!$B$5:$K$5)) / (SUM('item-matrix'!$B2:$K2))

        REQUIRE(_testScore[0] == Approx(0.285160265));
        REQUIRE(_testScore[1] == Approx(0.1573220359));
        REQUIRE(_testScore[9] == Approx(0.08522737361));
    }

}

// Test 14 (check user with more similare itens)
TEST_CASE("Check user with more similar foods"){
    std::vector<int> _testPreferences = {1,3,5,7,8};
    std::vector<std::vector<int>> _testPreferencesMatrix = {{1},{3,6,8,9},{1},{1,5},{1,3,4,9},{1,6,9},{1,4,5,6,8}};

    int highScoreUser = 0;

    testUser.searchSimilarItensToBreakfast(highScoreUser, _testPreferences,
                                  _testPreferencesMatrix);

    REQUIRE(testUser.searchSimilarItensToBreakfast(highScoreUser, _testPreferences,
                                          _testPreferencesMatrix) == 6);
}