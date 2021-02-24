#include <iostream>
#include "experiment.h"


int main() {
    /// Size of the problem equals the amount of recipes used in the instance
    const size_t problem_size = 568;

    /// Setting parameters
    /* Insert the numeral that indicates the model to be used
    set "0": static model
    set "1": static model with meals time division and calories restriction
    set "2": dinamic model without preferences considering
    set "3": dinamic model with preferences prediction considering */
    enum Model {
        STATIC,
        STATIC_MEASLTIME,
        DINAMIC_ONLY,
        DINAMIC_PREFERENCES_PREDICTION,
        //DINAMIC_COST_MINIMIZATION_MODEL
    };

    /// Full instance path of foods
    //Instance PATH is defined in CMAKE as LOCAL_PATH

    /// Output file saved on ...ProjectName/cmake-build-debug/test/benchmark
    std::string file_output = "experiments_result_1";

    /// Solving the problem
    run_experiment(problem_size, STATIC, file_output);

    return 0;
}