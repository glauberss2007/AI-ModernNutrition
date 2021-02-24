#include "evolutionary_algorithm.h"
#include "matplot/matplot.h"
#include "matplot/util/common.h"
#include "matplot/util/keywords.h"

#include <cmath>

#ifndef __GNUC__

#if defined WIN32
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4717 )
#pragma warning ( disable : 4996 )
#pragma warning ( disable : 4616 )
#endif

#endif

std::default_random_engine evolutionary_algorithm::_generator = std::default_random_engine(
    (unsigned int)std::chrono::system_clock::now().time_since_epoch().count());

evolutionary_algorithm::evolutionary_algorithm(nutrition_facts &p)
    :  _fitness_sharing_niche_size(0.05 * p.size()),_mutation_strength(1.0 / p.size()), _problem(p) {
    this->algorithm();
    if (this->_problem.is_minimization()) {
        this->_comp = [](individual_ptr &a, individual_ptr &b) {
            return a->fx < b->fx;
        };
        this->_not_comp = [](individual_ptr &a, individual_ptr &b) {
            return a->fx > b->fx;
        };
    } else {
        this->_comp = [](individual_ptr &a, individual_ptr &b) {
            return a->fx > b->fx;
        };
        this->_not_comp = [](individual_ptr &a, individual_ptr &b) {
            return a->fx < b->fx;
        };
    }
    this->_comp_fitness = [](individual_ptr &a, individual_ptr &b) {
        return a->fitness > b->fitness;
    };
    this->_not_comp_fitness = [](individual_ptr &a, individual_ptr &b) {
        return a->fitness < b->fitness;
    };
}

void evolutionary_algorithm::algorithm() {
    this->_population_size = 100;
    this->_number_of_islands = 5;
    this->_fitness_sharing_niche_size = 3.0;
    this->_children_proportion = 7.0;
    this->_crossover_probability = 0.6;
    this->_mutation_strength = 0.4; // 1/this->_problem.size();
    this->_competition_between_parents_and_children = false;
    this->_elitism_proportion = 0.02;
    this->_reproduction_scaling_strategy = scaling_strategy::window;
    this->_reproduction_selection_strategy = selection_strategy::tournament;
    this->_survival_scaling_strategy = scaling_strategy::window;
    this->_survival_selection_strategy = selection_strategy::tournament;
}

void evolutionary_algorithm::initialize_population() {
    this->_population.resize(0);
    this->_population.reserve(this->_population_size);
    for (size_t i = this->_population.size(); i < this->_population_size; ++i) {
        this->_population.emplace_back(
            std::make_shared<user_diet>(this->_problem));
    }
}

void evolutionary_algorithm::run() {
    initialize_population();
    std::vector<double> _fxs;
    for (size_t i = 0; i < this->_max_generations; ++i) {
        evolutionary_cycle();
        _fxs.push_back(this->best_fx());
    }
    // Generate matplot convergence image
    std::vector<int> _generations;
    for (int j=0; j < _fxs.size(); j++) {
        _generations.push_back(j);
        if (j > 0 && _fxs[j] > _fxs[j-1]) _fxs[j] = _fxs[j-1];
    }
    auto a = matplot::plot(_generations,_fxs);
    matplot::xlabel("Generation");
    matplot::ylabel("Objective Function");
    // Export matplot convergence and vetorial image and latex
    matplot::save(LOCAL_PATH_EXPORTS"/exports/convergence-curve/FXs.svg");
    matplot::save(LOCAL_PATH_EXPORTS"/exports/convergence-curve/", "epslatex");
    matplot::show();
}
double_t iteration;

void evolutionary_algorithm::run(size_t iterations) {
    initialize_population();
    for (int i = 0; i < (int)iterations; ++i) {
        evolutionary_cycle();
        iteration = (double_t)iterations;
    }
}

