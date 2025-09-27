#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <limits> // для numeric_limits
#include <cctype> // для isspace

using namespace std;


struct Defibrillator {
    // Упрощенная структуоа для храненения данных о дифибриляторе
    string name;
    float longitude; // Градусы
    float latitude; // Градусы
};

vector<string> splitString(const string& str) {
    vector<string> result;
    string modifiedStr = str;
    
    // Заменяем все запятые на точки
    replace(modifiedStr.begin(), modifiedStr.end(), ',', '.');
    
    stringstream ss(modifiedStr);
    string token;
    
    // Разделяем строку по символу ';'
    while (getline(ss, token, ';')) {
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

float degreeToRads(float angleDeg){
    // Перевод градусов в радианы
    return angleDeg * M_PI / 180;
}

float calcDistance(float lon1, float lon2, float lat1, float lat2){
    // Расчёт расстояния по формуле Хаверсина в общем виде
    float lon1_in_rads = degreeToRads(lon1);
    float lon2_in_rads = degreeToRads(lon2);
    float lat1_in_rads = degreeToRads(lat1);
    float lat2_in_rads = degreeToRads(lat2);
    float x = (lon2_in_rads - lon1_in_rads) * cos((lat1_in_rads + lat2_in_rads) / 2.0 );
    float y = lat2_in_rads - lat1_in_rads;
    return 6371.0 * sqrt(pow(x, 2) + pow(y, 2));
}
int main()
{
    // Получаем инфомрацию о долготе и широте человека
    // форматируем строку и переводим в float
    string lon;
    cin >> lon; cin.ignore();
    replace(lon.begin(), lon.end(), ',', '.');
    float per_lon = stof(lon);

    string lat;
    cin >> lat; cin.ignore();
    replace(lat.begin(), lat.end(), ',', '.');
    float per_lat = stof(lat);

    int n;
    cin >> n; cin.ignore();
    vector <Defibrillator> defibsData;

    for (int i = 0; i < n; i++) {
        string defib;
        getline(cin, defib);
        // Данные о дифибриляторе в форме строки, делим её на подстроки
        vector<string> defibData = splitString(defib);
        int last_el_idx = defibData.size() - 1;
        float latitude = stof(defibData[last_el_idx]); // Широта последняя
        float longitude  = stof(defibData[last_el_idx - 1]); // Долготоа предпоследняя
        string name = defibData[1];  // Имя вторая подстрока
    
        defibsData.push_back({name, longitude, latitude});
    }

    float min_dist = numeric_limits<float>::max();
    string closestDefibName = "";
    for (Defibrillator defib : defibsData){
        string name = defib.name;
        float defib_lon = defib.longitude;
        float defib_lat = defib.latitude;
        float dist = calcDistance(per_lon, defib_lon, per_lat, defib_lat);
        if (dist < min_dist){
            // Текущей дифибрилятор ближе
            min_dist = dist;
            closestDefibName = name;
        }
    }
    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << closestDefibName << endl;
}