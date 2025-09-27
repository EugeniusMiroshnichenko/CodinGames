#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <cmath>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// Поддерживаемые символы 
vector<char> ACCEPTABLE_CHARS = {'2', '3', '4', '5', '6', '7', '8', '9',
                                 'K', 'Q', 'J', 'T', 'A'};

// Словарь "символ : его численное значение"
unordered_map<char, int> SYMBOL_TO_NUMBER_MAP = {
    {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8},
    {'9', 9}, {'K', 10}, {'Q', 10}, {'J', 10}, {'T', 10}, {'A', 1}
};


class Deck{
    // Для инкапсуляции уменьшения количества карт
    // и расчёта вероятности
    protected:
        // Словарь "символ : его оставшееся количество"
        unordered_map<char, int> symbolToQuanityMap;
    public:
        Deck(vector<char>& symbols, int initialQuanity = 4){
            // Для простоты (не далеко от мира) количество всех карт одинаковое
            for (char symbol : symbols){
                symbolToQuanityMap[symbol] = initialQuanity;
            }
        }

        void decreaseSymbolQuanity(char symbol){
            // Уменьшение количества символов
            if (symbolToQuanityMap.find(symbol) != symbolToQuanityMap.end() && symbolToQuanityMap[symbol] > 0){
                symbolToQuanityMap[symbol] -= 1;
            }
            else {
                // Какая-то другая обработка, например exception
            }

        }

        int countProbability(int bustThreshold){
            // Расчёт вероятности того, что следующая случайная карта < bustThreshold
            // С учётом постепернного уменьшения количества карт в колоде
            int totalQuanity = 0;
            int lessThanbustThresholdQuanity = 0;

            for (auto& symbolToQuanity : symbolToQuanityMap){
                int symbolNum = SYMBOL_TO_NUMBER_MAP[symbolToQuanity.first];
                int symbolQuanity = symbolToQuanity.second;
                totalQuanity += symbolQuanity;
                if (symbolNum < bustThreshold && 
                    symbolQuanity > 0){
                        lessThanbustThresholdQuanity += symbolQuanity;
                    }
            }
            if (totalQuanity == 0) return 0; // Избегаем деления на 0
            // Округляем до БЛИЖАЙШЕГО
            return round(lessThanbustThresholdQuanity * 100.0 / totalQuanity);
        }
};

bool isStringValid(vector<char>& stringDividedToChars){
    // Проверка на то, что строка символов содержит лишь допустимые символы
    for (char symbol : stringDividedToChars){
        bool isOkay = false;
        for (char acceptableSymbol : ACCEPTABLE_CHARS){
            if (symbol == acceptableSymbol) {
                isOkay = true;
                break;
            }
        }
        if (!isOkay) return false;
    }
    return true;
}

vector<string> splitString(const string& str) {
    vector<string> result;
    stringstream ss(str);
    string token;
    
    // Разделяем строку по символу ';'
    while (getline(ss, token, '.')) {
        // Удаляем начальные и конечные пробелы
        size_t start = token.find_first_not_of(" \t\n\r\f\v");
        if (start == string::npos) {
            result.push_back(""); // пустая строка если только пробелы (на всякий случай)
            continue;
        }
        size_t end = token.find_last_not_of(" \t\n\r\f\v");
        result.push_back(token.substr(start, end - start + 1));
    }
    
    return result;
}

int main()
{
    string stream_of_consciousness;
    getline(cin, stream_of_consciousness);
    cerr << stream_of_consciousness << endl;
    int bust_threshold;
    cin >> bust_threshold; cin.ignore();

    Deck deck = Deck(ACCEPTABLE_CHARS, 4);
    // Все комбинации
    vector<string> allCombinations = splitString(stream_of_consciousness);
    vector<vector<char>> acceptableCombinations;

    for (string& combination : allCombinations){
        // Добавляем только те строки, которые содержат лишь допустимые символы
        vector<char> combinationDevidedToChars(combination.begin(), combination.end());
        if (isStringValid(combinationDevidedToChars)){
            acceptableCombinations.push_back(combinationDevidedToChars);
        }
    }

    for (vector<char>& combination : acceptableCombinations){
        // Уменьшаем количество карт в колоде
        for (char symbol : combination){
            deck.decreaseSymbolQuanity(symbol);
        }     
    }

    int percentage = deck.countProbability(bust_threshold); 
    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    // Не забываем добавить % в конце)
    cout << percentage << "%" << endl;
    return 0;
}