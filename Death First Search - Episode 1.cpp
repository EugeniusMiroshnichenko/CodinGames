#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/


 
class Vertex{
    // Вершина графа
    protected:
        vector<Vertex*> neighbors; // Все соседи
        vector<Vertex*> reachableNeighbors;  // Достижимые соседи (связь с которыми активная)
        int id;  // Уникальный номер
        bool is_gateway;  // Флаг того, что узел является шлюзом (конечная точка для вируса)
    public:
        Vertex(int id_val): id(id_val) {}

        void addNeighbor(Vertex* neighbor){
            neighbors.push_back(neighbor);
            reachableNeighbors.push_back(neighbor);
        }
        void killConnection(Vertex* neighbor){
            // Удаляем "связь" между текущим узлом и соседом
            reachableNeighbors.erase(
                remove_if(reachableNeighbors.begin(), reachableNeighbors.end(),
                    [neighbor](Vertex* x) { 
                        return x->get_id() == neighbor->get_id(); 
                    }),
                reachableNeighbors.end()
            );
        }

        // Геттеры и сеттеры
        int get_id(){return id;}
        vector<Vertex*> get_reachableNeighbors(){
            return reachableNeighbors;
        }

        bool get_is_gateway(){
            return is_gateway;
        }

        void set_as_gateway(){
            is_gateway = true;
        }
};

struct PathResult {
    // Структура для хранения данных о пути
    // Две вершины связи, примыкающей к конечному узлу в bfs
    Vertex* vertex1;
    Vertex* vertex2;
    // Длина пути
    int path_length;
    
    PathResult(Vertex* v1 = nullptr, Vertex* v2 = nullptr, int len = -1)
        : vertex1(v1), vertex2(v2), path_length(len) {}
};

PathResult find_path(Vertex* startV, Vertex* endV, Vertex** all_vertexes, int size){
    /* Поиск пути через bfs 
    (как по мне, лучше всё-таки bfs, так как если делать не через queue, а рекурсию,
    то существует ограничение максимальную глубину вызова функций)
    */

    unordered_map<Vertex*, bool> visited; // Посещённые узлы (во избежание циклов)
    unordered_map<Vertex*, int> distance; // Расстояния от начального узла до любого другого
    queue<Vertex*> q;

    for (int i = 0; i < size; i++) {
        Vertex* v = all_vertexes[i];
        visited[v] = false;
        distance[v] = -1;  // Если вершина недостижима, то положим, что путь до неё -1
        // (хотя такой исход нас и не интересует)
    }

    visited[startV] = true;
    distance[startV] = 0;
    q.push(startV);

    while (!q.empty()) {
        Vertex* currentV = q.front();
        q.pop();
        
        for (Vertex* neighbor : currentV->get_reachableNeighbors()) {
            // Проходимся по всем достижимым соеседям
            if (!visited[neighbor]) {
                distance[neighbor] = distance[currentV] + 1; // Условно вес ребёр = 1
                if (neighbor == endV){
                    // Достигли конечно узла
                    return PathResult(currentV, neighbor, distance[neighbor]);
                }
                
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    // Пути от стартового узла в конечному нет
    throw runtime_error("No path could be found");
}

int main()
{
    int n; // the total number of nodes in the level, including the gateways
    int l; // the number of links
    int e; // the number of exit gateways
    cin >> n >> l >> e; cin.ignore();
    Vertex** vertexes = new Vertex*[n](); // Упрощенная аналогия графа
    Vertex** startVetrexes = new Vertex*[e]();  // начальные узлы - они же gates

    for (int i = 0; i < l; i++) {
        int n1; // N1 and N2 defines a link between these nodes
        int n2;
        cin >> n1 >> n2; cin.ignore();
        Vertex* v1 = vertexes[n1];
        Vertex* v2 = vertexes[n2];
        // Если вершин ещё неь в графе, добавляем
        if (v1 == nullptr){
            v1 = new Vertex(n1);
            vertexes[n1] = v1;
        }
        if (v2 == nullptr){
            v2 = new Vertex(n2);
            vertexes[n2] = v2;
        }
        // Так как добавляем ребра, нужно не забыть создать связь
        v1->addNeighbor(v2);
        v2->addNeighbor(v1);
    }
    
    for (int i = 0; i < e; i++) {
        int ei; // the index of a gateway node
        cin >> ei; cin.ignore();
        // Помечаем требуемые узлы шлюзами и заносим в список стартовых узлов
        vertexes[ei] -> set_as_gateway();
        startVetrexes[i] = vertexes[ei];
    }

    // game loop
    while (1) {
        int si; // The index of the node on which the Bobnet agent is positioned this turn
        cin >> si; cin.ignore();
        
        // Нужно искать кратчайший путь от любого gate к вирусу.
        // У самого короткого пути и надо блокировать какое-либо ребро
        PathResult shortestPathRes;
        bool not_initialized = true;
        for (int i=0; i<e; i++){
            try{
                PathResult pathRes = find_path(startVetrexes[i], vertexes[si], vertexes, n);
                if (not_initialized || pathRes.path_length < shortestPathRes.path_length) {
                    // Нашли более короткий путь или ещё не инициализировали
                    shortestPathRes = pathRes;
                    not_initialized = false;
                }
            }
            catch (const runtime_error& e) {
                // Если нет пути, то просто игнорируем этот gate
                continue;
            }
        }
        
        Vertex* v1 = shortestPathRes.vertex1;
        Vertex* v2 = shortestPathRes.vertex2;
        /* В самом коротком пути убираем связь, примыкающую к вирусу 
        (так получится более эффективно, потому что мы как бы загоняем его в угол,
        а не защищаемся)*/ 
        v1 -> killConnection(v2);
        v2 -> killConnection(v1);
        string edge = to_string(v1 ->get_id()) + " " + to_string(v2 ->get_id());
        cout << edge << endl;
    }
}