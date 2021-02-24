#include "nutrition_facts.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <algorithm>
//#include <numeric>
#include <utility>

std::mt19937 nutrition_facts::_generator =
    std::mt19937(static_cast<unsigned long>(
                     std::chrono::system_clock::now().time_since_epoch().count()) | std::random_device()());

nutrition_facts::nutrition_facts(size_t n,
                                 double userCaloriesTarget, std::vector<int> _userMealstimes)
    : _calories(n, 0), _cost(n, 0), _revanues(n, "0"), _type(n, "0"), _limit(0),
      _id(n, 0),_protein(n, 0), _lipid(n, 0), _carbo(n, 0), _fiber(n, 0), _calc(n, 0),
      _magnesio(n, 0), _fosfer(n, 0), _ferro(n, 0), _zinc(n, 0),
      _manganes(n, 0),
      _breakfastPreferences(n, 0), _snack1Preferences(n, 0),
      _lunchPreferences(n, 0),_snack2Preferences(n, 0),_dinnerPreferences(n, 0),
      _supperPreferences(n, 0) {

    std::vector<std::string> _drev(n), _dcat(n);
    std::vector<int> _did(n);
    std::vector<double> _dpor(n), _dcal(n), _dcos(n), _dpro(n), _dlip(n),
        _dcol(n), _dcar(n), _dfib(n), _dcac(n), _dmag(n), _dfos(n), _dfer(n),
        _dzin(n), _dman(n);

    //Reading the database instance
    deserialize_nutrition_facts(n, _did, _drev, _dcal, _dcos,
                                _dpro, _dlip, _dcar, _dfib, _dcac, _dmag,
                                _dfos, _dfer, _dzin, _dman, _dcat);


    for (size_t i = 0; i < n; ++i) {
        this->_id[i] = _did[i];
        this->_calories[i] = _dcal[i];
        this->_revanues[i] = _drev[i];
        this->_cost[i] = _dcos[i];
        this->_protein[i] = _dpro[i];
        this->_lipid[i] = _dlip[i];
        this->_carbo[i] = _dcar[i];
        this->_fiber[i] = _dfib[i];
        this->_calc[i] = _dcac[i];
        this->_magnesio[i] = _dmag[i];
        this->_fosfer[i] = _dfos[i];
        this->_ferro[i] = _dfer[i];
        this->_zinc[i] = _dzin[i];
        this->_manganes[i] = _dman[i];
    }

    this->caloriesTarget = userCaloriesTarget;
    this->_mealstimes = std::move(_userMealstimes);

    // Food history generator for prediction
    generate_history(
        _breakfastPreferencesMatrix, _snack1PreferencesMatrix,
        _lunchPreferencesMatrix, _snack2PreferencesMatrix,
        _dinnerPreferencesMatrix, _supperPreferencesMatrix,
        _breakfastPreferencesPortionsMatrix, _snack1PreferencesPortionsMatrix,
        _lunchPreferencesPortionsMatrix, _snack2PreferencesPortionsMatrix,
        _dinnerPreferencesPortionsMatrix, _supperPreferencesPortionsMatrix);

    this->_mealstimeQuantity = static_cast<int>(this->_mealstimes.size());
}
// Set model to be used, affecting the evaluation function
void nutrition_facts::setModelToUse(model localModelUsed){
    this->modelUsed = localModelUsed;
}

