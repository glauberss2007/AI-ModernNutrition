
#include "user_preferences.h"

#include <utility>

// Preferences variables
std::vector<int> _breakfastPreferences;
std::vector<int> _breakfastPortionPreferences;
std::vector<int> _snack1Preferences;
std::vector<int> _snack1PortionPreferences;
std::vector<int> _lunchPreferences;
std::vector<int> _lunchPortionPreferences;
std::vector<int> _snack2Preferences;
std::vector<int> _snack2PortionPreferences;
std::vector<int> _dinnerPreferences;
std::vector<int> _dinnerPortionPreferences;
std::vector<int> _supperPreferences;
std::vector<int> _supperPortionPreferences;

//User information set
void predictions::user_preferences::setName(std::string nome) { this->name = std::move(nome); }
void predictions::user_preferences::setAge(int idade) { this->age = idade; }
void predictions::user_preferences::setWeight(float peso) { this->weight = peso; }
void predictions::user_preferences::setHigh(double altura) { this->high = altura; }
void predictions::user_preferences::setCaloriesBurn(int gastoEnergetico) { this->caloriesBurn = gastoEnergetico;}
void predictions::user_preferences::setHobbyes(std::vector<std::string> _localHobbies) {this->_hobbies = std::move(_localHobbies);}
void predictions::user_preferences::setMealsTime(std::vector<int> _horariosRefeicao) { this->_mealsTime = std::move(_horariosRefeicao);}
void predictions::user_preferences::SetCaloriesTarget(double localTarget) { this->target = localTarget; }
void predictions::user_preferences::setGender(int genero) { this->gender = genero; }

//User information get
int predictions::user_preferences::getAge() const { return this->age; }
int predictions::user_preferences::getCaloriesBurn() const {return this->caloriesBurn;}
int predictions::user_preferences::getMealstimeHour(int i) const {return this->_mealsTime[i];}
int predictions::user_preferences::getNumberOfMealsTime() const {return (int)this->_mealsTime.size();}
int predictions::user_preferences::getGender() const { return this->gender; }
double predictions::user_preferences::getWeight() const { return this->weight; }
double predictions::user_preferences::getHigh() const { return this->high; }
double predictions::user_preferences::getImc() const {return (this->weight / ((this->high / 100) * (this->high / 100)));}
double predictions::user_preferences::getTarget() const { return this->target; }
std::string predictions::user_preferences::getName() const { return this->name; }
std::vector<std::string> predictions::user_preferences::getHobbies() const {return this->_hobbies;}
std::string predictions::user_preferences::getHobbie(int i) const {return this->_hobbies[i];}
std::vector<int> predictions::user_preferences::getMealsTime() const {return this->_mealsTime;}

std::vector<std::vector<double>>
predictions::user_preferences::normalize(std::vector<std::vector<int>> _preferencesMatrix,
          std::vector<std::vector<int>> _portionsPreferences) {

    std::vector<std::vector<double>> _matrizRank(_preferencesMatrix.size());
    std::vector<double> _magnetude;
    double aux;

    for (auto & _portionsMatrix : _portionsPreferences) {
        aux = 0;
        for (int j : _portionsMatrix) {
            aux += (j * j);
        }
        _magnetude.push_back(sqrt(aux));
    }

    for (unsigned long i = 0; i < _preferencesMatrix.size(); i++) {
        for (unsigned long j = 0; j < _preferencesMatrix[i].size(); j++) {
            _matrizRank[i].push_back(_portionsPreferences[i][j] / _magnetude[i]);
        }
    }
    return _matrizRank;
}

std::vector<double> predictions::user_preferences::euclidean_distance(std::vector<std::vector<int>> _preferencesMatrix,
                                       std::vector<std::vector<double>> _normalizedRank,
                                       size_t size) {

    std::vector<double> l2;
    double aux;
    std::vector<int>::iterator it;
    int index;

    for (int i = 1; i <= (int)size; i++) {
        aux = 0;
        for (int j = 0; j < (int)_preferencesMatrix.size(); j++) {
            // Check if element i exists in user j itens
            it = std::find(_preferencesMatrix[j].begin(),
                           _preferencesMatrix[j].end(), i);
            if (it != _preferencesMatrix[j].end()) {
                // Get index of element from iterator
                index = (int)std::distance(_preferencesMatrix[j].begin(), it);
                aux += _normalizedRank[j][index] * _normalizedRank[j][index];
            }
        }
        aux = sqrt(aux);
        l2.push_back(aux);
    }
    return l2;
}

