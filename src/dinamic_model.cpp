#include "dinamic_model.h"
#include "dinamic_optimization.h"
#include "evolutionary_algorithm.h"
#include "user_diet.h"
#include "user_preferences.h"

void run_dinamic_model(const std::string& file_output) {
    // For experimental purposes we will create user only with the essential information to solve the problem
    predictions::user_preferences u;
    u.setName(file_output);
    u.setMealsTime({6, 10, 12, 15, 18, 21});
    u.SetCaloriesTarget(2000);

    // Prints on screen and file
    u.printScreenUser(u);
    std::ofstream fout(file_output);
    u.printScreenFile(u, fout);

    //Creates the problem considering user data
    nutrition_facts problem(568, u.getTarget(), u.getMealsTime());
    problem.setModelToUse(nutrition_facts::DINAMIC_ONLY);

    // Prints the problem on the screen and in file
    problem.disp();
    problem.dispFile(fout, u.getTarget(), u.getNumberOfMealsTime());

    // Start counting time
    clock_t time_req;
    time_req = clock();

    //Configures the model as static
    problem.setCaloriesTargetChangeFlag(0);

    // Creates dynamic object
    iteration::Dinamic d;

    //Variables used for preferences predictions
    std::vector<std::vector<double>> _snack1MatrixItens,_lunchMatrixItens,
        _snack2MatrixItens,_dinnerMatrixItens,_supperMatrixItens;

    for (int i = 0; i < problem.numberOfMealstime(); i++) {
        // Configures turn and mealstime
        problem.setTurn(i + 1);
        std::cout << "Current turn: " << problem.turn() << std::endl;
        fout << "Current turn: " << problem.turn() << std::endl;

        // CREATES THE SOLVER
        using solver_t = evolutionary_algorithm;
        solver_t solver(problem);
        solver.algorithm();

        // RUNS THE SOLVER
        solver.run();

        // PRINT CURRENT SOLUTION FOR EACH MEALSTIME
        auto iter = solver.best_solution();
        std::cout << std::endl;
        fout << std::endl;
        std::cout << "Mealstime: " << i + 1 << std::endl;
        fout << "Mealstime: " << i + 1 << std::endl;
        std::cout << "Hour: " << problem.getHour(i) << std::endl;
        fout << "Hour: " << problem.getHour(i) << std::endl;
        std::cout << "Solution: " << std::endl;
        fout << "Solution: " << std::endl;
        d.setCurrentSolution((iter)->disp(problem, fout));
        std::cout << "FO: " << (((iter)->fx)) << std::endl;
        fout << "FO: " << (((iter)->fx)) << std::endl;
        std::cout << "....................................................."
                     "......................."
                  << std::endl;
        fout << ".........................................................."
                ".................."
             << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        fout << std::endl;
        fout << std::endl;

        // STORE THE DIET
        d.saveCurrentMenu(d.getCurrentSolution(), d.getCurrentPortions());
    }

    // Print and save to file the IDs of the diets generated and their portions
    int hour = 0;
    d.printScreenMenu(problem.numberOfMealstime(), problem.getHour(),
                      problem.getRecipies(), hour);
    d.printFileMenu(problem.numberOfMealstime(), problem.getHour(),
                    problem.getRecipies(), hour, fout);

    time_req = clock() - time_req;
    std::cout << std::endl;
    std::cout << "Time counter:" << time_req << std::endl;
    fout << std::endl;
    fout << "Time counter:" << time_req << std::endl;

    /// Starts interaction with the user for breakfast
    //receives values ​​directly from the user (Production use)
    //d.userItemInput(problem.getHour(hour));
    //receives values ​​by parameter in a matrix, each vector representing a mealtime (experimental use)
    d.parameter_input(hour, problem.size());
    d.print_user_input(d.getInputID(),d.getInputPortions(),fout);

    // Adds the foods currently eaten by the user as favorites (considering mealstime)
    // Breakfast
    u.setBreakfastPreferences(d.getInputID());
    u.setBreakfastPreferencesPortions(d.getInputPortions());
    // insertMatrizPreferenciasDesjejum(_breakfastPreferences);
    // insertPorcaoMatrizPreferenciasDesjejum(_breakfastPortionPreferences);

    //Compare proposed diet x user preferences (ingested food)
    d.setCaloriesTargetChangeFlag(d.compareMealstimeItens(u.getBreakfastPreferences(),
                                                          u.getBreakfastPreferencesPortions(),hour));
    std::cout << d.getCaloriesTargetChangeFlag() << std::endl;

    // Recalculate next meals if necessary
    if (d.getCaloriesTargetChangeFlag()) {
        d.clearMatrix(hour);

        //Calculates the nutritional difference ingested by the user and compensates in next meals
        // Function regarding equation 1 of article
        d.setCaloricDiference(d.caloricDiferenceCalculation(
            problem.getHour(hour), fout, u.getBreakfastPreferences(),
            u.getBreakfastPreferencesPortions(), problem.getCalories(),
            problem.getBreakfastTarget()));

        // Function regarding equation 2 of the article
        d.setMeasltimeLeft(d.measltimeLeftCalc(
                hour, u.getNumberOfMealsTime()));

        // Function regarding equation 3 of the article
        d.setDiferenceForNextMeasltimes(d.applyCaloriesBalance(
                d.getCaloricDiference(), d.getMeasltimesLeft(), fout));

        // Function regarding equation 4 of the article
        d.setNextMealsNewCaloriesTarget(d.getDiferenceForNextMeasltimes());

        //Applies the caloric difference to next meals changing the objective function goals for each mealstime
        problem.setCaloriesTargetChangeFlag(d.getDiferenceForNextMeasltimes());

        // Start dynamic compensatory cycle
        // Start from next meal
        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            //Configures the shift skipping the meal already made
            problem.setTurn(i + 2);
            std::cout << "Current measltime: " << problem.turn() << std::endl;
            fout << "Current measltime: " << problem.turn() << std::endl;

            // Update solver with the new problem
            using solver_t = evolutionary_algorithm;
            solver_t solver(problem);
            solver.algorithm();

            // Runs solver
            solver.run();

            // Print solution
            auto iter = solver.best_solution();
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Measltime ID: " << i + 2 << std::endl;
            fout << "Measltime ID: " << i + 2 << std::endl;
            std::cout << "Hour: " << problem.getHour(i + 1)
                      << std::endl;
            fout << "Hour: " << problem.getHour(i + 1) << std::endl;
            std::cout << "Solution: " << std::endl;
            fout << "Solution: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
            std::cout << "FO: " << (((iter)->fx)) << std::endl;
            fout << "FO: " << (((iter)->fx)) << std::endl;
            std::cout << "................................................."
                         "..........................."
                      << std::endl;
            fout << "......................................................"
                    "......................"
                 << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            fout << std::endl;
            fout << std::endl;

            // Stores the diet of eat mealstime
            d.saveCurrentMenu(d.getCurrentSolution(),
                              d.getCurrentPortions());
        }

        // Print the new menu
        d.printScreenMenu(problem.numberOfMealstime(), problem.getHour(),
                          problem.getRecipies(), hour);
        d.printFileMenu(problem.numberOfMealstime(),
                        problem.getHour(), problem.getRecipies(),
                        hour, fout);
    }

    // set hour of second meal (hour = 0 is the first meal)
    hour = 1;

    //Clears the vector with the user's old entries
    d.clearInputs();

    //d.userItemInput(problem.getHour(hour));
    d.parameter_input(hour,problem.size());
    d.print_user_input(d.getInputID(),d.getInputPortions(),fout);

    u.setSnack1Preferences(d.getInputID());
    u.setSnack1PortionPreferences(d.getInputPortions());

    d.setCaloriesTargetChangeFlag(d.compareMealstimeItens(u.getSnack1Preferences(),
                                                          u.getSnack1PortionPreferences(),
                                                          hour));

    if (d.getCaloriesTargetChangeFlag()) {
        d.clearMatrix(hour);
        std::cout << "The sugested caloric diference for this meal is "
                  << d.getDiferenceForNextMeasltimes() << std::endl;
        d.setCaloricDiference(d.caloricDiferenceCalculation(
            problem.getHour(hour), fout, u.getSnack1Preferences(),
            u.getSnack1PortionPreferences(), problem.getCalories(),
            (problem.getCaloricTargetSnack1() +
             d.getDiferenceForNextMeasltimes())));
        d.setMeasltimeLeft(d.measltimeLeftCalc(
                hour, u.getNumberOfMealsTime()));
        d.setDiferenceForNextMeasltimes(d.applyCaloriesBalance(
                d.getCaloricDiference(), d.getMeasltimesLeft(), fout));
        d.setNextMealsNewCaloriesTarget(d.getCurrentBalance() +
                                        d.getDiferenceForNextMeasltimes());
        problem.setCaloriesTargetChangeFlag(d.getDiferenceForNextMeasltimes());

        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            problem.setTurn(i + 3);
            std::cout << "Current Mesaltime shift: " << problem.turn() << std::endl;
            fout << "Currente measltime shift: " << problem.turn() << std::endl;

            using solver_t = evolutionary_algorithm;
            solver_t solver(problem);
            solver.algorithm();

            solver.run();

            auto iter = solver.best_solution();
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Measltime shift: " << i + 3 << std::endl;
            fout << "Measltime shift: " << i + 3 << std::endl;
            std::cout << "Measltime hour: " << problem.getHour(i + 2)
                      << std::endl;
            fout << "Measltime hour: " << problem.getHour(i + 2) << std::endl;
            std::cout << "Solution: " << std::endl;
            fout << "Solution: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            std::cout << "FO: " << (((iter)->fx)) << std::endl;
            fout << "FO: " << (((iter)->fx)) << std::endl;
            std::cout << "................................................."
                         "..........................."
                      << std::endl;
            fout << "......................................................"
                    "......................"
                 << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            fout << std::endl;
            fout << std::endl;

            d.saveCurrentMenu(d.getCurrentSolution(),
                              d.getCurrentPortions());
        }
        d.printScreenMenu(problem.numberOfMealstime(), problem.getHour(),
                          problem.getRecipies(), hour);
        d.printFileMenu(problem.numberOfMealstime(),
                        problem.getHour(), problem.getRecipies(),
                        hour, fout);
    }

    hour = 2;

    d.clearInputs();
    //d.userItemInput(problem.getHour(hour));
    d.parameter_input(hour,problem.size());
    d.print_user_input(d.getInputID(),d.getInputPortions(),fout);

    u.setLunchPreferences(d.getInputID());
    u.setLunchPortionsPreferences(d.getInputPortions());

    d.setCaloriesTargetChangeFlag(d.compareMealstimeItens(u.getLunchPreferences(),
                                                          u.getLunchPortionsPreferences(),
                                                          hour));

    if (d.getCaloriesTargetChangeFlag()) {
        d.clearMatrix(hour);
        d.setCaloricDiference(d.caloricDiferenceCalculation(
            problem.getHour(hour), fout, d.getInputID(), d.getInputPortions(),
            problem.getCalories(),
            (problem.getCaloricTargetLunch() + d.getCurrentBalance())));
        std::cout << "Current balance applied: " << d.getCurrentBalance() << std::endl;
        d.setMeasltimeLeft(d.measltimeLeftCalc(
                hour, u.getNumberOfMealsTime()));
        d.setDiferenceForNextMeasltimes(d.applyCaloriesBalance(
                d.getCaloricDiference(), d.getMeasltimesLeft(), fout));
        d.setNextMealsNewCaloriesTarget(d.getCurrentBalance() +
                                        d.getDiferenceForNextMeasltimes());
        problem.setCaloriesTargetChangeFlag(d.getDiferenceForNextMeasltimes());

        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            problem.setTurn(i + 4);
            std::cout << "Current measltime shift: " << problem.turn() << std::endl;
            fout << "Current measltime shift: " << problem.turn() << std::endl;

                        using solver_t = evolutionary_algorithm;
            solver_t solver(problem);
            solver.algorithm();

            solver.run();

            auto iter = solver.best_solution();
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Measltime shift: " << i + 4 << std::endl;
            fout << "Measltime shift: " << i + 4 << std::endl;
            std::cout << "Hour: " << problem.getHour(i + 3)
                      << std::endl;
            fout << "Hour: " << problem.getHour(i + 3) << std::endl;
            std::cout << "Solution: " << std::endl;
            fout << "Solution: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
            std::cout << "FO: " << (((iter)->fx)) << std::endl;
            fout << "FO: " << (((iter)->fx)) << std::endl;
            std::cout << "................................................."
                         "..........................."
                      << std::endl;
            fout << "......................................................"
                    "......................"
                 << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            fout << std::endl;
            fout << std::endl;

            d.saveCurrentMenu(d.getCurrentSolution(),
                              d.getCurrentPortions());
        }
        d.printScreenMenu(problem.numberOfMealstime(), problem.getHour(),
                          problem.getRecipies(), hour);
        d.printFileMenu(problem.numberOfMealstime(),
                        problem.getHour(), problem.getRecipies(),
                        hour, fout);
    }

    hour = 3;

    d.clearInputs();
    //d.userItemInput(problem.getHour(hour));
    d.parameter_input(hour,problem.size());
    d.print_user_input(d.getInputID(),d.getInputPortions(),fout);

    u.setSnack2Preferences(d.getInputID());
    u.setSnack2PreferencesPortions(d.getInputPortions());

    d.setCaloriesTargetChangeFlag(d.compareMealstimeItens(u.getSnack2Preferences(),
                                                          u.getSnack2PreferencesPortions(),
                                                          hour));

    if (d.getCaloriesTargetChangeFlag()) {
        d.clearMatrix(hour);
        d.setCaloricDiference(d.caloricDiferenceCalculation(
            problem.getHour(hour), fout, d.getInputID(), d.getInputPortions(),
            problem.getCalories(),
            problem.getCaloricTargetSnack2() + d.getCurrentBalance()));
        std::cout << "Current calories balance applied " << d.getCurrentBalance() << std::endl;
        d.setMeasltimeLeft(d.measltimeLeftCalc(
                hour, u.getNumberOfMealsTime()));
        d.setDiferenceForNextMeasltimes(d.applyCaloriesBalance(
                d.getCaloricDiference(), d.getMeasltimesLeft(), fout));
        d.setNextMealsNewCaloriesTarget(d.getCurrentBalance() +
                                        d.getDiferenceForNextMeasltimes());
        problem.setCaloriesTargetChangeFlag(d.getDiferenceForNextMeasltimes());

        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            problem.setTurn(i + 5);
            std::cout << "Current measltime shift: " << problem.turn() << std::endl;
            fout << "Current measltime shift: " << problem.turn() << std::endl;

            using solver_t = evolutionary_algorithm;
            solver_t solver(problem);
            solver.algorithm();

            solver.run();

            auto iter = solver.best_solution();
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Mealstime shift: " << i + 4 << std::endl;
            fout << "Measltime shift: " << i + 4 << std::endl;
            std::cout << "Mealtime: " << problem.getHour(i + 3)
                      << std::endl;
            fout << "Mealtime: " << problem.getHour(i + 3) << std::endl;
            std::cout << "Solution: " << std::endl;
            fout << "Solution: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
            std::cout << "FO: " << (((iter)->fx)) << std::endl;
            fout << "FO: " << (((iter)->fx)) << std::endl;
            std::cout << "................................................."
                         "..........................."
                      << std::endl;
            fout << "......................................................"
                    "......................"
                 << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            fout << std::endl;
            fout << std::endl;

            d.saveCurrentMenu(d.getCurrentSolution(),
                              d.getCurrentPortions());
        }
        d.printScreenMenu(problem.numberOfMealstime(), problem.getHour(),
                          problem.getRecipies(), hour);
        d.printFileMenu(problem.numberOfMealstime(),
                        problem.getHour(), problem.getRecipies(),
                        hour, fout);
    }
    hour = 4;

    d.clearInputs();
    //d.userItemInput(problem.getHour(hour));
    d.parameter_input(hour,problem.size());
    d.print_user_input(d.getInputID(),d.getInputPortions(),fout);

    u.setDinnerPreferences(d.getInputID());
    u.setDinnerPreferencesPortions(d.getInputPortions());

    d.setCaloriesTargetChangeFlag(d.compareMealstimeItens(u.getDinnerPreferences(),
                                                          u.getDinnerPreferencesPortions(),
                                                          hour));

    if (d.getCaloriesTargetChangeFlag()) {
        d.clearMatrix(hour);
        d.setCaloricDiference(d.caloricDiferenceCalculation(
            problem.getHour(hour), fout, d.getInputID(), d.getInputPortions(),
            problem.getCalories(),
            (problem.getDinnerCaloriesTarget() + d.getCurrentBalance())));
        std::cout << "Current balance calories:" << d.getCurrentBalance() << std::endl;
        d.setMeasltimeLeft(d.measltimeLeftCalc(
                hour, u.getNumberOfMealsTime()));
        d.setDiferenceForNextMeasltimes(d.applyCaloriesBalance(
                d.getCaloricDiference(), d.getMeasltimesLeft(), fout));
        d.setNextMealsNewCaloriesTarget(d.getCurrentBalance() +
                                        d.getDiferenceForNextMeasltimes());
        problem.setCaloriesTargetChangeFlag(d.getDiferenceForNextMeasltimes());



        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            problem.setTurn(i + 6);
            std::cout << "Current mealstime shift: " << problem.turn() << std::endl;
            fout << "Current mealstime shift: " << problem.turn() << std::endl;

            using solver_t = evolutionary_algorithm;
            solver_t solver(problem);
            solver.algorithm();

            solver.run();

            auto iter = solver.best_solution();
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Current mealstime: " << i + 5 << std::endl;
            fout << "Current mealstime: " << i + 5 << std::endl;
            std::cout << "Mealstime: " << problem.getHour(i + 4)
                      << std::endl;
            fout << "Mealstime: " << problem.getHour(i + 4) << std::endl;
            std::cout << "Solution: " << std::endl;
            fout << "Solution: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
            std::cout << "FO: " << (((iter)->fx)) << std::endl;
            fout << "FO: " << (((iter)->fx)) << std::endl;
            std::cout << "................................................."
                         "..........................."
                      << std::endl;
            fout << "......................................................"
                    "......................"
                 << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            fout << std::endl;
            fout << std::endl;

            d.saveCurrentMenu(d.getCurrentSolution(),
                              d.getCurrentPortions());
        }
        d.printScreenMenu(problem.numberOfMealstime(), problem.getHour(),
                          problem.getRecipies(), hour);
        d.printFileMenu(problem.numberOfMealstime(),
                        problem.getHour(), problem.getRecipies(),
                        hour, fout);
    }
    hour = 5;

    d.clearInputs();
    //d.userItemInput(problem.getHour(hour));
    d.parameter_input(hour,problem.size());
    d.print_user_input(d.getInputID(),d.getInputPortions(),fout);

    u.setSupperPreferences(d.getInputID());
    u.setSupperPreferencesPortions(d.getInputPortions());

    d.setCaloriesTargetChangeFlag(d.compareMealstimeItens(d.getInputID(),
                                                          d.getInputPortions(), hour));

    if (d.getCaloriesTargetChangeFlag()) {
        d.clearMatrix(hour);
        d.setCaloricDiference(d.caloricDiferenceCalculation(
            problem.getHour(hour), fout, d.getInputID(), d.getInputPortions(),
            problem.getCalories(),
            (problem.getSupperCaloricTarget() + d.getCurrentBalance())));
    }

    //Print final menu
    std::cout << "Total menu calories deviation: "
              << d.getCaloricDiference() << std::endl;
    printMenu(u, fout);

    //Ends time count
    time_req = clock() - time_req;
    std::cout << std::endl;
    std::cout << "Time:" << time_req << std::endl;
    fout << std::endl;
    fout << "Time:" << time_req << std::endl;
}