// Prints the instance
void nutrition_facts::disp() {
    std::cout << "Otimizador nutritivo moderno" << std::endl;
    std::cout << "Refeiçoes/alimentos: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_revanues[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Calorias: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_calories[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Custo(porcao): ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_cost[i] << " ";
    }

    std::cout << std::endl;
    std::cout << "Proteinas: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_protein[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Gorduras: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_lipid[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Carboidratos: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_carbo[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Fibra: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_fiber[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Calcio: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_calc[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Magnesio: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_magnesio[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Manganes: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_manganes[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Fosforo: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_fosfer[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Ferro: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_ferro[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Zinco: ";
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << this->_zinc[i] << " ";
    }

    std::cout << std::endl;
    // std::cout << "Limit of cost: " << this->_limit << std::endl;
    std::cout << "Daily calories goal: " << this->caloriesTarget << std::endl;
    std::cout << "Number of meals time: " << this->_mealstimeQuantity << std::endl;

    /// Actually the implementation consider 1 or 6 measltime
    if (this->_mealstimeQuantity == 1) {
        std::cout << this->_mealstimes[0] << " Refeiçao unica" << std::endl;
    } else if (this->_mealstimeQuantity == 2) {
        std::cout << this->_mealstimes[0] << " Desjejum" << std::endl;
        std::cout << this->_mealstimes[1] << " Almoço" << std::endl;
    } else if (this->_mealstimeQuantity == 3) {
        std::cout << this->_mealstimes[0] << " Desjejum" << std::endl;
        std::cout << this->_mealstimes[1] << " Almoço" << std::endl;
        std::cout << this->_mealstimes[2] << " Jantar" << std::endl;
    } else if (this->_mealstimeQuantity == 4) {
        std::cout << this->_mealstimes[0] << " Desjejum" << std::endl;
        std::cout << this->_mealstimes[1] << " Almoço" << std::endl;
        std::cout << this->_mealstimes[2] << " Lanche" << std::endl;
        std::cout << this->_mealstimes[3] << " Jantar" << std::endl;
    } else if (this->_mealstimeQuantity == 5) {
        std::cout << this->_mealstimes[0] << " Desjejum" << std::endl;
        std::cout << this->_mealstimes[1] << " Lanche" << std::endl;
        std::cout << this->_mealstimes[2] << " Almoço" << std::endl;
        std::cout << this->_mealstimes[3] << " Lanche" << std::endl;
        std::cout << this->_mealstimes[4] << " Jantar" << std::endl;

    } else if (this->_mealstimeQuantity == 6) {
        std::cout << this->_mealstimes[0]
                  << " Desjejum, goal: " << this->getBreakfastTarget()
                  << std::endl;
        std::cout << this->_mealstimes[1]
                  << " Lanche 1, goal: " << this->getCaloricTargetSnack1() << std::endl;
        std::cout << this->_mealstimes[2]
                  << " Almoço, goal: " << this->getCaloricTargetLunch() << std::endl;
        std::cout << this->_mealstimes[3]
                  << " Lanche 2, goal: " << this->getCaloricTargetSnack2() << std::endl;
        std::cout << this->_mealstimes[4]
                  << " Jantar, goal: " << this->getDinnerCaloriesTarget() << std::endl;
        std::cout << this->_mealstimes[5]
                  << " Ceia, goal: " << this->getSupperCaloricTarget() << std::endl;
    } else if (this->_mealstimeQuantity == 7) {
        std::cout << this->_mealstimes[0] << " Desjejum" << std::endl;
        std::cout << this->_mealstimes[1] << " Lanche 1" << std::endl;
        std::cout << this->_mealstimes[2] << " Almoço" << std::endl;
        std::cout << this->_mealstimes[3] << " Lanche 2" << std::endl;
        std::cout << this->_mealstimes[4] << " Lanche 3" << std::endl;
        std::cout << this->_mealstimes[5] << " Jantar" << std::endl;
        std::cout << this->_mealstimes[6] << " Ceia" << std::endl;

    } else if (this->_mealstimeQuantity == 8) {
        std::cout << this->_mealstimes[0] << " Desjejum";
        std::cout << this->_mealstimes[1] << " Lanche 1";
        std::cout << this->_mealstimes[2] << " Lanche 2";
        std::cout << this->_mealstimes[3] << " Almoço";
        std::cout << this->_mealstimes[4] << " Lanche 3";
        std::cout << this->_mealstimes[5] << " Lanche 4";
        std::cout << this->_mealstimes[6] << " Jantar";
        std::cout << this->_mealstimes[7] << " Ceia";
    }

    std::cout << std::endl;
}

