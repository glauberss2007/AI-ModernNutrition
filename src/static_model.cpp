
#include "dinamic_optimization.h"
#include "evolutionary_algorithm.h"
#include "nutrition_facts.h"
#include "user_diet.h"
#include "user_preferences.h"
#include "static_model.h"

void run_static_model(std::string& file_output){

    // For experimental purposes we will create the user only with the essential
    // information to solve the problem
    predictions::user_preferences u;
    u.setName(file_output);
    u.setMealsTime({12});
    u.SetCaloriesTarget(2000);  //+ (u.getCaloriesBurn()*100));

    // Prints an execution log on screen and in file
    u.printScreenUser(u);
    std::ofstream fout(file_output);
    u.printScreenFile(u, fout);

    // Create the problem and print
    nutrition_facts problem(568, u.getTarget(), u.getMealsTime());
    problem.setModelToUse(nutrition_facts::STATIC);
    problem.disp();
    problem.dispFile(fout, u.getTarget(), u.getNumberOfMealsTime());

    // Start timing
    clock_t time_req;
    time_req = clock();

    // Set up for single meal
    problem.setTurn(1);

    // Create dinamic object
    iteration::Dinamic d;

    // Solve the problem
    using solver_t = evolutionary_algorithm;
    solver_t solver(problem);
    solver.algorithm();
    solver.run();

    // Print solution
    auto iter = solver.best_solution();
    std::cout << std::endl;
    fout << std::endl;
    std::cout << "Solution: " << std::endl;
    fout << "Solution: " << std::endl;
    d.setCurrentSolution((iter)->disp(problem, fout));
    std::cout << "Objective function: " << (((iter)->fx)) << std::endl;
    fout << "Objective function: " << (((iter)->fx)) << std::endl;
    std::cout << "........................................................."
    "..................."
    << std::endl;
    fout << ".............................................................."
    ".............."
    << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    fout << std::endl;
    fout << std::endl;

    // Stores diet generated
    d.saveCurrentMenu(d.getCurrentSolution(), d.getCurrentPortions());

    // Print and save to file the IDs of the diets generated and their portions
    int horario = 0;
    d.printScreenMenu(problem.numberOfMealstime(), problem.getHour(), problem.getRecipies(), horario);
    d.printFileMenu(problem.numberOfMealstime(), problem.getHour(), problem.getRecipies(), horario, fout);

    // Stop time counting
    time_req = clock() - time_req;
    std::cout << std::endl;
    std::cout << "Tempo:" << time_req << std::endl;
    fout << std::endl;
    fout << "Tempo:" << time_req << std::endl;
}