std::vector<std::vector<double>>
predictions::user_preferences::similarity_cosines(std::vector<std::vector<double>> _normalizedRank,
                   std::vector<std::vector<int>> _idsMatrix,
                   std::vector<double> l2, size_t size) {

    std::vector<std::vector<double>> itensMatrix(size,
                                                 std::vector<double>(size));
    std::vector<std::vector<double>> fullNormalizedMatrix(
            _normalizedRank.size(), std::vector<double>(size));

    for (unsigned long i = 0; i < _idsMatrix.size(); i++) {
        for (unsigned long j = 0; j < _idsMatrix[i].size(); j++) {
            fullNormalizedMatrix[i][_idsMatrix[i][j] - 1] =
                _normalizedRank[i][j];
        }
    }

    std::vector<std::vector<double>> itens(
        size, std::vector<double>(fullNormalizedMatrix.size()));

    for (unsigned long i = 0; i < size; i++) {
        for (unsigned long j = 0; j < fullNormalizedMatrix.size(); j++) {
            itens[i][j] = fullNormalizedMatrix[j][i];
        }
    }

    for (unsigned long i = 0; i < size; i++) {
        for (unsigned long j = 0; j < size; j++) {
            double aux = 0;
            for (unsigned long k = 0; k < _normalizedRank.size(); k++) {
                aux += (itens[i][k] * itens[j][k]);
            }
            aux = l2[i] * l2[j] == 0 ? 0 : aux / (l2[i] * l2[j]);
            itensMatrix[i][j] = aux;
        }
    }
    return itensMatrix;
}

std::vector<double>
predictions::user_preferences::score_calc(std::vector<std::vector<double>> _normalizedRank,
           std::vector<std::vector<double>> _itensMatrix,
           std::vector<std::vector<int>> _idsMatrix, int user, size_t size) {
    std::vector<double> score(size);
    std::vector<std::vector<double>> fullNormalizedMatrix(
            _normalizedRank.size(), std::vector<double>(size));
    for (unsigned long i = 0; i < _idsMatrix.size(); i++) {
        for (unsigned long j = 0; j < _idsMatrix[i].size(); j++) {
            fullNormalizedMatrix[i][_idsMatrix[i][j] - 1] =
                _normalizedRank[i][j];
        }
    }

    double aux, aux2;
    for (unsigned long i = 0; i < _itensMatrix.size(); i++) {
        aux2 = 0;
        aux = 0;
        for (unsigned long j = 0; j < size; j++)
            aux2 += _itensMatrix[i][j];
        for (unsigned long j = 0; j < size; j++)
            aux += (fullNormalizedMatrix[user][j] * _itensMatrix[i][j]);
        aux = aux2 == 0 ? 0 : aux / aux2;
        score[i] = aux;
    }
    return score;
}

void predictions::user_preferences::printScreenUser(const predictions::user_preferences& u) {
    std::cout << "Dieta para o usuario(a): " << u.getName() << std::endl;
    //std::cout << "Idade: " << u.getAge() << std::endl;
    //std::cout << "Altura: " << u.getHigh() << std::endl;
    //std::cout << "Gasto energetico: " << u.getCaloriesBurn() << std::endl;
    //std::cout << "IMC: " << u.getImc() << std::endl;
    std::cout << "Horarios de refeiçao: ";
    for (int i = 0; i < u.getNumberOfMealsTime(); i++)
        std::cout << u.getMealstimeHour(i) << " ";
    std::cout << std::endl;
    std::cout << std::endl;
}

void predictions::user_preferences::printScreenFile(const predictions::user_preferences& u, std::ofstream &fout) {
    fout << "Dieta para o usuario(a): " << u.getName() << std::endl;
    //fout << "Idade: " << u.getAge() << std::endl;
    //fout << "Altura: " << u.getHigh() << std::endl;
    //fout << "Gasto energetico: " << u.getCaloriesBurn() << std::endl;
    //fout << "IMC: " << u.getImc() << std::endl;
    fout << "Horarios de refeiçao: ";
    for (int i = 0; i < u.getNumberOfMealsTime(); i++)
        fout << u.getMealstimeHour(i) << " ";
    fout << std::endl;
    fout << std::endl;
}