// Save on file the instance
void nutrition_facts::dispFile(std::ofstream &fout, double target,
                               int mealstime_quantity) {
    fout << "Otmizador Nutritivo Dinamic e Interativo" << std::endl;
    fout << "Alimentos: ";
    // for (int i = 0; i < this->size(); ++i) fout << this->_revanues[i] << ","
    // << this->_calories[i] << "Kcal,R$" << this->_cost[i]<< std::endl;
    fout << "Meta calorica: " << target << std::endl;
    this->setMeta(target);
    std::cout << "Numero de refeiçoes: " << mealstime_quantity
              << ",calorias:" << target << std::endl;
    if (mealstime_quantity == 1) {
        fout << this->_mealstimes[0] << " Refeiçao unica" << std::endl;
    } else if (mealstime_quantity == 2) {
        fout << this->_mealstimes[0] << " Desjejum" << std::endl;
        fout << this->_mealstimes[1] << " Almoço" << std::endl;
    } else if (mealstime_quantity == 3) {
        fout << this->_mealstimes[0] << " Desjejum" << std::endl;
        fout << this->_mealstimes[1] << " Almoço" << std::endl;
        fout << this->_mealstimes[2] << " Jantar" << std::endl;
    } else if (mealstime_quantity == 4) {
        fout << this->_mealstimes[0] << " Desjejum" << std::endl;
        fout << this->_mealstimes[1] << " Almoço" << std::endl;
        fout << this->_mealstimes[2] << " Lanche" << std::endl;
        fout << this->_mealstimes[3] << " Jantar" << std::endl;
    } else if (mealstime_quantity == 5) {
        fout << this->_mealstimes[0] << " Desjejum" << std::endl;
        fout << this->_mealstimes[1] << " Lanche" << std::endl;
        fout << this->_mealstimes[2] << " Almoço" << std::endl;
        fout << this->_mealstimes[3] << " Lanche" << std::endl;
        fout << this->_mealstimes[4] << " Jantar" << std::endl;

    } else if (mealstime_quantity == 6) {
        fout << this->_mealstimes[0]
             << " Desjejum, goal: " << this->getBreakfastTarget() << std::endl;
        fout << this->_mealstimes[1]
             << " Lanche 1, goal: " << this->getCaloricTargetSnack1() << std::endl;
        fout << this->_mealstimes[2] << " Almoço, goal: " << this->getCaloricTargetLunch()
             << std::endl;
        fout << this->_mealstimes[3]
             << " Lanche 2, goal: " << this->getCaloricTargetSnack2() << std::endl;
        fout << this->_mealstimes[4] << " Jantar, goal: " << this->getDinnerCaloriesTarget()
             << std::endl;
        fout << this->_mealstimes[5] << " Ceia, goal: " << this->getSupperCaloricTarget()
             << std::endl;
    } else if (mealstime_quantity == 7) {
        fout << this->_mealstimes[0] << " Desjejum" << std::endl;
        fout << this->_mealstimes[1] << " Lanche 1" << std::endl;
        fout << this->_mealstimes[2] << " Almoço" << std::endl;
        fout << this->_mealstimes[3] << " Lanche 2" << std::endl;
        fout << this->_mealstimes[4] << " Lanche 3" << std::endl;
        fout << this->_mealstimes[5] << " Jantar" << std::endl;
        fout << this->_mealstimes[6] << " Ceia" << std::endl;

    } else if (mealstime_quantity == 8) {
        fout << this->_mealstimes[0] << " Desjejum";
        fout << this->_mealstimes[1] << " Lanche 1";
        fout << this->_mealstimes[2] << " Lanche 2";
        fout << this->_mealstimes[3] << " Almoço";
        fout << this->_mealstimes[4] << " Lanche 3";
        fout << this->_mealstimes[5] << " Lanche 4";
        fout << this->_mealstimes[6] << " Jantar";
        fout << this->_mealstimes[7] << " Ceia";
    }
    fout << std::endl;
}

///Getters
size_t nutrition_facts::size() { return this->_calories.size(); }

std::vector<double> nutrition_facts::getCalories() { return this->_calories; }

std::vector<double> nutrition_facts::getPreferencesDesjejum() {return this->_breakfastPreferences;}

std::vector<double> nutrition_facts::getSnack1Preferences() {return this->_snack1Preferences;}

std::vector<double> nutrition_facts::getLunchPreferences() { return this->_lunchPreferences;}

std::vector<double> nutrition_facts::getSnack2Preferences() {return this->_snack2Preferences;}

std::vector<double> nutrition_facts::getDinnerPreferences() {return this->_dinnerPreferences;}

std::vector<double> nutrition_facts::getSupperPreferences() {return this->_supperPreferences;}

double nutrition_facts::changeTarget() { return _mudancaMeta; }

double nutrition_facts::calories(size_t i) { return this->_calories[i]; }

int nutrition_facts::id(size_t i) { return this->_id[i]; }

double nutrition_facts::protein(size_t i) { return this->_protein[i]; }

double nutrition_facts::lipid(size_t i) { return this->_lipid[i]; }

double nutrition_facts::fiber(size_t i) { return this->_fiber[i]; }

double nutrition_facts::calc(size_t i) { return this->_calc[i]; }

