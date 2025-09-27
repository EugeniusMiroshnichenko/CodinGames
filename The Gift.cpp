#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int n;
    cin >> n; cin.ignore();
    int c;
    cin >> c; cin.ignore();

    // Список всех бюджетов
    vector<int> budgets;
    for (int i = 0; i < n; i++) {
        int b;
        cin >> b; cin.ignore();
        budgets.push_back(b);
    }
    // Сортируем по возрастнанию (хотя в примере не давали понять, что может идти в разброс)
    sort(budgets.begin(), budgets.end());
    
    // Максимальный бюджет - она же максимальная стоимость подарка
    int maxBudget = accumulate(budgets.begin(), budgets.end(), 0);
    // Если не можем позволить подарок - сигнализируем об этом и завершаем программу
    if (maxBudget < c) {cout << "IMPOSSIBLE" << endl; return 0;}

    // Дабы не менять исходные данные, создаим свом переменные.
    // Мало-ли где они могут понадобиться
    int costLeft = c;
    int participantsLeft = n;

    for (int budget : budgets){
        /* На каждом шаге ищем оптимльный бюджет.
        Деление int на другое число отбрасывает дробную часть.
        Но нам это и надо (пример №2, строчки 2-3, отбросили 0.5 на конце) */
        int optimalCost = costLeft / participantsLeft;

        // Платим либо оптимальную стоимость, если она меньше, чем бюджет, либо весь бюджет
        int payment = min(budget, optimalCost);

        cout << payment << endl;

        costLeft -= payment;
        participantsLeft -= 1;     
    }

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;
    return 0;    
}