int predictions::user_preferences::searchSimilarItensToBreakfast(
        int userSimilarity, std::vector<int> _localBreakfastPreferences,
        std::vector<std::vector<int>> __breakfastPreferencesMatrix) {

    std::vector<int>::iterator comparation, it;
    std::vector<int> similarity(_localBreakfastPreferences.size());
    int bestSimilarityScore = 0;
    int similarUser = userSimilarity;
    for (unsigned long i = 0; i < __breakfastPreferencesMatrix.size(); i++) {
        int aux = 0;
        comparation = std::set_intersection(
            _localBreakfastPreferences.begin(),
            _localBreakfastPreferences.end(),
                __breakfastPreferencesMatrix[i].begin(),
                __breakfastPreferencesMatrix[i].end(), similarity.begin());

        similarity.resize(comparation - similarity.begin());

        for (it = similarity.begin(); it != comparation; ++it)
            aux = aux + 1;
        if (aux > bestSimilarityScore)
            similarUser = i;
    }
    return similarUser;
}

int predictions::user_preferences::snack1SimilaritySearch(
        int similarUser, std::vector<int> _localBreakfastPreferences,
        std::vector<std::vector<int>> _breakfastPreferencesMatrix,
        std::vector<int> _localSnack1Preferences,
        std::vector<std::vector<int>> _snack1PreferencesMatrix) {

    std::vector<int>::iterator breakfastComparison, snack1Comparison, it;
    std::vector<int> _breakfastSamePreferences(
        _localBreakfastPreferences.size()),
        _snack1SamePreferences(_localSnack1Preferences.size());
    int mostSimilarScore = 0;
    int mostSimilareUser = similarUser;

    for (unsigned long i = 0; i < _breakfastPreferencesMatrix.size(); i++) {
        int aux = 0;

        breakfastComparison = std::set_intersection(_localBreakfastPreferences.begin(),
                                  _localBreakfastPreferences.end(),
                _breakfastPreferencesMatrix[i].begin(),
                _breakfastPreferencesMatrix[i].end(),
                _breakfastSamePreferences.begin());

        snack1Comparison = std::set_intersection(
            _localSnack1Preferences.begin(), _localSnack1Preferences.end(),
                _snack1PreferencesMatrix[i].begin(),
                _snack1PreferencesMatrix[i].end(),
                _snack1SamePreferences.begin());

        _breakfastSamePreferences.resize(breakfastComparison -
                                         _breakfastSamePreferences.begin());
        _snack1SamePreferences.resize(snack1Comparison -
                                      _snack1SamePreferences.begin());

        for (it = _breakfastSamePreferences.begin(); it != breakfastComparison;
             ++it)
            aux = aux + 1;
        for (it = _snack1SamePreferences.begin(); it != snack1Comparison;
             ++it)
            aux = aux + 1;

        if (aux > mostSimilarScore) {
            mostSimilareUser = i;
            mostSimilarScore = aux;
        }
    }
    return mostSimilareUser;
}