double nutrition_facts::magnesio(size_t i) { return this->_magnesio[i]; }

double nutrition_facts::fosfer(size_t i) { return this->_fosfer[i]; }

double nutrition_facts::ferro(size_t i) { return this->_ferro[i]; }

double nutrition_facts::carbo(size_t i) { return this->_carbo[i]; }

double nutrition_facts::zinc(size_t i) { return this->_zinc[i]; }

double nutrition_facts::manganes(size_t i) { return this->_manganes[i]; }

std::string nutrition_facts::revanues(size_t i) { return this->_revanues[i]; }

std::string nutrition_facts::type(size_t i) { return this->_type[i]; }

double nutrition_facts::cost(size_t i) { return this->_cost[i]; }

int nutrition_facts::numberOfMealstime() { return this->_mealstimeQuantity; }

double nutrition_facts::getmeta() { return this->caloriesTarget; }

int nutrition_facts::turn() { return this->_turno; }

double nutrition_facts::getBreakfastTarget() { return (this->caloriesTarget / 100 * 20); }

double nutrition_facts::getCaloricTargetSnack1() { return (this->caloriesTarget / 100 * 10); }

double nutrition_facts::getCaloricTargetLunch() { return (this->caloriesTarget / 100 * 30); }

double nutrition_facts::getCaloricTargetSnack2() { return (this->caloriesTarget / 100 * 10); }

double nutrition_facts::getDinnerCaloriesTarget() { return (this->caloriesTarget / 100 * 20); }

double nutrition_facts::getSupperCaloricTarget() { return (this->caloriesTarget / 100 * 10); }

int nutrition_facts::getModel() { return this->modelUsed; }

std::vector<int> nutrition_facts::getHour() { return this->_mealstimes; }

std::vector<std::string> nutrition_facts::getRecipies() {return this->_revanues;}

int nutrition_facts::getHour(int horario) {return this->_mealstimes[horario];}
// Desjejum
std::vector<std::vector<int>> nutrition_facts::getBreakfastPreferencesMatrix() {return this->_breakfastPreferencesMatrix;}
// Lanche da manha
std::vector<std::vector<int>> nutrition_facts::getSnack1PreferencesMatrix() {return this->_snack1PreferencesMatrix;}
std::vector<std::vector<int>> nutrition_facts::getSnack1PortionPreferencesMatrix() {return this->_snack1PreferencesPortionsMatrix;}
// Almoco
std::vector<std::vector<int>> nutrition_facts::getLunchPreferencesMatrix() {return this->_lunchPreferencesMatrix;}
std::vector<std::vector<int>> nutrition_facts::getLunchPortionPreferencesMatrix() {return this->_lunchPreferencesPortionsMatrix;}
// Lanche da tarde
std::vector<std::vector<int>> nutrition_facts::getSnack2PreferencesMatrix() {return this->_snack2PreferencesMatrix;}
std::vector<std::vector<int>> nutrition_facts::getSnack2PortionsPreferencesMatrix() {return this->_snack2PreferencesPortionsMatrix;}
// Jantar
std::vector<std::vector<int>> nutrition_facts::getDinnerPreferencesMatrix() {return this->_dinnerPreferencesMatrix;}
std::vector<std::vector<int>> nutrition_facts::getDinnerPortionsPreferencesMatrix() {return this->_dinnerPreferencesPortionsMatrix;}
// Ceia
std::vector<std::vector<int>> nutrition_facts::getSupperPreferencesMatrix() {return this->_supperPreferencesMatrix;}
std::vector<std::vector<int>> nutrition_facts::getSupperPortionsPreferencesMatrix() {return this->_supperPreferencesPortionsMatrix;}

///Setters
void nutrition_facts::setSnack1Preferences(std::vector<double> _preferencesLanche1) { this->_snack1Preferences = std::move(_preferencesLanche1);}

void nutrition_facts::setLunchPreferences(std::vector<double> _preferencesAlmoco) { this->_lunchPreferences = std::move(_preferencesAlmoco);}

void nutrition_facts::setSnack2Preferences(std::vector<double> _preferencesLanche2) { this->_snack2Preferences = std::move(_preferencesLanche2);}

void nutrition_facts::setDinnerPreferences(std::vector<double> _preferencesJantar) { this->_dinnerPreferences = std::move(_preferencesJantar);}

