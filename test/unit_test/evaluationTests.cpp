//
// Created by glaub on 13/12/2020.
//
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "user_diet.h"
#include "vector"

/// object used for test
std::vector<int> _measltime = {6,10,12,15,18,21};
nutrition_facts test_problem(568, 2000,
                        _measltime);

user_diet test_diet(test_problem);

// Test 1
TEST_CASE( "Caloric deviation calculation") {

    REQUIRE(test_diet.deviation_calc(100,50) == 7500);
}

//test 2
TEST_CASE( "Proportional penaltyes calculation") {
    double nutritional_test_obtained =20;
    double nutritional_test_target = 30;
    double nutritional_limit_exceds = nutritional_test_target - nutritional_test_obtained;
    double proportionality_for_current_meal = 1;
    double penalty_factor = 1000;

    REQUIRE(test_diet.penalty_calculation(nutritional_limit_exceds,nutritional_test_target,
                                          proportionality_for_current_meal,penalty_factor) == Approx(333.3333333333));
}

//test 3
TEST_CASE( "Preferences inclusion limit") {

    // Breakfast
    test_diet.setPortion(4,0);
    test_problem.setTurn(1);
    std::vector<double> _test_user_score = {0,0,0,4};
    test_diet.preferences_inclusion_limit(test_problem);
    REQUIRE(test_diet.getPortion(4) == 0);

    // Snack 1
    _test_user_score = {0,0,0,4};
    test_problem.setTurn(2);
    test_problem.setSnack1Preferences(_test_user_score);
    test_diet.preferences_inclusion_limit(test_problem);
    REQUIRE(test_diet.getPortion(4) > 0);

    // Lunch
    _test_user_score = {0,0,0,4,0,5};
    test_diet.setPortion(4,0);
    test_diet.setPortion(6,0);
    test_problem.setTurn(3);
    test_problem.setLunchPreferences(_test_user_score);
    test_diet.preferences_inclusion_limit(test_problem);
    REQUIRE(test_diet.getPortion(4) > 0);
    REQUIRE(test_diet.getPortion(6) > 0);

    // Snack 2
    _test_user_score = {0,0,0,4,0,5};
    test_diet.setPortion(4,0);
    test_diet.setPortion(6,0);
    test_problem.setTurn(4);
    test_problem.setSnack2Preferences(_test_user_score);
    test_diet.preferences_inclusion_limit(test_problem);
    REQUIRE(test_diet.getPortion(6) > 0);
    REQUIRE(test_diet.getPortion(4) == 0);

    // Dinner
    _test_user_score = {0,2,0,4,0,5};
    test_diet.setPortion(4,0);
    test_diet.setPortion(6,0);
    test_diet.setPortion(2,0);
    test_problem.setTurn(5);
    test_problem.setDinnerPreferences(_test_user_score);
    test_diet.preferences_inclusion_limit(test_problem);
    REQUIRE(test_diet.getPortion(6) > 0);
    REQUIRE(test_diet.getPortion(4) > 0);
    REQUIRE(test_diet.getPortion(2) == 0);

    //Supper
    _test_user_score = {0,0,0,4,0,5};
    test_diet.setPortion(4,0);
    test_diet.setPortion(6,0);
    test_problem.setTurn(6);
    test_problem.setSupperPreferences(_test_user_score);
    test_diet.preferences_inclusion_limit(test_problem);
    REQUIRE(test_diet.getPortion(6) > 0);
    REQUIRE(test_diet.getPortion(4) == 0);

}