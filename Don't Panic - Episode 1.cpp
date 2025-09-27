#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

string defineCommandOnOneFloor(string& direction, int clone_pos, int target_pos){
    string command;
    // Определяем, что делать лидирующему клону на том этаже, на котором он находится
    if (direction == "RIGHT") {
        // Если движется вправо и цель правее текущей позиции, то продолжаем движение
        // иначе, нужно менять сторону движения, то есть блокировать лидера
        command = clone_pos <= target_pos ? "WAIT" : "BLOCK";
    } else if (direction == "LEFT") {
        // Если движется влево и цель левее текущей позиции, то продолжаем движение
        // иначе, нужно менять сторону движения, то есть блокировать лидера
        command = clone_pos >= target_pos ? "WAIT" : "BLOCK";
    }
    // Если движение лидера NONE, то ждём появления движущегося лидера
    else command = "WAIT";

    return command;
}

int main()
{
    int nb_floors; // number of floors
    int width; // width of the area
    int nb_rounds; // maximum number of rounds
    int exit_floor; // floor on which the exit is found
    int exit_pos; // position of the exit on its floor
    int nb_total_clones; // number of generated clones
    int nb_additional_elevators; // ignore (always zero)
    int nb_elevators; // number of elevators
    cin >> nb_floors >> width >> nb_rounds >> exit_floor >> exit_pos >> nb_total_clones >> nb_additional_elevators >> nb_elevators; cin.ignore();

    // Аналогия на разряженную матрицу, ключ - этаж, значение - координата лифта
    unordered_map<int, int> elev_positions;

    for (int i = 0; i < nb_elevators; i++) {
        int elevator_floor; // floor on which this elevator is found
        int elevator_pos; // position of the elevator on its floor
        cin >> elevator_floor >> elevator_pos; cin.ignore();
        elev_positions[elevator_floor] = elevator_pos;
    }

    // game loop
    while (1) {
        int clone_floor; // floor of the leading clone
        int clone_pos; // position of the leading clone on its floor
        string direction; // direction of the leading clone: LEFT or RIGHT
        cin >> clone_floor >> clone_pos >> direction; cin.ignore();

        string command;
        if (clone_floor == exit_floor){
            // Находимся на этаже с выходом, значит стремимся к выходу
            command = defineCommandOnOneFloor(direction, clone_pos, exit_pos);
        }
        else{
            // НЕ находимся на этаже с выходом, значит стремимся к лифту
            auto iter = elev_positions.find(clone_floor);
            if (iter != elev_positions.end()) {
                // Лифт на текущем этаже существует
                int thisElevPosition = iter->second;
                command = defineCommandOnOneFloor(direction, clone_pos, thisElevPosition);
            }
            else{
                /* Лифта на текущем этаже нет - по идее exception
                но при этом ситуация с -1 -1 NONE тоже попадает сюда.
                По идее нужны разные обработки, но для простоты оставим так */
                command = "WAIT";
            }
        }

        cout << command << endl; // action: WAIT or BLOCK
    }

    return 0;
}