void nutrition_facts::setSupperPreferences(std::vector<double> _preferencesCeia) { this->_supperPreferences = std::move(_preferencesCeia);}

void nutrition_facts::setCaloriesTargetChangeFlag(double mudanca) { this->_mudancaMeta = -mudanca;}

void nutrition_facts::setMeta(double metaNova) { this->caloriesTarget = metaNova; }

void nutrition_facts::setTurn(int newShift) { this->_turno = newShift; }

void nutrition_facts::deserialize_nutrition_facts(size_t size, std::vector<int> &_did,
    std::vector<std::string> &_drev,
    std::vector<double> &_dcal, std::vector<double> &_dcos,
    std::vector<double> &_dpro, std::vector<double> &_dlip,
    std::vector<double> &_dcar,
    std::vector<double> &_dfib, std::vector<double> &_dcac,
    std::vector<double> &_dmag, std::vector<double> &_dfos,
    std::vector<double> &_dfer, std::vector<double> &_dzin,
    std::vector<double> &_dman, std::vector<std::string> &_dcat) {

    // Variables
    std::string line;
    std::string stringRevanue;
    std::string stringNumber;
    std::stringstream ss(line);

    // Open File
    std::ifstream fin(LOCAL_PATH, std::ios::in);

    if (fin.is_open()) {
        // ID
        for (size_t i = 0; i < size; i++) {
            int id = 0;
            fin >> id;
            _did[i] = id;
        }
        // Revanue
        for (size_t i = 0; i < size; i++) {
            std::string revanue;
            fin >> revanue;
            _drev[i] = revanue;
        }
        // Categorie
        for (size_t i = 0; i < size; i++) {
            std::string category;
            fin >> category;
            _dcat[i] = category;
        }
        // Price
        for (size_t i = 0; i < size; i++) {
            double cost = 0;
            fin >> cost;
            _dcos[i] = cost;
        }
        // Calories
        for (size_t i = 0; i < size; i++) {
            double calories;
            fin >> calories;
            _dcal[i] = calories;
        }
        // Proteinas
        for (size_t i = 0; i < size; i++) {
            double protein;
            fin >> protein;
            _dpro[i] = protein;
        }
        // Lipideos
        for (size_t i = 0; i < size; i++) {
            double lipids;
            fin >> lipids;
            _dlip[i] = lipids;
        }
        // Carboidratos
        for (unsigned long i = 0; i < size; i++) {
            double carboidrats;
            fin >> carboidrats;
            _dcar[i] = carboidrats;
        }
        // Fibra
        for (size_t i = 0; i < size; i++) {
            double fibers;
            fin >> fibers;
            _dfib[i] = fibers;
        }
        // Calcio
        for (size_t i = 0; i < size; i++) {
            double calcium;
            fin >> calcium;
            _dcac[i] = calcium;
        }
        // Magnesio
        for (size_t i = 0; i < size; i++) {
            double magnesio;
            fin >> magnesio;
            _dmag[i] = magnesio;
        }
        // Manganes
        for (size_t i = 0; i < size; i++) {
            double manganes;
            fin >> manganes;
            _dman[i] = manganes;
        }
        // Fosforo
        for (size_t i = 0; i < size; i++) {
            double fosfer;
            fin >> fosfer;
            _dfos[i] = fosfer;
        }
        // Ferro
        for (size_t i = 0; i < size; i++) {
            double ferro;
            fin >> ferro;
            _dfer[i] = ferro;
        }
        // Zinco
        for (size_t i = 0; i < size; i++) {
            double zinco;
            fin >> zinco;
            _dzin[i] = zinco;
        }
    } else {
        throw std::runtime_error(
            "serialization::deserialize_nutrition_facts: can't find file "  LOCAL_PATH
            );
    }
}

