#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
vector<pair<string, string>> findSequence(vector<string> strDevided, int size){
    // Словарь вида "число : количество повторений подряд"
    vector<pair<string, string>> symbolToQuanityMap;
    // Увеличение итератора кастомное в теле цикла
    for (int i=0; i < size;){
        string number = strDevided[i];
        int numberQuanity = 1;
        for (int j = i + 1; j < size; j++){
            // Проверяем, что следующий элемент такой же, как предыщущий
            string nextNumber = strDevided[j];
            if (number == nextNumber){
                numberQuanity++;
            }
            else {
                break;
            }
        }
        // Даже если не попали в цикл, как минимум 1 раз число встретитлось
        symbolToQuanityMap.push_back({number, to_string(numberQuanity)});
        // Увеличиваем итератор на количество повторяющихся символов
        i += numberQuanity;
    }
    return symbolToQuanityMap;
};

vector<string> splitString(const string& str) {
    vector<string> result;
    stringstream ss(str);
    string token;
    // Метод для деления сторки на подстроки
    // по проблемам (да и другим символам обрыва)
    
    while (ss >> token) {
        result.push_back(token);
    }
    
    return result;
}

int main()
{
    int r;
    cin >> r; cin.ignore();
    int l;
    cin >> l; cin.ignore();

    vector<string> strDevided;
    string output_str = to_string(r); // Если будет только 1 уровень, то выведем исходный символ

    for (int line_num=1; line_num<l; line_num++){
        // Делим выходную строку по пробелам на подстроки
        strDevided = splitString(output_str);
        output_str = ""; // Обнуляем строку вывода

        vector<pair<string, string>> sequence = findSequence(strDevided, strDevided.size());
        for (const auto& pair : sequence) {
            // Значение 2 (как бы значение) - количество повторений
            output_str += pair.second + " ";
            // Значение 1 (как бы ключ) - количество повторений
            output_str += pair.first + " ";
        }
        // Обрезаем пустой пробел на конце
        if (!output_str.empty()) output_str.pop_back();
    }
    cout << output_str << endl;
}