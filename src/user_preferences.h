

#ifndef NUTRITION_PREFERENCES_H
#define NUTRITION_PREFERENCES_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <unordered_set>
#include <vector>

namespace predictions {
    class user_preferences {
      public:
        /// Setters
        void setName(std::string nome);
        void setAge(int idade);
        void setWeight(float peso);
        void setHigh(double altura);
        void setCaloriesBurn(int gastoEnergetico);
        void setHobbyes(std::vector<std::string> _localHobbies);
        void setMealsTime(std::vector<int> _horariosRefeicao);
        void SetCaloriesTarget(double localTarget);
        void setGender(int genero);
        void setPais(std::string pais);
        void setBreakfastPreferences(std::vector<int> _idInput);
        void setBreakfastPreferencesPortions(std::vector<int> _portionsInput);
        void setSnack1Preferences(std::vector<int> _entradasID);
        void setSnack1PortionPreferences(std::vector<int> _portionsInput);
        void setLunchPreferences(std::vector<int> _idInput);
        void setLunchPortionsPreferences(std::vector<int> _portionsInput);
        void setSnack2Preferences(std::vector<int> _idInput);
        void setSnack2PreferencesPortions(std::vector<int> _portionsInput);
        void setDinnerPreferences(std::vector<int> _idInput);
        void setDinnerPreferencesPortions(std::vector<int> _entradasPorcoes);
        void setSupperPreferences(std::vector<int> _idInput);
        void setSupperPreferencesPortions(std::vector<int> _portionsInput);

        /// Getters
        // item-item collaborative filter
        std::vector<std::vector<double>>
        normalize(std::vector<std::vector<int>> _preferencesMatrix,
                  std::vector<std::vector<int>> _portionsPreferences);

        std::vector<double>
        euclidean_distance(std::vector<std::vector<int>> _preferencesMatrix,
                           std::vector<std::vector<double>> _normalizedRank,
                           size_t size);

        std::vector<std::vector<double>>
        similarity_cosines(std::vector<std::vector<double>> _normalizedRank,
                           std::vector<std::vector<int>> _idsMatrix,
                           std::vector<double> l2, size_t size);
        std::vector<double>
        score_calc(std::vector<std::vector<double>> _normalizedRank,
                   std::vector<std::vector<double>> _itensMatrix,
                   std::vector<std::vector<int>> _idsMatrix, int user, size_t size);
        ///Search for itens consume similarity
        int searchSimilarItensToBreakfast(
            int userSimilarity, std::vector<int> _localBreakfastPreferences,
            std::vector<std::vector<int>> __breakfastPreferencesMatrix);
        int snack1SimilaritySearch(
            int similarUser, std::vector<int> _localBreakfastPreferences,
            std::vector<std::vector<int>> _breakfastPreferencesMatrix,
            std::vector<int> _localSnack1Preferences,
            std::vector<std::vector<int>> _snack1PreferencesMatrix);
        int lunchSimilaritySearch(
            int usuarioSemelhante, std::vector<int> _localBreakfastPreferences,
            std::vector<std::vector<int>> _matrizPreferenciasDesjejum,
            std::vector<int> _localSnack1Preferences,
            std::vector<std::vector<int>> _matrizPreferenciasLanche1,
            std::vector<int> _localLunchPreferences,
            std::vector<std::vector<int>> _matrizPreferenciasAlmoco);
        int snack2SimilaritySearch(
            int usuarioSemelhante, std::vector<int> _localBreakfastPreferences,
            std::vector<std::vector<int>> _breakfastPreferenceMatrix,
            std::vector<int> _localSnack1Preferences,
            std::vector<std::vector<int>> _matrizPreferenciasLanche1,
            std::vector<int> _localLunchPreferences,
            std::vector<std::vector<int>> _matrizPreferenciasAlmoco,
            std::vector<int> _localSnack2Preferences,
            std::vector<std::vector<int>> _matrizPreferenciasLanche2);
        int dinnerSimilaritySearch(
            int similarUser, std::vector<int> _localBreakfastPreferences,
            std::vector<std::vector<int>> _breakfastPreferenceMatrix,
            std::vector<int> _localSnack1Preferences,
            std::vector<std::vector<int>> __snack1PreferencesMatrix,
            std::vector<int> _localLunchPreferences,
            std::vector<std::vector<int>> __lunchPreferencesMatrix,
            std::vector<int> _localSnack2Preferences,
            std::vector<std::vector<int>> __snack2PreferencesMatrix,
            std::vector<int> _localDinnerPreferences,
            std::vector<std::vector<int>> __dinnerPreferencesMatrix);
        //Print considered user
        void printScreenUser(const predictions::user_preferences& u);
        void printScreenFile(const predictions::user_preferences& u, std::ofstream &fout);
        [[nodiscard]] int getAge() const;
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] double getWeight() const;
        [[nodiscard]] double getHigh() const;
        [[nodiscard]] int getCaloriesBurn() const;
        [[nodiscard]] std::vector<std::string> getHobbies() const;
        [[nodiscard]] std::string getHobbie(int i) const;
        [[nodiscard]] double getImc() const;
        [[nodiscard]] std::vector<int> getMealsTime() const;
        [[nodiscard]] int getMealstimeHour(int i) const;
        [[nodiscard]] int getNumberOfMealsTime() const;
        [[nodiscard]] double getTarget() const;
        [[nodiscard]] int getGender() const;
        [[nodiscard]] std::vector<int> getBreakfastPreferences() const;
        [[nodiscard]] std::vector<int> getBreakfastPreferencesPortions() const;
        [[nodiscard]] std::vector<int> getSnack1Preferences() const;
        [[nodiscard]] std::vector<int> getSnack1PortionPreferences() const;
        [[nodiscard]] std::vector<int> getLunchPreferences() const;
        [[nodiscard]] std::vector<int> getLunchPortionsPreferences() const;
        [[nodiscard]] std::vector<int> getSnack2Preferences() const;
        [[nodiscard]] std::vector<int> getSnack2PreferencesPortions() const;
        [[nodiscard]] std::vector<int> getDinnerPreferences() const;
        [[nodiscard]] std::vector<int> getDinnerPreferencesPortions() const;
        [[nodiscard]] std::vector<int> getPreferenciasCeia() const;
        [[nodiscard]] std::vector<int> getPorcaoPreferenciasCeia() const;

      private:
        int age;
        int gender;
        int caloriesBurn;
        float weight;
        double high;
        double target;
        std::string name;
        std::vector<int> _mealsTime;
        std::vector<std::string> _hobbies;
        std::vector<int> _breakfastPreferences;
        std::vector<int> _breakfastPreferencesPortions;
        std::vector<int> _snack1Preferences;
        std::vector<int> _snack1PreferencesPortions;
        std::vector<int> _lunchPreferences;
        std::vector<int> _lunchPreferencesPortions;
        std::vector<int> _snack2Preferences;
        std::vector<int> _snack2PreferencesPortions;
        std::vector<int> _dinnerPreferences;
        std::vector<int> _dinnerPreferencesPortions;
        std::vector<int> _supperPreferences;
        std::vector<int> _supperPreferencesPortions;
    };

}

/// prints the final diet on the screen and in the file
void printMenu(const predictions::user_preferences &u, std::ofstream &fout);

#endif // NUTRITION_PREFERENCES_H