void nutrition_facts::generate_history(
    std::vector<std::vector<int>> &__breakfastPreferencesMatrix,
    std::vector<std::vector<int>> &__snack1PreferencesMatrix,
    std::vector<std::vector<int>> &__lunchPreferencesMatrix,
    std::vector<std::vector<int>> &__snack2PreferencesMatrix,
    std::vector<std::vector<int>> &__dinnerPreferencesMatrix,
    std::vector<std::vector<int>> &__supperPreferencesMatrix,
    std::vector<std::vector<int>> &__breakfastPortionsPreferencesMatrix,
    std::vector<std::vector<int>> &__snack1PortionsPreferencesMatrix,
    std::vector<std::vector<int>> &__lunchPortionsPreferencesMatrix,
    std::vector<std::vector<int>> &__snack2PortionsPreferencesMatrix,
    std::vector<std::vector<int>> &__dinnerPortionsPreferencesMatrix,
    std::vector<std::vector<int>> &__supperPortionsPreferencesMatrix) {

    // Number of users considered on history instance
    int n_users = 10;

    ///This range must consider the food intance
    // ID dos alelos fo TACO food instance
    /* Amendoas 1 ao 11
     * Bebidas 12 ao 27
     * Carboidratos simples 28 ao 89
     * Carboidratos complexos 90 ao 137
     * Frutas 139 a 234
     * Graos 235 ao 268
     * Lacteos 269 ao 292
     * Oleos 293 a 306
     * Proteinas 307 a 492
     * Verduras 493 a 568
     */

    std::uniform_int_distribution<int> almonds(1, 11);
    std::uniform_int_distribution<int> drinks(12, 27);
    std::uniform_int_distribution<int> simpleCarbo(28, 89);
    std::uniform_int_distribution<int> complexCarbo(90, 138);
    std::uniform_int_distribution<int> fruits(139, 234);
    std::uniform_int_distribution<int> grains(235, 268);
    std::uniform_int_distribution<int> dairy(269, 292);
    std::uniform_int_distribution<int> oils(293, 306);
    std::uniform_int_distribution<int> proteins(307, 492);
    std::uniform_int_distribution<int> vegetables(493, 568);

    std::uniform_int_distribution<int> portions(1, 3);

    for (int i = 0; i < n_users; i++) {
        std::vector<int> _food1, _food2, _food3, _food4, _food5, _food6;
        std::vector<int> _portion1, _portion2, _portion3, _portion4, _portion5, _portion6;

        // breakfast
        _food1.push_back(dairy(_generator));
        _food1.push_back(fruits(_generator));
        _food1.push_back(simpleCarbo(_generator));
        __breakfastPreferencesMatrix.push_back(_food1);
        for (int j = 0; j < 3; j++)
            _portion1.push_back(portions(_generator));
        __breakfastPortionsPreferencesMatrix.push_back(_portion1);

        // Snack 1
        _food2.push_back(almonds(_generator));
        _food2.push_back(fruits(_generator));
        _food2.push_back(oils(_generator));
        __snack1PreferencesMatrix.push_back(_food2);
        for (int j = 0; j < 3; j++)
            _portion2.push_back(portions(_generator));
        __snack1PortionsPreferencesMatrix.push_back(_portion2);

        // Lunch
        _food3.push_back(proteins(_generator));
        _food3.push_back(complexCarbo(_generator));
        _food3.push_back(drinks(_generator));
        _food3.push_back(grains(_generator));
        _food3.push_back(vegetables(_generator));
        _food3.push_back(vegetables(_generator));
        _food3.push_back(vegetables(_generator));
        __lunchPreferencesMatrix.push_back(_food3);
        for (int j = 0; j < 7; j++)
            _portion3.push_back(portions(_generator));
        __lunchPortionsPreferencesMatrix.push_back(_portion3);

        // Snack 2
        _food4.push_back(almonds(_generator));
        _food4.push_back(fruits(_generator));
        _food4.push_back(oils(_generator));
        __snack2PreferencesMatrix.push_back(_food4);
        for (int j = 0; j < 3; j++)
            _portion4.push_back(portions(_generator));
        __snack2PortionsPreferencesMatrix.push_back(_portion4);

        // Dinner
        _food5.push_back(proteins(_generator));
        _food5.push_back(complexCarbo(_generator));
        _food5.push_back(drinks(_generator));
        _food5.push_back(grains(_generator));
        _food5.push_back(vegetables(_generator));
        _food5.push_back(vegetables(_generator));
        _food5.push_back(vegetables(_generator));
        __dinnerPreferencesMatrix.push_back(_food5);
        for (int j = 0; j < 7; j++)
            _portion5.push_back(portions(_generator));
        __dinnerPortionsPreferencesMatrix.push_back(_portion5);

        // Supper
        _food6.push_back(almonds(_generator));
        _food6.push_back(simpleCarbo(_generator));
        __supperPreferencesMatrix.push_back(_food6);
        for (int j = 0; j < 2; j++)
            _portion6.push_back(portions(_generator));
        __supperPortionsPreferencesMatrix.push_back(_portion6);
    }
}