int predictions::user_preferences::lunchSimilaritySearch(
        int similarUser, std::vector<int> _localBreakfastPreferences,
        std::vector<std::vector<int>> _breakfastPreferenceMatrix,
        std::vector<int> _localSnack1Preferences,
        std::vector<std::vector<int>> __snack1PreferencesMatrix,
        std::vector<int> _localLunchPreferences,
        std::vector<std::vector<int>> __lunchPreferencesMatrix
        ) {

    std::vector<int>::iterator breakfastCompare, snack1Compare,
            lunchCompare, it;
    std::vector<int> _breakfastSamePreferences(
        _localBreakfastPreferences.size()),
            _snack1SamePreferences(_localSnack1Preferences.size()),
            _lunchSamePreferences(_localLunchPreferences.size());
    int similariestScore = 0;
    int currentSimilariest = similarUser;

    for (unsigned long i = 0; i < _breakfastPreferenceMatrix.size(); i++) {
        int aux = 0;

        breakfastCompare = std::set_intersection(_localBreakfastPreferences.begin(),
                                  _localBreakfastPreferences.end(),
                _breakfastPreferenceMatrix[i].begin(),
                _breakfastPreferenceMatrix[i].end(),
                _breakfastSamePreferences.begin());

        snack1Compare = std::set_intersection(
            _localSnack1Preferences.begin(), _localSnack1Preferences.end(),
                __snack1PreferencesMatrix[i].begin(),
                __snack1PreferencesMatrix[i].end(),
                _snack1SamePreferences.begin());

        lunchCompare = std::set_intersection(
            _localLunchPreferences.begin(), _localLunchPreferences.end(),
                __lunchPreferencesMatrix[i].begin(),
                __lunchPreferencesMatrix[i].end(),
                _lunchSamePreferences.begin());

        _breakfastSamePreferences.resize(breakfastCompare -
                                         _breakfastSamePreferences.begin());
        _snack1SamePreferences.resize(snack1Compare -
                                      _snack1SamePreferences.begin());
        _lunchSamePreferences.resize(lunchCompare -
                                     _lunchSamePreferences.begin());

        for (it = _breakfastSamePreferences.begin(); it != breakfastCompare;
             ++it)
            aux = aux + 1;
        for (it = _snack1SamePreferences.begin(); it != snack1Compare;
             ++it)
            aux = aux + 1;
        for (it = _lunchSamePreferences.begin(); it != lunchCompare;
             ++it)
            aux = aux + 1;

        if (aux > similariestScore) {
            currentSimilariest = i;
            similariestScore = aux;
        }
    }
    return currentSimilariest;
}

int predictions::user_preferences::snack2SimilaritySearch(
        int similarUser, std::vector<int> _localBreakfastPreferences,
        std::vector<std::vector<int>> _breakfastPreferenceMatrix,
        std::vector<int> _localSnack1Preferences,
        std::vector<std::vector<int>> __snack1PreferencesMatrix,
        std::vector<int> _localLunchPreferences,
        std::vector<std::vector<int>> __lunchPreferencesMatrix,
        std::vector<int> _localSnack2Preferences,
        std::vector<std::vector<int>> __snack2PreferencesMatrix) {

    std::vector<int>::iterator breakfastCompare, snack1Compare,
            lunchCompare, snack2Compare, it;
    std::vector<int> _breakfastSamePreferences(
        _localBreakfastPreferences.size()),
            _snack1SamePreferences(_localSnack1Preferences.size()),
            _lunchSamePreferences(_localLunchPreferences.size()),
            _snack2SamePreferences(_localSnack2Preferences.size());
    int similariestScore = 0;
    int currentSimilariest = similarUser;

    for (unsigned long i = 0; i < _breakfastPreferenceMatrix.size(); i++) {
        int aux = 0;

        breakfastCompare = std::set_intersection(_localBreakfastPreferences.begin(),
                                  _localBreakfastPreferences.end(),
                _breakfastPreferenceMatrix[i].begin(),
                _breakfastPreferenceMatrix[i].end(),
                _breakfastSamePreferences.begin());

        snack1Compare = std::set_intersection(
            _localSnack1Preferences.begin(), _localSnack1Preferences.end(),
                __snack1PreferencesMatrix[i].begin(),
                __snack1PreferencesMatrix[i].end(),
                _snack1SamePreferences.begin());

        lunchCompare = std::set_intersection(
            _localLunchPreferences.begin(), _localLunchPreferences.end(),
                __lunchPreferencesMatrix[i].begin(),
                __lunchPreferencesMatrix[i].end(),
                _lunchSamePreferences.begin());

        snack2Compare = std::set_intersection(
            _localSnack2Preferences.begin(), _localSnack2Preferences.end(),
                __snack2PreferencesMatrix[i].begin(),
                __snack2PreferencesMatrix[i].end(),
                _snack2SamePreferences.begin());



        _breakfastSamePreferences.resize(breakfastCompare -
                                         _breakfastSamePreferences.begin());
        _snack1SamePreferences.resize(snack1Compare -
                                      _snack1SamePreferences.begin());
        _lunchSamePreferences.resize(lunchCompare -
                                     _lunchSamePreferences.begin());
        _snack2SamePreferences.resize(snack2Compare -
                                      _snack2SamePreferences.begin());


        for (it = _breakfastSamePreferences.begin(); it != breakfastCompare;
             ++it)
            aux = aux + 1;
        for (it = _snack1SamePreferences.begin(); it != snack1Compare;
             ++it)
            aux = aux + 1;
        for (it = _lunchSamePreferences.begin(); it != lunchCompare;
             ++it)
            aux = aux + 1;
        for (it = _snack2SamePreferences.begin(); it != snack2Compare;
             ++it)
            aux = aux + 1;


        if (aux > similariestScore) {
            currentSimilariest = i;
            similariestScore = aux;
        }
    }
    return currentSimilariest;
}

