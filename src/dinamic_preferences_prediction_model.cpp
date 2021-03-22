#include "dinamic_preferences_prediction_model.h"
#include "dinamic_optimization.h"
#include "evolutionary_algorithm.h"
#include "user_diet.h"
#include "user_preferences.h"

void run_dinamic_preferences_prediction_model(size_t problem_size, const std::string& file_output) {

    // For experimental purposes we will create user only with the essential information to solve the problem
    predictions::user_preferences u;
    u.setName(file_output);
    u.setMealsTime({6, 10, 12, 15, 18, 21});
    u.SetCaloriesTarget(2000);  //+ (u.getCaloriesBurn()*100));

    // Prints on screen and file
    u.printScreenUser(u);
    std::ofstream fout(file_output);
    u.printScreenFile(u, fout);

    //Creates the problem considering user data
    nutrition_facts problem(568, u.getTarget(), u.getMealsTime());
    problem.setModelToUse(nutrition_facts::DINAMIC_PREFERENCES_PREDICTION);

    // Prints the problem on the screen and in file
    problem.disp();
    problem.dispFile(fout, u.getTarget(), u.getNumberOfMealsTime());

    // Start counting time
    clock_t time_req;
    time_req = clock();

    problem.setCaloriesTargetChangeFlag(0);

    // Creates dynamic object
    iteration::Dinamic d;

    //Variables used for preferences predictions
    int userSimilarity = 0;
    std::vector<std::vector<double>> _rankSnack1, _rankLunch, _rankSnack2, _rankDinner, _rankSupper;
    std::vector<double>  _l2Snack1, _l2Lunch, _l2Snack2, _l2Dinner, _l2Supper;
    std::vector<std::vector<double>> _snack1ItensMatrix,_lunchItensMatrix,
            _snack2ItensMatrix,_dinnerItensMatrix,_supperItensMatrix;
    std::vector<double> _snack1UserScore,_lunchUserScore,_snack2UserScore,
            _dinnerUserScore,_supperUserScore;

    for (int i = 0; i < problem.numberOfMealstime(); i++) {
        // Configura o turn e o tipo de refeiçao
        problem.setTurn(i + 1);
        std::cout << "Turno atual: " << problem.turn() << std::endl;
        fout << "Turno atual: " << problem.turn() << std::endl;

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
        std::cout << "Refeiçao: " << i + 1 << std::endl;
        fout << "Refeiçao: " << i + 1 << std::endl;
        std::cout << "Horario: " << problem.getHour(i) << std::endl;
        fout << "Horario: " << problem.getHour(i) << std::endl;
        std::cout << "Solução: " << std::endl;
        fout << "Solução: " << std::endl;
        d.setCurrentSolution((iter)->disp(problem, fout));
        //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
        std::cout << "Função objetivo: " << (((iter)->fx)) << std::endl;
        fout << "Função objetivo: " << (((iter)->fx)) << std::endl;
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
    std::cout << "Tempo:" << time_req << std::endl;
    fout << std::endl;
    fout << "Tempo:" << time_req << std::endl;

    /// Starts interaction with the user for breakfast
    //receives values ​​directly from the user (Production use)
    //d.userItemInput(problem.getHour(hour));
    //receives values ​​by parameter in a matrix, each vector representing a mealtime (experimental use)
    d.parameter_input(hour,problem.size());
    d.print_user_input(d.getInputID(),d.getInputPortions(),fout);

    // Adds the foods currently eaten by the user as favorites (considering mealstime)
    // Breakfast
    u.setBreakfastPreferences(d.getInputID());
    u.setBreakfastPreferencesPortions(d.getInputPortions());
    // insertMatrizPreferenciasDesjejum(_breakfastPreferences);
    // insertPorcaoMatrizPreferenciasDesjejum(_breakfastPortionPreferences);

    //Compare proposed diet x user preferences (ingested food)
    d.setCaloriesTargetChangeFlag(
            d.compareMealstimeItens(u.getBreakfastPreferences(), u.getBreakfastPreferencesPortions(),
                                    hour));
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

        // Start considering preferences ((now we have the food eaten at breakfast))
        /* Calculates preferences prediction for next meals considering food eaten so far (braskfast).
         */

        // Calculates the user with more similar items eaten in breakfast
        userSimilarity = u.searchSimilarItensToBreakfast(
                userSimilarity, u.getBreakfastPreferences(),
                problem.getBreakfastPreferencesMatrix());

        // Functions that calculates preferences for other meals using Equations 1,2,3 and 4 of article
        // Predict snack 1

        ///Equation 1
        _rankSnack1 = u.normalize(
                problem.getSnack1PreferencesMatrix(),
                problem.getSnack1PortionPreferencesMatrix());

        ///Equation 2
        _l2Snack1 =
            u.euclidean_distance(problem.getSnack1PreferencesMatrix(),
                                   _rankSnack1, problem.size());

        ///Equation 3
        _snack1ItensMatrix = u.similarity_cosines(
                _rankSnack1, problem.getSnack1PreferencesMatrix(),
                _l2Snack1, problem.size());

        ///Equation 4
        _snack1UserScore =
            u.score_calc(_rankSnack1, _snack1ItensMatrix,
                           problem.getSnack1PreferencesMatrix(),
                           userSimilarity, problem_size);

        // Predict lunch
        _rankLunch = u.normalize(
                problem.getLunchPreferencesMatrix(),
                problem.getLunchPortionPreferencesMatrix());
        _l2Lunch =
            u.euclidean_distance(problem.getLunchPreferencesMatrix(),
                                   _rankLunch, problem.size());
        _lunchItensMatrix = u.similarity_cosines(
                _rankLunch, problem.getLunchPreferencesMatrix(), _l2Lunch,
                problem.size());
        _lunchUserScore =
            u.score_calc(_rankLunch, _lunchItensMatrix,
                           problem.getLunchPreferencesMatrix(),
                           userSimilarity, problem_size);

        // Predict snack 2
        _rankSnack2 = u.normalize(
                problem.getSnack2PreferencesMatrix(),
                problem.getSnack2PortionsPreferencesMatrix());
        _l2Snack2 =
            u.euclidean_distance(problem.getSnack2PreferencesMatrix(),
                                   _rankSnack2, problem.size());
        _snack2ItensMatrix = u.similarity_cosines(
                _rankSnack2, problem.getSnack2PreferencesMatrix(),
                _l2Snack2, problem.size());
        _snack2UserScore =
            u.score_calc(_rankSnack2, _snack2ItensMatrix,
                           problem.getSnack2PreferencesMatrix(),
                           userSimilarity, problem_size);

        // Predict dinner
        _rankDinner = u.normalize(
                problem.getDinnerPreferencesMatrix(),
                problem.getDinnerPortionsPreferencesMatrix());
        _l2Dinner =
            u.euclidean_distance(problem.getDinnerPreferencesMatrix(),
                                   _rankDinner, problem.size());
        _dinnerItensMatrix = u.similarity_cosines(
                _rankDinner, problem.getDinnerPreferencesMatrix(), _l2Dinner,
                problem.size());
        _dinnerUserScore =
            u.score_calc(_rankDinner, _dinnerItensMatrix,
                           problem.getDinnerPreferencesMatrix(),
                           userSimilarity, problem_size);

        // Predict supper
        _rankSupper = u.normalize(
                problem.getSupperPreferencesMatrix(),
                problem.getSupperPortionsPreferencesMatrix());
        _l2Supper = u.euclidean_distance(problem.getSupperPreferencesMatrix(),
                                       _rankSupper, problem.size());
        _supperItensMatrix = u.similarity_cosines(
                _rankSupper, problem.getSupperPreferencesMatrix(), _l2Supper,
                problem.size());
        _supperUserScore =
            u.score_calc(_rankSupper, _supperItensMatrix,
                           problem.getSupperPreferencesMatrix(),
                           userSimilarity, problem_size);

        // Start dynamic compensatory cycle
        //Start from next meal
        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            //Configures the shift skipping the meal already made
            problem.setTurn(i + 2);
            std::cout << "Turno atual: " << problem.turn() << std::endl;
            fout << "Turno atual: " << problem.turn() << std::endl;

            /// Send the vector with the score of each food for each type of meal to be considere in evaluation
            if (i == 0)
                problem.setSnack1Preferences(_snack1UserScore);
            else if (i == 1)
                problem.setLunchPreferences(_lunchUserScore);
            else if (i == 2)
                problem.setSnack2Preferences(_snack2UserScore);
            else if (i == 3)
                problem.setDinnerPreferences(_dinnerUserScore);
            else if (i == 4)
                problem.setSupperPreferences(_supperUserScore);

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
            std::cout << "Refeiçao: " << i + 2 << std::endl;
            fout << "Refeiçao: " << i + 2 << std::endl;
            std::cout << "Horario: " << problem.getHour(i + 1)
                      << std::endl;
            fout << "Horario: " << problem.getHour(i + 1) << std::endl;
            std::cout << "Solução: " << std::endl;
            fout << "Solução: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
            std::cout << "Função objetivo: " << (((iter)->fx)) << std::endl;
            fout << "Função objetivo: " << (((iter)->fx)) << std::endl;
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
        std::cout << "Diferenca no lanche 1 e "
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

        userSimilarity = u.snack1SimilaritySearch(
                userSimilarity, u.getBreakfastPreferences(),
                problem.getBreakfastPreferencesMatrix(),
                u.getSnack1Preferences(),
                problem.getSnack1PreferencesMatrix());

        _rankLunch = u.normalize(
                problem.getLunchPreferencesMatrix(),
                problem.getLunchPortionPreferencesMatrix());
        _l2Lunch =
            u.euclidean_distance(problem.getLunchPreferencesMatrix(),
                                   _rankLunch, problem.size());
        _lunchItensMatrix = u.similarity_cosines(
                _rankLunch, problem.getLunchPreferencesMatrix(), _l2Lunch,
                problem.size());
        _lunchUserScore =
            u.score_calc(_rankLunch, _lunchItensMatrix,
                           problem.getLunchPreferencesMatrix(),
                           userSimilarity, problem_size);

        _rankSnack2 = u.normalize(
                problem.getSnack2PreferencesMatrix(),
                problem.getSnack2PortionsPreferencesMatrix());
        _l2Snack2 =
            u.euclidean_distance(problem.getSnack2PreferencesMatrix(),
                                   _rankSnack2, problem.size());
        _snack2ItensMatrix = u.similarity_cosines(
                _rankSnack2, problem.getSnack2PreferencesMatrix(),
                _l2Snack2, problem.size());
        _snack2UserScore =
            u.score_calc(_rankSnack2, _snack2ItensMatrix,
                           problem.getSnack2PreferencesMatrix(),
                           userSimilarity, problem_size);

        _rankDinner = u.normalize(
                problem.getDinnerPreferencesMatrix(),
                problem.getDinnerPortionsPreferencesMatrix());
        _l2Dinner =
            u.euclidean_distance(problem.getDinnerPreferencesMatrix(),
                                   _rankDinner, problem.size());
        _dinnerItensMatrix = u.similarity_cosines(
                _rankDinner, problem.getDinnerPreferencesMatrix(), _l2Dinner,
                problem.size());
        _dinnerUserScore =
            u.score_calc(_rankDinner, _dinnerItensMatrix,
                           problem.getDinnerPreferencesMatrix(),
                           userSimilarity, problem_size);

        _rankSupper = u.normalize(
                problem.getSupperPreferencesMatrix(),
                problem.getSupperPortionsPreferencesMatrix());
        _l2Supper = u.euclidean_distance(problem.getSupperPreferencesMatrix(),
                                       _rankSupper, problem.size());
        _supperItensMatrix = u.similarity_cosines(
                _rankSupper, problem.getSupperPreferencesMatrix(), _l2Supper,
                problem.size());
        _supperUserScore =
            u.score_calc(_rankSupper, _supperItensMatrix,
                           problem.getSupperPreferencesMatrix(),
                           userSimilarity, problem_size);

        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            problem.setTurn(i + 3);
            std::cout << "Turno atual: " << problem.turn() << std::endl;
            fout << "Turno atual: " << problem.turn() << std::endl;

            if (i == 0)
                problem.setLunchPreferences(_lunchUserScore);
            else if (i == 1)
                problem.setSnack2Preferences(_snack2UserScore);
            else if (i == 2)
                problem.setDinnerPreferences(_dinnerUserScore);
            else if (i == 3)
                problem.setSupperPreferences(_supperUserScore);

            using solver_t = evolutionary_algorithm;
            solver_t solver(problem);
            solver.algorithm();

            solver.run();

            auto iter = solver.best_solution();
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Refeiçao: " << i + 3 << std::endl;
            fout << "Refeiçao: " << i + 3 << std::endl;
            std::cout << "Horario: " << problem.getHour(i + 2)
                      << std::endl;
            fout << "Horario: " << problem.getHour(i + 2) << std::endl;
            std::cout << "Solução: " << std::endl;
            fout << "Solução: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
            std::cout << "Função objetivo: " << (((iter)->fx)) << std::endl;
            fout << "Função objetivo: " << (((iter)->fx)) << std::endl;
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
        std::cout << "Anteriores Almoco" << d.getCurrentBalance() << std::endl;
        d.setMeasltimeLeft(d.measltimeLeftCalc(
                hour, u.getNumberOfMealsTime()));
        d.setDiferenceForNextMeasltimes(d.applyCaloriesBalance(
                d.getCaloricDiference(), d.getMeasltimesLeft(), fout));
        d.setNextMealsNewCaloriesTarget(d.getCurrentBalance() +
                                        d.getDiferenceForNextMeasltimes());
        problem.setCaloriesTargetChangeFlag(d.getDiferenceForNextMeasltimes());

        userSimilarity = u.lunchSimilaritySearch(
                userSimilarity, u.getBreakfastPreferences(),
                problem.getBreakfastPreferencesMatrix(),
                u.getSnack1Preferences(),
                problem.getSnack1PreferencesMatrix(),
                u.getLunchPreferences(),
                problem.getSnack1PreferencesMatrix());

        _rankSnack2 = u.normalize(
                problem.getSnack2PreferencesMatrix(),
                problem.getSnack2PortionsPreferencesMatrix());
        _l2Snack2 =
            u.euclidean_distance(problem.getSnack2PreferencesMatrix(),
                                   _rankSnack2, problem.size());
        _snack2ItensMatrix = u.similarity_cosines(
                _rankSnack2, problem.getSnack2PreferencesMatrix(),
                _l2Snack2, problem.size());
        _snack2UserScore =
            u.score_calc(_rankSnack2, _snack2ItensMatrix,
                           problem.getSnack2PreferencesMatrix(),
                           userSimilarity, problem_size);

        _rankDinner = u.normalize(
                problem.getDinnerPreferencesMatrix(),
                problem.getDinnerPortionsPreferencesMatrix());
        _l2Dinner =
            u.euclidean_distance(problem.getDinnerPreferencesMatrix(),
                                   _rankDinner, problem.size());
        _dinnerItensMatrix = u.similarity_cosines(
                _rankDinner, problem.getDinnerPreferencesMatrix(), _l2Dinner,
                problem.size());
        _dinnerUserScore =
            u.score_calc(_rankDinner, _dinnerItensMatrix,
                           problem.getDinnerPreferencesMatrix(),
                           userSimilarity, problem_size);

        _rankSupper = u.normalize(
                problem.getSupperPreferencesMatrix(),
                problem.getSupperPortionsPreferencesMatrix());
        _l2Supper = u.euclidean_distance(problem.getSupperPreferencesMatrix(),
                                       _rankSupper, problem.size());
        _supperItensMatrix = u.similarity_cosines(
                _rankSupper, problem.getSupperPreferencesMatrix(), _l2Supper,
                problem.size());
        _supperUserScore =
            u.score_calc(_rankSupper, _supperItensMatrix,
                           problem.getSupperPreferencesMatrix(),
                           userSimilarity, problem_size);

        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            problem.setTurn(i + 4);
            std::cout << "Turno atual: " << problem.turn() << std::endl;
            fout << "Turno atual: " << problem.turn() << std::endl;


            if (i == 0)
                problem.setSnack2Preferences(_snack2UserScore);
            else if (i == 1)
                problem.setDinnerPreferences(_dinnerUserScore);
            else if (i == 2)
                problem.setSupperPreferences(_supperUserScore);

            using solver_t = evolutionary_algorithm;
            solver_t solver(problem);
            solver.algorithm();

            solver.run();

            auto iter = solver.best_solution();
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Refeiçao: " << i + 4 << std::endl;
            fout << "Refeiçao: " << i + 4 << std::endl;
            std::cout << "Horario: " << problem.getHour(i + 3)
                      << std::endl;
            fout << "Horario: " << problem.getHour(i + 3) << std::endl;
            std::cout << "Solução: " << std::endl;
            fout << "Solução: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
            std::cout << "Função objetivo: " << (((iter)->fx)) << std::endl;
            fout << "Função objetivo: " << (((iter)->fx)) << std::endl;
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
        std::cout << "Anteriores Lanche2" << d.getCurrentBalance() << std::endl;
        d.setMeasltimeLeft(d.measltimeLeftCalc(
                hour, u.getNumberOfMealsTime()));
        d.setDiferenceForNextMeasltimes(d.applyCaloriesBalance(
                d.getCaloricDiference(), d.getMeasltimesLeft(), fout));
        d.setNextMealsNewCaloriesTarget(d.getCurrentBalance() +
                                        d.getDiferenceForNextMeasltimes());
        problem.setCaloriesTargetChangeFlag(d.getDiferenceForNextMeasltimes());

        userSimilarity = u.snack2SimilaritySearch(
                userSimilarity, u.getBreakfastPreferences(),
                problem.getBreakfastPreferencesMatrix(),
                u.getSnack1Preferences(),
                problem.getSnack1PreferencesMatrix(),
                u.getLunchPreferences(),
                problem.getLunchPreferencesMatrix(),
                u.getSnack2Preferences(),
                problem.getSnack2PreferencesMatrix());

        _rankDinner = u.normalize(
                problem.getDinnerPreferencesMatrix(),
                problem.getDinnerPortionsPreferencesMatrix());
        _l2Dinner =
            u.euclidean_distance(problem.getDinnerPreferencesMatrix(),
                                   _rankDinner, problem.size());
        _dinnerItensMatrix = u.similarity_cosines(
                _rankDinner, problem.getDinnerPreferencesMatrix(), _l2Dinner,
                problem.size());
        _dinnerUserScore =
            u.score_calc(_rankDinner, _dinnerItensMatrix,
                           problem.getDinnerPreferencesMatrix(),
                           userSimilarity, problem_size);

        _rankSupper = u.normalize(
                problem.getSupperPreferencesMatrix(),
                problem.getSupperPortionsPreferencesMatrix());
        _l2Supper = u.euclidean_distance(problem.getSupperPreferencesMatrix(),
                                       _rankSupper, problem.size());
        _supperItensMatrix = u.similarity_cosines(
                _rankSupper, problem.getSupperPreferencesMatrix(), _l2Supper,
                problem.size());
        _supperUserScore =
            u.score_calc(_rankSupper, _supperItensMatrix,
                           problem.getSupperPreferencesMatrix(),
                           userSimilarity, problem_size);

        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            problem.setTurn(i + 5);
            std::cout << "Turno atual: " << problem.turn() << std::endl;
            fout << "Turno atual: " << problem.turn() << std::endl;

            if (i == 0)
                problem.setSnack2Preferences(_snack2UserScore);
            else if (i == 1)
                problem.setDinnerPreferences(_dinnerUserScore);
            else if (i == 2)
                problem.setSupperPreferences(_supperUserScore);

            using solver_t = evolutionary_algorithm;
            solver_t solver(problem);
            solver.algorithm();

            solver.run();

            auto iter = solver.best_solution();
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Refeiçao: " << i + 4 << std::endl;
            fout << "Refeiçao: " << i + 4 << std::endl;
            std::cout << "Horario: " << problem.getHour(i + 3)
                      << std::endl;
            fout << "Horario: " << problem.getHour(i + 3) << std::endl;
            std::cout << "Solução: " << std::endl;
            fout << "Solução: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
            std::cout << "Função objetivo: " << (((iter)->fx)) << std::endl;
            fout << "Função objetivo: " << (((iter)->fx)) << std::endl;
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
        std::cout << "Anteriores jantar" << d.getCurrentBalance() << std::endl;
        d.setMeasltimeLeft(d.measltimeLeftCalc(
                hour, u.getNumberOfMealsTime()));
        d.setDiferenceForNextMeasltimes(d.applyCaloriesBalance(
                d.getCaloricDiference(), d.getMeasltimesLeft(), fout));
        d.setNextMealsNewCaloriesTarget(d.getCurrentBalance() +
                                        d.getDiferenceForNextMeasltimes());
        problem.setCaloriesTargetChangeFlag(d.getDiferenceForNextMeasltimes());

        userSimilarity = u.dinnerSimilaritySearch(
                userSimilarity, u.getBreakfastPreferences(),
                problem.getBreakfastPreferencesMatrix(),
                u.getSnack1Preferences(),
                problem.getSnack1PreferencesMatrix(),
                u.getLunchPreferences(),
                problem.getLunchPreferencesMatrix(),
                u.getSnack2Preferences(),
                problem.getSnack2PreferencesMatrix(),
                u.getDinnerPreferences(),
                problem.getDinnerPreferencesMatrix());

        std::cout << "Jantar ok 1" << std::endl;
        // Ceia
        _rankSupper = u.normalize(
                problem.getSupperPreferencesMatrix(),
                problem.getSupperPortionsPreferencesMatrix());
        std::cout << "Jantar ok 2" << std::endl;
        _l2Supper = u.euclidean_distance(problem.getSupperPreferencesMatrix(),
                                       _rankSupper, problem.size());
        std::cout << "Jantar ok 3" << std::endl;
        _supperItensMatrix = u.similarity_cosines(
                _rankSupper, problem.getSupperPreferencesMatrix(), _l2Supper,
                problem.size());
        std::cout << "Jantar ok 4" << std::endl;
        _supperUserScore =
            u.score_calc(_rankSupper, _supperItensMatrix,
                           problem.getSupperPreferencesMatrix(),
                           userSimilarity, problem_size);
        std::cout << "Jantar ok 5" << std::endl;

        hour++;
        for (int i = 0; i < d.getMeasltimesLeft(); i++) {

            problem.setTurn(i + 6);
            std::cout << "Turno atual: " << problem.turn() << std::endl;
            fout << "Turno atual: " << problem.turn() << std::endl;

            if (i == 0)
                problem.setDinnerPreferences(_dinnerUserScore);
            else if (i == 1)
                problem.setSupperPreferences(_supperUserScore);

            using solver_t = evolutionary_algorithm;
            solver_t solver(problem);
            solver.algorithm();

            solver.run();

            auto iter = solver.best_solution();
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Refeiçao: " << i + 5 << std::endl;
            fout << "Refeiçao: " << i + 5 << std::endl;
            std::cout << "Horario: " << problem.getHour(i + 4)
                      << std::endl;
            fout << "Horario: " << problem.getHour(i + 4) << std::endl;
            std::cout << "Solução: " << std::endl;
            fout << "Solução: " << std::endl;
            d.setCurrentSolution((iter)->disp(problem, fout));
            //d.setCurrentMealtimePortionSolution((iter)->porcao(problem,  d.getCurrentSolution(), fout));
            std::cout << "Função objetivo: " << (((iter)->fx)) << std::endl;
            fout << "Função objetivo: " << (((iter)->fx)) << std::endl;
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
    std::cout << "Sua ingestao final de calorias teve um desvio de "
              << d.getCaloricDiference() << std::endl;

    fout << "Sua ingestao final de calorias teve um desvio de "
              << d.getCaloricDiference() << std::endl;

    printMenu(u,fout);

    //Ends time count
    time_req = clock() - time_req;
    std::cout << std::endl;
    std::cout << "Tempo:" << time_req << std::endl;
    fout << std::endl;
    fout << "Tempo:" << time_req << std::endl;
}
