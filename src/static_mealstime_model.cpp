
#include "dinamic_optimization.h"
#include "evolutionary_algorithm.h"
#include "user_diet.h"
#include "user_preferences.h"

void run_static_mealstime_model(std::string& file_output){

    // For experimental purposes we will create the user only with the essential information to solve the problem
    predictions::user_preferences u;
    u.setName(file_output);
    u.setMealsTime({6, 10, 12, 15, 18, 21});
    u.SetCaloriesTarget(2000);  //+ (u.getCaloriesBurn()*100));

    /*
    //These parameters will be used in the future for "user-based" preferences predictions
    u.setHigh(157);
    u.setGender(1); // true = mulher / false = homem
    u.setPais("Brasil");
    u.setWeight(52);
    u.setAge(26);
    u.setCaloriesBurn(3); // 0 = sedentario - 5 = Esportista
    u.setHobbyes({"Esporte"});
    */

    //Prints on screen and in file
    u.printScreenUser(u);
    std::ofstream fout(file_output, std::ofstream::out | std::ofstream::app);
    u.printScreenFile(u, fout);

    //Recreates the problem considering user data
    nutrition_facts problem(568, u.getTarget(),
                            u.getMealsTime());

    //Prints the problem on the screen and in the file
    problem.disp();
    problem.dispFile(fout, u.getTarget(), u.getNumberOfMealsTime());
    problem.setModelToUse(nutrition_facts::STATIC_MEASLTIME);

    //Starts time count
    clock_t time_req;
    time_req = clock();

    //Configures the model as static
    problem.setCaloriesTargetChangeFlag(1);

    // Creates dynamic object
    iteration::Dinamic d;

    //Starts meals-ttime cycle
    for (int i = 0; i < problem.numberOfMealstime(); i++) {
        // Sets the mealstime
        problem.setTurn(i + 1);
        std::cout << "Turno atual: " << problem.turn() << std::endl;
        fout << "Turno atual: " << problem.turn() << std::endl;

        // Creates the solver
        using solver_t = evolutionary_algorithm;
        solver_t solver(problem);
        solver.algorithm();

        // Run solver
        solver.run();

        // Print solution
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

        // Stores on the menu of the day
        d.saveCurrentMenu(d.getCurrentSolution(),
                          d.getCurrentPortions());
    }

    // Print and save to a file the IDs and their portions
    int horario = 0;
    d.printScreenMenu(problem.numberOfMealstime(), problem.getHour(),
                      problem.getRecipies(), horario);
    d.printFileMenu(problem.numberOfMealstime(), problem.getHour(),
                    problem.getRecipies(), horario, fout);

    // End time counting
    time_req = clock() - time_req;
    std::cout << std::endl;
    std::cout << "Tempo:" << time_req << std::endl;
    fout << std::endl;
    fout << "Tempo:" << time_req << std::endl;

}