#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    // Координаты зданий, которые нужно подключить
    vector<pair<int, int>> buildings;
    int n;
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        int x;
        int y;
        cin >> x >> y; cin.ignore();
        buildings.push_back({x, y});
    }

    // Сортируем здания по первой координате, т.е. X.
    // X_max - X_min даст длину центрального (магистрального) кабеля
    sort(buildings.begin(), buildings.end(), []
        (const pair<int, int>& a, const pair<int, int>& b) {return a.first < b.first;}
    );
    
    int x_start = buildings[0].first;
    int x_end = buildings[n-1].first;

    int mainCableLength = x_end - x_start;

    /* Сортируем здания по второй координате, т.е. Y
    Оптимальным расположение центрального кабеля по координате Y будет 
    соответствовать медиане по Y в отсортированном списке (не среднему!)*/
    sort(buildings.begin(), buildings.end(), []
        (const pair<int, int>& a, const pair<int, int>& b) {return a.second < b.second;}
    );

    int middleEl = 0; 
    /* Если элементов чётное количество, то проблем при делении int не возникнет.
    Если количество нечётное, то отбрасывание дробной части нам ничего не сломает.
    Например: 4/2=2, для медианы берём 1 и 2 элемент (с учётом отчёта с 0), всё ок.
    Если 5/2 = 2.5 -> 2, но нам и нужен элемент под номером 2 (с учётом отчёта с 0), всё ок*/
    int middleElIdx = n / 2;
    if (n % 2 != 0){
        // Берём медианный элемент
        middleEl = buildings[middleElIdx].second;
    }
    else{
        // Берём среднее между медианными элементами
        middleEl = (buildings[middleElIdx-1].second + buildings[middleElIdx].second) / 2;
    }

    // Выделяем побольше памяти, иначе произойдёт переполнение
    unsigned long buildingssCablesLength = 0;
    for (const pair <int, int>& coordinate : buildings){
        // Суммарная длина от здания к центральному кабелю
        buildingssCablesLength += abs(coordinate.second - middleEl);
    }
    

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << mainCableLength + buildingssCablesLength << endl;
    return 0;
}