int predictions::user_preferences::dinnerSimilaritySearch(
        int similarUser, std::vector<int> _localBreakfastPreferences,
        std::vector<std::vector<int>> _breakfastPreferenceMatrix,
        std::vector<int> _localSnack1Preferences,
        std::vector<std::vector<int>> __snack1PreferencesMatrix,
        std::vector<int> _localLunchPreferences,
        std::vector<std::vector<int>> __lunchPreferencesMatrix,
        std::vector<int> _localSnack2Preferences,
        std::vector<std::vector<int>> __snack2PreferencesMatrix,
        std::vector<int> _localDinnerPreferences,
        std::vector<std::vector<int>> __dinnerPreferencesMatrix) {

    std::vector<int>::iterator breakfastCompare, snack1Compare,
        lunchCompare, snack2Compare, dinnerCompare, it;
    std::vector<int> _breakfastSamePreferences(
        _localBreakfastPreferences.size()),
        _snack1SamePreferences(_localSnack1Preferences.size()),
        _lunchSamePreferences(_localLunchPreferences.size()),
        _snack2SamePreferences(_localSnack2Preferences.size()),
        _dinnerSamePreferences(_localDinnerPreferences.size());
    int similariestScore = 0;
    int currentSimilariest = similarUser;

    for (unsigned long i = 0; i < _breakfastPreferenceMatrix.size(); i++) {
        int aux = 0;

        breakfastCompare = std::set_intersection(_localBreakfastPreferences.begin(),
                                  _localBreakfastPreferences.end(),
                _breakfastPreferenceMatrix[i].begin(),
                _breakfastPreferenceMatrix[i].end(),
                _breakfastSamePreferences.begin());

        snack1Compare = std::set_intersection(
            _localSnack1Preferences.begin(), _localSnack1Preferences.end(),
                __snack1PreferencesMatrix[i].begin(),
                __snack1PreferencesMatrix[i].end(),
                _snack1SamePreferences.begin());

        lunchCompare = std::set_intersection(
            _localLunchPreferences.begin(), _localLunchPreferences.end(),
                __lunchPreferencesMatrix[i].begin(),
                __lunchPreferencesMatrix[i].end(),
                _lunchSamePreferences.begin());

        snack2Compare = std::set_intersection(
            _localSnack2Preferences.begin(), _localSnack2Preferences.end(),
                __snack2PreferencesMatrix[i].begin(),
                __snack2PreferencesMatrix[i].end(),
                _snack2SamePreferences.begin());

        dinnerCompare = std::set_intersection(
            _localDinnerPreferences.begin(), _localDinnerPreferences.end(),
                __dinnerPreferencesMatrix[i].begin(),
                __dinnerPreferencesMatrix[i].end(),
                _dinnerSamePreferences.begin());

        _breakfastSamePreferences.resize(breakfastCompare -
                                         _breakfastSamePreferences.begin());
        _snack1SamePreferences.resize(snack1Compare -
                                      _snack1SamePreferences.begin());
        _lunchSamePreferences.resize(lunchCompare -
                                     _lunchSamePreferences.begin());
        _snack2SamePreferences.resize(snack2Compare -
                                      _snack2SamePreferences.begin());
        _dinnerSamePreferences.resize(dinnerCompare -
                                      _dinnerSamePreferences.begin());

        for (it = _breakfastSamePreferences.begin(); it != breakfastCompare;
             ++it)
            aux = aux + 1;
        for (it = _snack1SamePreferences.begin(); it != snack1Compare;
             ++it)
            aux = aux + 1;
        for (it = _lunchSamePreferences.begin(); it != lunchCompare;
             ++it)
            aux = aux + 1;
        for (it = _snack2SamePreferences.begin(); it != snack2Compare;
             ++it)
            aux = aux + 1;
        for (it = _dinnerSamePreferences.begin(); it != dinnerCompare;
             ++it)
            aux = aux + 1;

        if (aux > similariestScore) {
            currentSimilariest = i;
            similariestScore = aux;
        }
    }
    return currentSimilariest;
}

