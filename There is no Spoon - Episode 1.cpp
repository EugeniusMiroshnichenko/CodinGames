#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Don't let the machines win. You are humanity's last hope...
 **/

/* Для инкапсуляции получения строки на вывод,
а также более удобного хранения информации о существующих узлах
создадим класс PowerNode
*/
class PowerNode{
    private:
        PowerNode* neighborToRight;  // Сосед справа, в общем случае может и не быть (nullptr)
        PowerNode* neighborToBottom;  // Сосед снизу, в общем случае может и не быть (nullptr)
        int x,y;  // Координаты
    public:
        PowerNode(int xVal, int yVal) : x(xVal), y(yVal), neighborToRight(nullptr), neighborToBottom(nullptr) {}
    
        string coordinatToStr(){
            return to_string(x) + " " + to_string(y);
        }

        string output(){
            /* Для удобного вывода результатов 
            достаточно будет вызвать output у всех PowerNode
            */ 

            // Собственная координата
            string str = coordinatToStr() + " ";
            
            // Координата соседа справа
            if (neighborToRight) {
                str += neighborToRight->coordinatToStr();
            } else {
                // Нет соседа
                str += "-1 -1";
            }
            str += " ";

            // Координата соседа снизу
            if (neighborToBottom) {
                str += neighborToBottom->coordinatToStr();
            } else {
                // Нет соседа
                str += "-1 -1";
            }
            
            return str;
        }
    
    // Геттеры и сеттеры
    int get_x() const { return x; }

    int get_y() const { return y; }

    void setRightNeighbor(PowerNode* neighbor) {
        neighborToRight = neighbor;
    }
    
    void setBottomNeighbor(PowerNode* neighbor) {
        neighborToBottom = neighbor;
    }
};

int main()
{
    int width; // the number of cells on the X axis
    cin >> width; cin.ignore();
    int height; // the number of cells on the Y axis
    cin >> height; cin.ignore();

    vector<PowerNode*> powerNodes;
    /* Для удобного поиска соседей создадим матрицу:
    если в узле есть PowerNode, то на него будет указатель,
    иначе - nullptr
    */
    vector<vector<PowerNode*>> matrix(height, vector<PowerNode*>(width, nullptr));

    for (int i = 0; i < height; i++) {
        string line;
        getline(cin, line); // width characters, each either 0 or .
        for (int j=0; j < width; j++){
            PowerNode* node;
            if (line[j] == '0'){
                // Найден PowerNode
                node = new PowerNode(j, i);
                powerNodes.push_back(node);
                matrix[i][j] = node;
            }
        }
    }

    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            PowerNode* curNode = matrix[i][j];
            if (curNode){
                // В текущей ячейке PowerNode
                for (int k=j+1; k < width; k++){
                    PowerNode* rightNeighbor = matrix[i][k];
                    if (rightNeighbor) {
                        // Найден первый сосед справа 
                        curNode -> setRightNeighbor(rightNeighbor);
                        break;
                    }
                }
                for (int k=i+1; k < height; k++){
                    PowerNode* bottomNeighbor = matrix[k][j];
                    if (bottomNeighbor) {
                        // Найден первый сосед снизу 
                        curNode -> setBottomNeighbor(bottomNeighbor);
                        break;
                    }
                }
            }
        }
    }

    if (powerNodes.size() >= 1){
        for (PowerNode* node : powerNodes) {
            // Результат - вывод output для всех PowerNodes
            cout << node->output() << endl;
        }
    }
    // Если вдруг нет Power Nodes
    else cout << "No Power Nodes detected" << endl;

    return 0;
}