void evolutionary_algorithm::evolutionary_cycle() {
    display_status();
    evaluate(this->_population);
    scaling(this->_population, this->_reproduction_scaling_strategy);
    fitness_sharing(this->_population);
    population_type parents =
        selection(this->_population, n_of_selection_candidates(),
                  this->_reproduction_selection_strategy);
    population_type children = reproduction(parents);
    evaluate(children);

    const size_t size_of_elite_set = this->size_of_elite_set();
    population_type parents_and_children =
        this->merge(this->_population, children);
    population_type next_population_candidates =
        (this->_competition_between_parents_and_children ||
         this->_children_proportion < 1.0)
            ? parents_and_children
            : children;
    scaling(next_population_candidates, this->_survival_scaling_strategy);
    population_type survivors = selection(
        next_population_candidates, this->_population_size - size_of_elite_set,
        this->_survival_selection_strategy);
    this->_population =
        insert_elite_set(survivors, parents_and_children, size_of_elite_set);
    migration_step();
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::merge(population_type &parents,
                                       population_type &children) {
    population_type result = parents;
    result.insert(result.end(), children.begin(), children.end());
    return result;
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::insert_elite_set(population_type &individuals,
                                                  population_type &elite_source,
                                                  size_t size_of_elite_set) {
    std::partial_sort(elite_source.begin(),
                      elite_source.begin() + size_of_elite_set,
                      elite_source.end(), this->_comp_fitness);
    individuals.insert(individuals.end(), elite_source.begin(),
                       elite_source.begin() + size_of_elite_set);
    return individuals;
}

double evolutionary_algorithm::best_fx() {
    if (!this->_best_solutions.empty()) {
        return this->_best_solutions[0]->fx;
    } else {
        return this->_problem.is_minimization()
                   ? std::numeric_limits<double>::max()
                   : -std::numeric_limits<double>::max();
    }
}

typename evolutionary_algorithm::individual_ptr
evolutionary_algorithm::best_solution() {
    if (!this->_best_solutions.empty()) {
        return this->_best_solutions[0];
    } else {
        return nullptr;
    }
}

void evolutionary_algorithm::copy_solution(int i) {
    if (!this->_best_solutions.empty()) {
        this->_hours_solutions[i] = this->_best_solutions[0];
    }
}

void evolutionary_algorithm::evaluate(population_type &population) {
    for (individual_ptr &item : population) {
        item->fx = item->evaluate(this->_problem);
        this->try_to_update_best(item);
    }
}

size_t evolutionary_algorithm::n_of_selection_candidates() {
    double ret = (double)this->_population_size * (double)this->_parents_per_children *
        this->_children_proportion;

    return (size_t)ret;
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::selection(population_type &population,
                                           size_t n_of_candidates,
                                           selection_strategy s) {
    population_type result;
    for (int i = 0; i < (int)this->_number_of_islands; ++i) {
        population_type island = get_island(population, i);
        population_type island_result;
        size_t n_of_candidates_per_island =
            n_of_candidates / this->_number_of_islands;
        switch (s) {
        case selection_strategy::uniform:
            island_result =
                uniform_selection(island, n_of_candidates_per_island);
            break;
        case selection_strategy::truncate:
            island_result =
                truncate_selection(island, n_of_candidates_per_island);
            break;
        case selection_strategy::tournament:
            island_result =
                tournament_selection(island, n_of_candidates_per_island);
            break;
        case selection_strategy::roulette:
            island_result =
                roulette_selection(island, n_of_candidates_per_island);
            break;
        case selection_strategy::sus:
            island_result = sus_selection(island, n_of_candidates_per_island);
            break;
        case selection_strategy::overselection:
            island_result =
                overselection_selection(island, n_of_candidates_per_island);
            break;
        case selection_strategy::roundrobin_tournament:
            island_result =
                roundrobin_selection(island, n_of_candidates_per_island);
            break;
        }
        result.insert(result.end(), island_result.begin(), island_result.end());
    }
    return result;
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::reproduction(population_type &parents) {
    std::uniform_real_distribution<double> r(0.0, 1.0);
    population_type children;
    children.reserve(parents.size() / this->_parents_per_children);
    for (size_t j = 0; j < parents.size(); j += this->_parents_per_children) {
        double crossover_probability =
            (parents[j]->_crossover_probability +
             parents[j + 1]->_crossover_probability) /
            2;
        if (r(evolutionary_algorithm::_generator) < crossover_probability) {
            children.emplace_back(std::make_shared<user_diet>(
                parents[j]->crossover(this->_problem, *parents[j + 1])));
        } else {
            children.emplace_back(std::make_shared<user_diet>(*parents[j]));
            children.back()->mutation(children.back()->_mutation_strength);
        }
    }
    return children;
}

void evolutionary_algorithm::display_status() {
    std::cout << "Generation #" << ++_current_generation;
    if (iteration == 6) {
        _problem.setTurn(6);
        _problem.setMeta(200);
        std::cout << " Nova caloriesTarget: " << _problem.getmeta() << std::endl;
    } else if (iteration == 5) {
        _problem.setTurn(5);
        _problem.setMeta(400);
        std::cout << " Nova caloriesTarget: " << _problem.getmeta() << std::endl;
    } else if (iteration == 4) {
        _problem.setTurn(4);
        _problem.setMeta(200);
        std::cout << " Nova caloriesTarget: " << _problem.getmeta() << std::endl;
    } else if (iteration == 3) {
        _problem.setTurn(3);
        _problem.setMeta(600);
        std::cout << " Nova caloriesTarget: " << _problem.getmeta() << std::endl;
    } else if (iteration == 2) {
        _problem.setTurn(2);
        _problem.setMeta(200);
        std::cout << " Nova caloriesTarget: " << _problem.getmeta() << std::endl;
    } else if (iteration == 1) {
        _problem.setTurn(1);
        _problem.setMeta(400);
        // std::cout << " Nova caloriesTarget: " << _problem.caloriesTarget() << std::endl;
    }

    if (this->_current_generation > 1) {
        std::cout << " Best_fx: " << (this->best_fx()) << std::endl;
        double min_fx = std::numeric_limits<double>::max();
        double avg_fx = 0.0;
        double max_fx = -std::numeric_limits<double>::max();
        for (individual_ptr &ind : this->_population) {
             min_fx = std::min(min_fx, ind->fx);
             max_fx = std::max(max_fx, ind->fx);
             avg_fx += ind->fx;
        }
         avg_fx /= this->_population.size();
         std::cout << "| Min: " << min_fx
                  << " | Avg: " << avg_fx
                  << " | Max: " << max_fx
                  << " | "
                  << ((min_fx == max_fx) ? ("(Population Converged)") : (""))
                  << std::endl;
    } else {
       std::cout << std::endl;
    }
}

void evolutionary_algorithm::scaling(population_type &population, scaling_strategy s) {
    switch (s) {
    case scaling_strategy::window: {
        window_scaling(population);
        break;
    }
    case scaling_strategy::sigma: {
        sigma_scaling(population);
        break;
    }
    case scaling_strategy::linear_rank: {
        linear_rank_scaling(population);
        break;
    }
    case scaling_strategy::exponential_rank: {
        exponential_rank_scaling(population);
        break;
    }
    }
}

void evolutionary_algorithm::attribute_fitness_from_rank(
    evolutionary_algorithm::population_type &population) {
    std::sort(population.begin(), population.end(), this->_comp);
    for (size_t i = 0; i < population.size(); ++i) {
        population[i]->fitness =
            (double)(this->_problem.is_minimization() ? population.size() - i : i + 1);
    }
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::uniform_selection(
    evolutionary_algorithm::population_type &population, size_t n_of_candidates) {
    population_type parents(n_of_candidates);
    std::uniform_int_distribution<size_t> pos_d(0, population.size() - 1);
    for (individual_ptr &parent : parents) {
        parent = population[pos_d(evolutionary_algorithm::_generator)];
    }
    return parents;
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::tournament_selection(
    evolutionary_algorithm::population_type &population,
                         size_t n_of_candidates) {
    population_type parents;
    std::uniform_int_distribution<size_t> pos_d(0, population.size() - 1);
    for (size_t i = 0; i < n_of_candidates; ++i) {
        size_t position = pos_d(evolutionary_algorithm::_generator);
        for (size_t j = 1; j < this->_tournament_size; ++j) {
            size_t position2 = pos_d(evolutionary_algorithm::_generator);
            if (population[position2]->fitness >
                population[position]->fitness) {
                position = position2;
            }
        }
        parents.push_back(population[position]);
    }
    return parents;
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::roulette_selection(
    evolutionary_algorithm::population_type &population,
                       size_t n_of_candidates) {
    population_type parents;
    parents.reserve(n_of_candidates);
    std::discrete_distribution<size_t> pos_d(
        population.size(), 0, (double)(population.size() - 1),
        [&population](double pos) { return (size_t)population[(size_t)pos]->fitness; });
    for (size_t i = 0; i < n_of_candidates; ++i) {
        parents.push_back(population[pos_d(evolutionary_algorithm::_generator)]);
    }
    return parents;
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::truncate_selection(
    evolutionary_algorithm::population_type &population,
                       size_t n_of_candidates) {
    population_type parents;
    parents.reserve(n_of_candidates);
    std::partial_sort(population.begin(),
                      population.begin() +
                          std::min(n_of_candidates, population.size()),
                      population.end(), this->_comp_fitness);
    std::copy(population.begin(),
              population.begin() + std::min(n_of_candidates, population.size()),
              std::back_inserter(parents));
    int i = 0;
    while (parents.size() < n_of_candidates) {
        parents.push_back(parents[i % population.size()]);
        i++;
    }
    std::shuffle(parents.begin(), parents.end(),
                 evolutionary_algorithm::_generator);
    return parents;
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::sus_selection(
    evolutionary_algorithm::population_type &population,
                                               size_t n_of_candidates) {
    population_type parents;
    parents.reserve(n_of_candidates);
    double total_fit = 0.0;
    for (individual_ptr &ind : population) {
        total_fit += ind->fitness;
    }
    double gap = total_fit / n_of_candidates;
    std::uniform_real_distribution<double> dist_r(0.0, gap);
    double r = dist_r(evolutionary_algorithm::_generator);
    size_t current_ind = 0;
    double sum = population[current_ind]->fitness;
    for (size_t i = 0; i < n_of_candidates; ++i) {
        while (r > sum) {
            ++current_ind;
            sum += population[current_ind]->fitness;
        }
        parents.push_back(population[current_ind]);
        r += gap;
    }
    std::shuffle(parents.begin(), parents.end(),
                 evolutionary_algorithm::_generator);
    return parents;
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::overselection_selection(
    evolutionary_algorithm::population_type &population,
                            size_t n_of_candidates) {
    population_type parents;
    parents.reserve(n_of_candidates);

    std::partial_sort(population.begin(),

                      population.begin() +

                          (int)(population.size() * this->_overselection_proportion),

                      population.end(), this->_comp_fitness);

    std::uniform_int_distribution<size_t> pos_best(
        0, (size_t)(population.size() * this->_overselection_proportion - 1));
    for (int i = 0; i < n_of_candidates * 0.8; ++i) {
        parents.push_back(population[pos_best(evolutionary_algorithm::_generator)]);
    }
    std::uniform_int_distribution<size_t> pos_worst(
        size_t(population.size() * this->_overselection_proportion),
        size_t(population.size() - 1));
    for (int i = 0; i < n_of_candidates - (n_of_candidates * 0.8); ++i) {
        parents.push_back(population[pos_worst(evolutionary_algorithm::_generator)]);
    }
    return parents;
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::roundrobin_selection(
    evolutionary_algorithm::population_type &population,
                         size_t n_of_candidates) {
    struct solution_with_score {
        individual_ptr s;
        size_t score = 0;
    };
    std::vector<solution_with_score> scores(population.size());
    for (size_t i = 0; i < population.size(); ++i) {
        scores[i].s = population[i];
    }
    std::uniform_int_distribution<size_t> pos_d(0, scores.size() - 1);
    for (size_t i = 0; i < population.size(); ++i) {
        solution_with_score &player1 = scores[i];
        for (size_t j = 1; j < this->_roundrobin_tournament_size; ++j) {
            solution_with_score &player2 = scores[pos_d(evolutionary_algorithm::_generator)];
            if (player1.s->fitness > player2.s->fitness) {
                player1.score++;
            } else {
                player2.score++;
            }
        }
    }
    std::sort(scores.begin(), scores.end(),
              [](solution_with_score &a, solution_with_score &b) {
                  return a.score > b.score;
              });
    population_type parents;
    parents.reserve(n_of_candidates);
    for (size_t i = 0; i < std::min(n_of_candidates, population.size()); ++i) {
        parents.push_back(scores[i].s);
    }
    int i = 0;
    while (parents.size() < n_of_candidates) {
        parents.push_back(parents[i % population.size()]);
        i++;
    }
    return parents;
}

void evolutionary_algorithm::window_scaling(
    evolutionary_algorithm::population_type &population) {
    for (individual_ptr &ind : population) {
        ind->fitness = this->_problem.is_minimization() ? -ind->fx : ind->fx;
    }
    const auto iter_to_min_fitness = std::min_element(
        population.begin(), population.end(), this->_not_comp_fitness);
    double min_fitness = (*iter_to_min_fitness)->fitness;
    for (individual_ptr &ind : population) {
        ind->fitness -= min_fitness + 1;
    }
}

void evolutionary_algorithm::sigma_scaling(
    evolutionary_algorithm::population_type &population) {
    for (individual_ptr &ind : population) {
        ind->fitness = this->_problem.is_minimization() ? -ind->fx : ind->fx;
    }
    double avg_fitness = 0.0;
    for (individual_ptr &ind : population) {
        avg_fitness += ind->fitness;
    }
    double std_fitness = 0.0;
    for (individual_ptr &ind : population) {
        std_fitness += pow(ind->fitness - avg_fitness, 2.0);
    }
    std_fitness /= population.size() - 1;
    std_fitness = sqrt(std_fitness);
    for (individual_ptr &ind : population) {
        ind->fitness = std::max(this->_sigma_bias +
                                    (ind->fitness - avg_fitness) /
                                        (this->_sigma_constant * std_fitness),
                                0.0);
    }
}

void evolutionary_algorithm::linear_rank_scaling(
    evolutionary_algorithm::population_type &population) {
    attribute_fitness_from_rank(population);
    const double bias =
        ((2 - _linear_rank_selective_pressure) / population.size());
    const double denominator = (double)(population.size() * (population.size() - 1));
    for (individual_ptr &ind : population) {
        ind->fitness =
            bias + (2 * ind->fitness * (_linear_rank_selective_pressure - 1)) /
                       denominator;
    }
}

void evolutionary_algorithm::exponential_rank_scaling(
    evolutionary_algorithm::population_type &population) {
    attribute_fitness_from_rank(population);
    for (individual_ptr &ind : population) {
        ind->fitness = (1 - exp(-ind->fitness)) / population.size();
    }
}

size_t evolutionary_algorithm::size_of_elite_set() {
    return size_t(ceil(this->_elitism_proportion * this->_population.size()));
}

typename evolutionary_algorithm::population_type
evolutionary_algorithm::get_island(population_type &population,
                                            int idx) {
    population_type island;
    const size_t island_size = population.size() / this->_number_of_islands;
    island.insert(island.end(), population.begin() + idx * island_size,
                  population.begin() + (idx + 1) * island_size);
    return island;
}

void evolutionary_algorithm::try_to_update_best(
    evolutionary_algorithm::individual_ptr &candidate) {
    const bool solution_is_new =
        std::find_if(this->_best_solutions.begin(), this->_best_solutions.end(),
                     [this, &candidate](individual_ptr &a) {
                         return (a->distance(this->_problem, *candidate, 0.0) ==
                                 0.0);
                     }) == this->_best_solutions.end();
    if (solution_is_new) {
        if (this->_best_solutions.size() < this->_number_of_best_solutions) {
            this->_best_solutions.push_back(candidate);
        } else {
            if (this->_comp(candidate, this->_best_solutions.back())) {
                this->_best_solutions.pop_back();
                this->_best_solutions.push_back(candidate);
            }
        }
        std::sort(this->_best_solutions.begin(), this->_best_solutions.end(),
                  this->_comp);
    }
}

typename evolutionary_algorithm::population_type::iterator
evolutionary_algorithm::best_solutions_begin() {
    return this->_best_solutions.begin();
}

typename evolutionary_algorithm::population_type::iterator
evolutionary_algorithm::best_solutions_end() {
    return this->_best_solutions.end();
}

void evolutionary_algorithm::migration_step() {
    if (this->_current_generation % this->_migration_epoch == 0) {
        for (size_t i = 0; i < this->_number_of_islands; ++i) {
            population_type island1 = get_island(this->_population, (int)i);
            population_type island2;
            switch (this->_island_structure) {
            case island_structure::random: {
                std::uniform_int_distribution<int> island_idx(
                    0, (int)(this->_number_of_islands - 1));
                island2 =
                    get_island(this->_population, island_idx(evolutionary_algorithm::_generator));
                break;
            }
            case island_structure::ring: {
                island2 = get_island(this->_population,
                                     (int)((i + 1) % this->_number_of_islands));
                break;
            }
            }

            population_type individuals_emigrating;
            switch (this->_island_migration_policy) {
            case island_migration_policy::random: {
                std::uniform_int_distribution<int> island_idx(
                    0, (int)(island1.size() - 1));
                for (size_t j = 0; j < this->_migration_size; ++j) {
                    individuals_emigrating.push_back(
                        island1[island_idx(
                        evolutionary_algorithm::_generator)]);
                }
                break;
            }
            case island_migration_policy::best: {
                std::partial_sort(island1.begin(),
                                  island1.begin() + _migration_size,
                                  island1.end(), this->_comp);
                individuals_emigrating.insert(
                    individuals_emigrating.end(), island1.begin(),
                    island1.begin() + _migration_size);
                break;
            }
            case island_migration_policy::fittest_half: {
                std::partial_sort(island1.begin(),
                                  island1.begin() + island1.size() / 2,
                                  island1.end(), this->_comp);
                std::uniform_int_distribution<int> island_idx(
                    0, (int)(island1.size() / 2));
                for (size_t j = 0; j < this->_migration_size; ++j) {
                    individuals_emigrating.push_back(
                        island1[island_idx(
                        evolutionary_algorithm::_generator)]);
                }
                break;
            }
            }

            population_type individuals_immigrating;
            switch (this->_island_replacement_policy) {
            case island_replacement_policy::worst_swap: {
                std::partial_sort(island2.begin(),
                                  island2.begin() + _migration_size,
                                  island2.end(), this->_not_comp);
                individuals_immigrating.insert(
                    individuals_immigrating.end(), island2.begin(),
                    island2.begin() + _migration_size);
                for (size_t j = 0; j < this->_migration_size; ++j) {
                    std::swap(*individuals_immigrating[j],
                              *individuals_emigrating[j]);
                }
                break;
            }
            case island_replacement_policy::worst_overwrite: {
                std::partial_sort(island2.begin(),
                                  island2.begin() + _migration_size,
                                  island2.end(),
                                  [this](individual_ptr &a, individual_ptr &b) {
                                      return !this->_comp(a, b);
                                  });
                individuals_immigrating.insert(
                    individuals_immigrating.end(), island2.begin(),
                    island2.begin() + _migration_size);
                for (size_t j = 0; j < this->_migration_size; ++j) {
                    *individuals_immigrating[j] = *individuals_emigrating[j];
                }
                break;
            }
            case island_replacement_policy::random_swap: {
                std::uniform_int_distribution<int> island_idx(
                    0, (int)(island2.size() - 1));
                for (size_t j = 0; j < this->_migration_size; ++j) {
                    individuals_immigrating.push_back(
                        island2[island_idx(
                        evolutionary_algorithm::_generator)]);
                }
                for (size_t j = 0; j < this->_migration_size; ++j) {
                    std::swap(*individuals_immigrating[j],
                              *individuals_emigrating[j]);
                }
                break;
            }
            case island_replacement_policy::random_overwrite: {
                std::uniform_int_distribution<int> island_idx(
                    0, (int)(island2.size() - 1));
                for (size_t j = 0; j < this->_migration_size; ++j) {
                    individuals_immigrating.push_back(
                        island2[island_idx(
                        evolutionary_algorithm::_generator)]);
                }
                for (size_t j = 0; j < this->_migration_size; ++j) {
                    *individuals_immigrating[j] = *individuals_emigrating[j];
                }
                break;
            }
            }
        }
    }
}

void evolutionary_algorithm::fitness_sharing(
    evolutionary_algorithm::population_type &population) {
    for (size_t i = 0; i < population.size(); ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < population.size(); ++j) {
            const double d =
                population[i]->distance(this->_problem, *population[j],
                                        this->_fitness_sharing_niche_size);
            const double sh = (d <= this->_fitness_sharing_niche_size)
                                  ? (1 - d / this->_fitness_sharing_niche_size)
                                  : 0.0;
            sum += sh;
        }
        population[i]->fitness /= sum + 1.0;
    }
}

// Setters and getters
// Population management

size_t evolutionary_algorithm::population_size() { return this->_population_size; }

void evolutionary_algorithm::population_size(size_t value) { this->_population_size = value; }

size_t evolutionary_algorithm::number_of_islands() { return this->_number_of_islands; }

void evolutionary_algorithm::number_of_islands(size_t value) { this->_number_of_islands = value; }

enum evolutionary_algorithm::island_structure
evolutionary_algorithm::island_structure() {
    return this->_island_structure;
}

void evolutionary_algorithm::island_structure(enum island_structure value) {
    this->_island_structure = value;
}

enum evolutionary_algorithm::island_migration_policy
evolutionary_algorithm::island_migration_policy() {
    return this->_island_migration_policy;
}

void evolutionary_algorithm::island_migration_policy(enum island_migration_policy value) {
    this->_island_migration_policy = value;
}

enum evolutionary_algorithm::island_replacement_policy
evolutionary_algorithm::island_replacement_policy() {
    return this->_island_replacement_policy;
}

void evolutionary_algorithm::island_replacement_policy(enum island_replacement_policy value) {
    this->_island_replacement_policy = value;
}

size_t evolutionary_algorithm::migration_epoch() { return this->_migration_epoch; }

void evolutionary_algorithm::migration_epoch(size_t value) { this->_migration_epoch = value; }

size_t evolutionary_algorithm::migration_size() { return this->_migration_size; }

void evolutionary_algorithm::migration_size(size_t value) { this->_migration_size = value; }

double evolutionary_algorithm::fitness_sharing_niche_size() {
    return this->_fitness_sharing_niche_size;
}

void evolutionary_algorithm::fitness_sharing_niche_size(double value) {
    this->_fitness_sharing_niche_size = value;
}
// Stopping criteria

size_t evolutionary_algorithm::max_generations() { return this->_max_generations; }

void evolutionary_algorithm::max_generations(size_t value) { this->_max_generations = value; }
// Reproduction

double evolutionary_algorithm::children_proportion() { return this->_children_proportion; }

void evolutionary_algorithm::children_proportion(double value) {
    this->_children_proportion = value;
}

double evolutionary_algorithm::crossover_probability() { return this->_crossover_probability; }

void evolutionary_algorithm::crossover_probability(double value) {
    this->_crossover_probability = value;
}

double evolutionary_algorithm::mutation_strength() { return this->_mutation_strength; }

void evolutionary_algorithm::mutation_strength(double value) { this->_mutation_strength = value; }
// Selection

bool evolutionary_algorithm::competition_between_parents_and_children() {
    return this->_competition_between_parents_and_children;
}

void evolutionary_algorithm::competition_between_parents_and_children(bool value) {
    this->_competition_between_parents_and_children = value;
}

unsigned int evolutionary_algorithm::tournament_size() { return this->_tournament_size; }

void evolutionary_algorithm::tournament_size(unsigned int value) { this->_tournament_size = value; }

double evolutionary_algorithm::overselection_proportion() {
    return this->_overselection_proportion;
}

void evolutionary_algorithm::overselection_proportion(double value) {
    this->_overselection_proportion = value;
}

size_t evolutionary_algorithm::roundrobin_tournament_size() {
    return this->_roundrobin_tournament_size;
}

void evolutionary_algorithm::roundrobin_tournament_size(size_t value) {
    this->_roundrobin_tournament_size = value;
}

double evolutionary_algorithm::elitism_proportion() { return this->_elitism_proportion; }

void evolutionary_algorithm::elitism_proportion(double value) { this->_elitism_proportion = value; }
// scaling

enum evolutionary_algorithm::scaling_strategy
evolutionary_algorithm::reproduction_scaling_strategy() {
    return this->_reproduction_scaling_strategy;
}

void evolutionary_algorithm::reproduction_scaling_strategy(enum scaling_strategy value) {
    this->_reproduction_scaling_strategy = value;
}

enum evolutionary_algorithm::selection_strategy
evolutionary_algorithm::reproduction_selection_strategy() {
    return this->_reproduction_selection_strategy;
}

void evolutionary_algorithm::reproduction_selection_strategy(enum selection_strategy value) {
    this->_reproduction_selection_strategy = value;
}

enum evolutionary_algorithm::scaling_strategy
evolutionary_algorithm::survival_scaling_strategy() {
    return this->_survival_scaling_strategy;
}

void evolutionary_algorithm::survival_scaling_strategy(enum scaling_strategy value) {
    this->_survival_scaling_strategy = value;
}

enum evolutionary_algorithm::selection_strategy
evolutionary_algorithm::survival_selection_strategy() {
    return this->_survival_selection_strategy;
}

void evolutionary_algorithm::survival_selection_strategy(enum selection_strategy value) {
    this->_survival_selection_strategy = value;
}

double evolutionary_algorithm::sigma_bias() { return this->_sigma_bias; }

void evolutionary_algorithm::sigma_bias(double value) { this->_sigma_bias = value; }

double evolutionary_algorithm::sigma_constant() { return this->_sigma_constant; }

void evolutionary_algorithm::sigma_constant(double value) { this->_sigma_constant = value; }

double evolutionary_algorithm::linear_rank_selective_pressure() {
    return this->_linear_rank_selective_pressure;
}

void evolutionary_algorithm::linear_rank_selective_pressure(double value) {
    this->_linear_rank_selective_pressure = value;
}

void evolutionary_algorithm::individual::trampoline_mutation(nutrition_facts &p) {
    individual::mutation(p);
}

void evolutionary_algorithm::individual::mutation(nutrition_facts &p) {
    // mutate search parameters before they influence mutation
    std::normal_distribution<double> n(0.0, 1);

    _mutation_strength =
        _mutation_strength *
        exp(_second_order_mutation_strength * n(evolutionary_algorithm::_generator));
    reflect(_mutation_strength, 0.001, 0.999);

    _crossover_probability =
        _crossover_probability *
        exp(_second_order_mutation_strength * n(evolutionary_algorithm::_generator));
    reflect(_crossover_probability, 0.001, 0.999);

    // call the underlying mutation function
    individual::trampoline_mutation(p);
}

void evolutionary_algorithm::individual::reflect(double &v, double lb, double ub) {
    // reflect is just a better version of truncating
    while (v < lb || v > ub) {
        if (v < lb) {
            v += 2 * (lb - v);
        }
        if (v > ub) {
            v -= 2 * (v - ub);
        }
    }
    return;
}

typename evolutionary_algorithm::individual
evolutionary_algorithm::individual::trampoline_crossover(nutrition_facts &p,
                                              individual &rhs){

    individual child = this->individual::crossover(p, rhs);

    return child;

}


typename evolutionary_algorithm::individual
evolutionary_algorithm::individual::crossover(nutrition_facts &p,
                                                  individual &rhs) {
    // crossover search parameters that might influence crossover
    std::uniform_real_distribution<double> u(0.0, 1.0);

    double alpha = u(evolutionary_algorithm::_generator);
    double child_mutation_strength = (alpha * this->_mutation_strength) +
                                     ((1 - alpha) * rhs._mutation_strength);

    alpha = u(evolutionary_algorithm::_generator);
    double child_crossover_probability =
        (alpha * this->_crossover_probability) +
        ((1 - alpha) * rhs._crossover_probability);

    // call the underlying crossover function
    individual child = this->individual::trampoline_crossover(p, rhs);

    // copy the values to the new born child
    child._mutation_strength = child_mutation_strength;
    child._crossover_probability = child_crossover_probability;

    return child;
}