void printMenu(const predictions::user_preferences &u, std::ofstream &fout) {
    std::cout << "Os IDs dos alimentos ingeridos por " << u.getName()
              << " foram:" << std::endl;
    fout << "Os IDs dos alimentos ingeridos por " << u.getName()
              << " foram:" << std::endl;

    std::cout << "Desjejum: " << std::endl;
    fout << "Desjejum: " << std::endl;

    _breakfastPreferences = u.getBreakfastPreferences();
    _snack1Preferences = u.getSnack1Preferences();
    _lunchPreferences = u.getLunchPreferences();
    _snack2Preferences = u.getSnack2Preferences();
    _dinnerPreferences = u.getDinnerPreferences();
    _supperPreferences = u.getPreferenciasCeia();

    _breakfastPortionPreferences = u.getBreakfastPreferencesPortions();
    _snack1PortionPreferences = u.getSnack1PortionPreferences();
    _lunchPortionPreferences = u.getLunchPortionsPreferences();
    _snack2PortionPreferences = u.getSnack2PreferencesPortions();
    _dinnerPortionPreferences = u.getDinnerPreferencesPortions();
    _supperPortionPreferences = u.getPorcaoPreferenciasCeia();

    for (unsigned long j = 0; j < _breakfastPreferences.size(); j++) {
        std::cout << "ID:" << _breakfastPreferences[j]
                  << ", Porçoes:" << _breakfastPortionPreferences[j]
                  << std::endl;

        fout << "ID:" << _breakfastPreferences[j]
                  << ", Porçoes:" << _breakfastPortionPreferences[j]
                  << std::endl;
    }
    std::cout << std::endl;
    fout << std::endl;

    std::cout << "Lanche1: " << std::endl;
    fout << "Lanche1: " << std::endl;
    for (unsigned long j = 0; j < _snack1Preferences.size(); j++) {
        std::cout << "ID:" << _snack1Preferences[j]
                  << ", Porçoes:" << _snack1PortionPreferences[j] << std::endl;
        fout << "ID:" << _snack1Preferences[j]
                  << ", Porçoes:" << _snack1PortionPreferences[j] << std::endl;
    }
    std::cout << std::endl;
    fout << std::endl;

    std::cout << "Almoco: " << std::endl;
    fout << "Almoco: " << std::endl;

    for (unsigned long j = 0; j < _lunchPreferences.size(); j++) {
        std::cout << "ID:" << _lunchPreferences[j]
                  << ", Porçoes:" << _lunchPortionPreferences[j] << std::endl;
        fout << "ID:" << _lunchPreferences[j]
                  << ", Porçoes:" << _lunchPortionPreferences[j] << std::endl;
    }
    std::cout << std::endl;
    fout << std::endl;

    std::cout << "Lanche2: " << std::endl;
    fout << "Lanche2: " << std::endl;
    for (unsigned long j = 0; j < _snack2Preferences.size(); j++) {
        std::cout << "ID:" << _snack2Preferences[j]
                  << ", Porçoes:" << _snack2PortionPreferences[j] << std::endl;
        fout << "ID:" << _snack2Preferences[j]
                  << ", Porçoes:" << _snack2PortionPreferences[j] << std::endl;
    }
    std::cout << std::endl;
    fout << std::endl;

    std::cout << "Jantar: " << std::endl;
    fout << "Jantar: " << std::endl;
    for (unsigned long j = 0; j < _dinnerPreferences.size(); j++) {
        std::cout << "ID:" << _dinnerPreferences[j]
                  << ", Porçoes:" << _dinnerPortionPreferences[j] << std::endl;
        fout << "ID:" << _dinnerPreferences[j]
                  << ", Porçoes:" << _dinnerPortionPreferences[j] << std::endl;
    }
    std::cout << std::endl;
    fout << std::endl;

    std::cout << "Ceia: " << std::endl;
    fout << "Ceia: " << std::endl;
    for (unsigned long j = 0; j < _supperPreferences.size(); j++) {
        fout << "ID:" << _supperPreferences[j]
                  << ", Porçoes:" << _supperPortionPreferences[j] << std::endl;
        std::cout << "ID:" << _supperPreferences[j]
                  << ", Porçoes:" << _supperPortionPreferences[j] << std::endl;
    }
    std::cout << std::endl;
    fout << std::endl;
}

