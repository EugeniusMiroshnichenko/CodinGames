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
    int n;
    cin >> n; cin.ignore();
    // Значения цены
    vector<int> values;
    for (int i = 0; i < n; i++) {
        int v;
        cin >> v; cin.ignore();
        values.push_back(v);
    }
    /* Можно просто делать перебор справа налево,
    при этом от каждой точки перебирать до самого начала в поисках максимального падения
    (как бы двойной перебор) */

    /*Но можно и проще - достаточно знать максимальное значение в списке за исключением
    последнего элемента, и для каждой точки проверять потери цены по отношеннию к этому максимуму.
    При этом отлавливание максимума и сравнения потери цены с ним можно делать параллельно*/

    // Предварительно максимальная цена - первый элемент
    int maxValue = values[0];
    int maxLoss = 0;
    
    for (int i = 1; i < values.size(); i++) {
        if (values[i] > maxValue) {
            // Найден новый максимум
            maxValue = values[i];
        }
        // Потери по отношению к текущему максимуму
        int currentLoss = maxValue - values[i];
        if (currentLoss > maxLoss) {
            // Потери по отношению к текущему максимуму
            // оказались больше, чем было до этого (слева)
            maxLoss = currentLoss;
        }
    }
    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;
    
    // Если потерей не было (только рост), то выведем 0, т.к maxLoss проинициализирован в 0
    cout << -maxLoss << endl;  // Число было положительным, нам надо вывести отрицательное
}