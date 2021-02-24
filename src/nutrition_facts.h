#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#ifndef ComEvoGlauber_opnP_H
#define ComEvoGlauber_opnP_H

/// \class This class describes the problem
/// It contains the nutrition facts for each for each food
/// and the preference tables for each of them.
class nutrition_facts {
  public:
    /// Creates the problem based on the instances
    nutrition_facts(size_t n, double userCaloriesTarget,
                    std::vector<int> _userMealstimes);

    /// Prints the relevant problem information
    void disp();
    void dispFile(std::ofstream &fout, double target, int mealstime_quantity);

    /// Read external file (txt) containing the recipes and their nutritional
    /// facts
    static void deserialize_nutrition_facts(size_t size, std::vector<int> &_did,
        std::vector<std::string> &_drev,
        std::vector<double> &_dcal, std::vector<double> &_dcos,
        std::vector<double> &_dpro, std::vector<double> &_dlip,
        std::vector<double> &_dcar,
        std::vector<double> &_dfib, std::vector<double> &_dcac,
        std::vector<double> &_dmag, std::vector<double> &_dfos,
        std::vector<double> &_dfer, std::vector<double> &_dzin,
        std::vector<double> &_dman, std::vector<std::string> &_dcat);

    size_t size();
    enum model {STATIC,STATIC_MEASLTIME,DINAMIC_ONLY,DINAMIC_PREFERENCES_PREDICTION};

    /// assigns the direction of the FO (max or min)
    static inline bool is_minimization() { return true; };

    /// setters
    void setModelToUse(model localModelUsed);
    void setCaloriesTargetChangeFlag(double mudanca);
    void setSupperPreferences(std::vector<double> _preferencesCeia);
    void setSnack1Preferences(std::vector<double> _preferencesLanche1);
    void setLunchPreferences(std::vector<double> _preferencesAlmoco);
    void setSnack2Preferences(std::vector<double> _preferencesLanche2);
    void setDinnerPreferences(std::vector<double> _preferencesJantar);
    void setMeta(double i);
    void setTurn(int i);
    /// getters
    int numberOfMealstime();
    int getHour(int horario);
    int id(size_t i);
    int turn();
    std::vector<std::string> getRecipies();
    std::vector<int> getHour();
    std::string revanues(size_t i);
    std::string type(size_t i);
    int getModel();
    double calories(size_t i);
    double cost(size_t i);
    double protein(size_t i);
    double lipid(size_t i);
    double carbo(size_t i);
    double fiber(size_t i);
    double calc(size_t i);
    double magnesio(size_t i);
    double fosfer(size_t i);
    double ferro(size_t i);
    double zinc(size_t i);
    double manganes(size_t i);
    double porcao(size_t i);
    double getmeta();
    double changeTarget();
    double getBreakfastTarget();
    double getCaloricTargetSnack1();
    double getCaloricTargetLunch();
    double getCaloricTargetSnack2();
    double getDinnerCaloriesTarget();
    double getSupperCaloricTarget();
    std::vector<double> getPreferencesDesjejum();
    std::vector<double> getSnack1Preferences();
    std::vector<double> getLunchPreferences();
    std::vector<double> getSnack2Preferences();
    std::vector<double> getDinnerPreferences();
    std::vector<double> getSupperPreferences();
    std::vector<double> getCalories();
    // Desjejum
    std::vector<std::vector<int>> getBreakfastPreferencesMatrix();
    // Lanche da manha
    std::vector<std::vector<int>> getSnack1PreferencesMatrix();
    std::vector<std::vector<int>> getSnack1PortionPreferencesMatrix();
    // Almoco
    std::vector<std::vector<int>> getLunchPreferencesMatrix();
    std::vector<std::vector<int>> getLunchPortionPreferencesMatrix();
    // Lanche da tarde
    std::vector<std::vector<int>> getSnack2PreferencesMatrix();
    std::vector<std::vector<int>> getSnack2PortionsPreferencesMatrix();
    // Jantar
    std::vector<std::vector<int>> getDinnerPreferencesMatrix();
    std::vector<std::vector<int>> getDinnerPortionsPreferencesMatrix();
    // Ceia
    std::vector<std::vector<int>> getSupperPreferencesMatrix();
    std::vector<std::vector<int>> getSupperPortionsPreferencesMatrix();

  public:
    /// Generate a users history based on a random generator
    /// Limited by types and quantities of food per meal (used in literature)
    static void generate_history(
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
        std::vector<std::vector<int>> &__supperPortionsPreferencesMatrix);
  private:
    int _turno;
    int _mealstimeQuantity;

    double caloriesTarget;
    double _mudancaMeta;
    model modelUsed;
    std::vector<int> _mealstimes;

    std::vector<double> _calories;
    std::vector<double> _cost;
    std::vector<std::string> _revanues;
    std::vector<std::string> _type;
    double _limit;
    std::vector<int> _id;
    std::vector<double> _protein;
    std::vector<double> _lipid;
    std::vector<double> _carbo;
    std::vector<double> _fiber;
    std::vector<double> _calc;
    std::vector<double> _magnesio;
    std::vector<double> _fosfer;
    std::vector<double> _ferro;
    std::vector<double> _zinc;
    std::vector<double> _manganes;

    std::vector<double> _breakfastPreferences;
    std::vector<double> _snack1Preferences;
    std::vector<double> _lunchPreferences;
    std::vector<double> _snack2Preferences;
    std::vector<double> _dinnerPreferences;
    std::vector<double> _supperPreferences;
    //std::vector<double> _porcao;
    // Desjejum
    std::vector<std::vector<int>> _breakfastPreferencesMatrix;
    std::vector<std::vector<int>> _breakfastPreferencesPortionsMatrix;
    // Lanche da manha
    std::vector<std::vector<int>> _snack1PreferencesMatrix;
    std::vector<std::vector<int>> _snack1PreferencesPortionsMatrix;
    // Almoco
    std::vector<std::vector<int>> _lunchPreferencesMatrix;
    std::vector<std::vector<int>> _lunchPreferencesPortionsMatrix;
    // Lanche da tarde
    std::vector<std::vector<int>> _snack2PreferencesMatrix;
    std::vector<std::vector<int>> _snack2PreferencesPortionsMatrix;
    // Jantar
    std::vector<std::vector<int>> _dinnerPreferencesMatrix;
    std::vector<std::vector<int>> _dinnerPreferencesPortionsMatrix;
    // Ceia
    std::vector<std::vector<int>> _supperPreferencesMatrix;
    std::vector<std::vector<int>> _supperPreferencesPortionsMatrix;

    static std::mt19937 _generator;
};

#endif // ComEvoGlauber_opnP_HH