std::vector<int> predictions::user_preferences::getBreakfastPreferences() const {
    return this->_breakfastPreferences;
} // Itens escolhidos pelo usuario corrente

std::vector<int> predictions::user_preferences::getBreakfastPreferencesPortions() const {
    return this->_breakfastPreferencesPortions;

} // Porcao dos itens escolhidos pelo usuario corrente
std::vector<int> predictions::user_preferences::getSnack1Preferences() const {
    return this->_snack1Preferences;
}
std::vector<int> predictions::user_preferences::getSnack1PortionPreferences() const {
    return this->_snack1PreferencesPortions;
}
std::vector<int> predictions::user_preferences::getLunchPreferences() const {
    return this->_lunchPreferences;
}
std::vector<int> predictions::user_preferences::getLunchPortionsPreferences() const {
    return this->_lunchPreferencesPortions;
}
std::vector<int> predictions::user_preferences::getSnack2Preferences() const {
    return this->_snack2Preferences;
}
std::vector<int> predictions::user_preferences::getSnack2PreferencesPortions() const {
    return this->_snack2PreferencesPortions;
}
std::vector<int> predictions::user_preferences::getDinnerPreferences() const {
    return this->_dinnerPreferences;
}
std::vector<int> predictions::user_preferences::getDinnerPreferencesPortions() const {
    return this->_dinnerPreferencesPortions;
}
std::vector<int> predictions::user_preferences::getPreferenciasCeia() const {
    return this->_supperPreferences;
}
std::vector<int> predictions::user_preferences::getPorcaoPreferenciasCeia() const {
    return this->_supperPreferencesPortions;
}
void predictions::user_preferences::setBreakfastPreferences(std::vector<int> _idInput) {
    this->_breakfastPreferences = std::move(_idInput);
}
void predictions::user_preferences::setBreakfastPreferencesPortions(
    std::vector<int> _portionsInput) {
    this->_breakfastPreferencesPortions = std::move(_portionsInput);
}
void predictions::user_preferences::setSnack1Preferences(std::vector<int> _entradasID) {
    this->_snack1Preferences = std::move(_entradasID);
}
void predictions::user_preferences::setSnack1PortionPreferences(
    std::vector<int> _portionsInput) {
    this->_snack1PreferencesPortions = std::move(_portionsInput);
}
void predictions::user_preferences::setLunchPreferences(std::vector<int> _idInput) {
    this->_lunchPreferences = std::move(_idInput);
}
void predictions::user_preferences::setLunchPortionsPreferences(
    std::vector<int> _portionsInput) {
    this->_lunchPreferencesPortions = std::move(_portionsInput);
}
void predictions::user_preferences::setSnack2Preferences(std::vector<int> _idInput) {
    this->_snack2Preferences = std::move(_idInput);
}
void predictions::user_preferences::setSnack2PreferencesPortions(
    std::vector<int> _portionsInput) {
    this->_snack2PreferencesPortions = std::move(_portionsInput);
}
void predictions::user_preferences::setDinnerPreferences(std::vector<int> _idInput) {
    this->_dinnerPreferences = std::move(_idInput);
}
void predictions::user_preferences::setDinnerPreferencesPortions(
    std::vector<int> _entradasPorcoes) {
    this->_dinnerPreferencesPortions = std::move(_entradasPorcoes);
}
void predictions::user_preferences::setSupperPreferences(std::vector<int> _idInput) {
    this->_supperPreferences = std::move(_idInput);
}
void predictions::user_preferences::setSupperPreferencesPortions(
    std::vector<int> _portionsInput) {
    this->_supperPreferencesPortions = std::move(_portionsInput);
}
