#include <iostream>

#include "static_model.h"
#include "static_mealstime_model.h"
#include "dinamic_preferences_prediction_model.h"
#include "dinamic_model.h"

void run_experiment(size_t problem_size, int model, std::string& file_output) {

    // check the model to be used
    switch (model) {
        /// static model
        case 0: {
            run_static_model(file_output);
        } break;

        /// Meals-time model
        case 1: {
            run_static_mealstime_model(file_output);
        } break;

        /// Dinamic-model
        case 2: {
            run_dinamic_model(file_output);
        } break;

        /// Dinamic-preferences-prediction
        case 3: {
            run_dinamic_preferences_prediction_model(problem_size, file_output);
        } break;
        default : {std::cout << "Modelo nao encontrado!";}
    }
}