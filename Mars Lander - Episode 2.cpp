#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// Блок констант
float G = 3.711;
float const V_Y_MAX = 40.0;
float const V_X_MAX = 20.0;

class SurfaceVertex{
    // Вершина на поверхности
    protected:
        int x, y;
    public:
        SurfaceVertex() : x(), y() {}
        SurfaceVertex(int x_val, int y_val) : x(x_val), y(y_val) {}
        int const get_x() {return x;}
        int const get_y() {return y;}
};

class SurfaceEdge{
    // Грань поверхности
    private:
        SurfaceVertex* v_start;
        SurfaceVertex* v_end;
    public:
        SurfaceEdge() : v_start(), v_end(){}
        SurfaceEdge(SurfaceVertex* v1, SurfaceVertex* v2) : v_start(v1), v_end(v2) {}
        bool is_landing_spot(){
            // Если координаты по y совпадают - то это посадочное место (ровная земля)
            return v_start -> get_y() == v_end -> get_y() ? true : false;
        }

        int get_start_y(){return v_start -> get_y();}
        int get_end_y(){return v_end -> get_y();}
        int get_start_x(){return v_start -> get_x();}
        int get_end_x(){return v_end -> get_x();}

        float get_x_middle(){
            // Середина грани поверхности
            int x1 = get_start_x();
            int x2 = get_end_x();
            return float(float((x2 - x1)) / 2 + x1);
        }

};

float control_x(float cur_x, float target_x, float v_x, bool isCloseToLand, float cur_a_x) {
    // Управление по Х. Результат - желаемое ускорение по X
    
    // Если мы уже рядом с землей, то по Ox не ускоряемся, чтобы приземлиться
    // перпендикулярно поверхности
    if (isCloseToLand) return 0.0;

    // Расстояние от текущего x до цели
    float distance = target_x - cur_x;

    // Желаемая скорость в конечной точке 
    float desired_v_x = 0.0;

    // Если мы уже близко по Ох, то ничего делать не надо
    if (abs(cur_x - target_x) < 20) return 0.0;

    /* Для учёта хоть в каком-то виде инертности системы 
    (невозможно мгновенно приложить ускорении по любой оси, угол поворота ограничен по 15° за раз)
    будем вычитать текущее ускорение */
    float accel_x = (desired_v_x * desired_v_x - v_x * v_x) / (2 * distance) - cur_a_x;

    // Для исключения путанницы с направлениями разруливаем вручную
    if (distance > 0) return -accel_x;
    else return accel_x;
    
}

float control_y(float cur_y, float target_y, float v_y, float cur_a_y) {
    // Управление по Y. Результат - желаемое ускорение по Y

    // Расстояние от текущего y до цели
    float height = cur_y - target_y;

    // Желаемая скорость в конечной точке
    float desired_v_y = 0.7 * V_Y_MAX;

    // Снова пытаемся хоть как-то учесть инерцию
    float accel_y = (desired_v_y * desired_v_y - v_y * v_y) / (2 * height) + G - cur_a_y;
    return - accel_y;
}

int main()
{   

    int surface_n; // the number of points used to draw the surface of Mars.
    cin >> surface_n; cin.ignore();

    // Создаём поверхность
    SurfaceVertex* vertexes = new SurfaceVertex[surface_n]();
    SurfaceEdge* edges = new SurfaceEdge[surface_n - 1]();

    for (int i = 0; i < surface_n; i++) {
        int land_x; // X coordinate of a surface point. (0 to 6999)
        int land_y; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
        cin >> land_x >> land_y; cin.ignore();
        vertexes[i] = SurfaceVertex(land_x, land_y);
    }

    SurfaceEdge landing_spot;
    bool isLandFound = false;
    for (int i=0; i < surface_n - 1; i++){
        edges[i] = SurfaceEdge(&vertexes[i], &vertexes[i+1]); // Указатели на массив
        if (edges[i].is_landing_spot()){
            // Найдено мето для посадки - ровная поверхность
            landing_spot = edges[i];
            isLandFound = true;
        }
    }
    // Если нет места для посадки - что-то не так
    if (!isLandFound) return 1;

    // Изначально целимся в точку повыше центра ровной поверхности,
    // чтобы потом просто снизится
    int start_target_x = landing_spot.get_x_middle();
    int start_target_y = landing_spot.get_start_y() + 50;
    while (1) {
        int x;
        int y;
        
        int h_speed; // the horizontal speed (in m/s), can be negative.
        int v_speed; // the vertical speed (in m/s), can be negative.

        int fuel; // the quantity of remaining fuel in liters.
        int rotate; // the rotation angle in degrees (-90 to 90).
        int power; // the thrust power (0 to 4).
        cin >> x >> y >> h_speed >> v_speed >> fuel >> rotate >> power; cin.ignore();

        bool isCloseToLand = false;
        int targer_x;
        int targer_y;
    
        if (y < start_target_y){
            // Если мы ниже, чем требуемая высота
            // то стремимся к начальной точке
            targer_x = start_target_x;
            targer_y = start_target_y;
        }
        else{
            // Если достигли начальной по высоте - можно снижаться
            targer_x = landing_spot.get_x_middle();
            targer_y = landing_spot.get_start_y();
        }

        // Если мы близко к посадочному месту - нужно выровняться под 0° (прекратит ускорение по Ох)
        if (y - landing_spot.get_start_y() < 50 && y > landing_spot.get_start_y()){
            isCloseToLand = true;
        }

        // "-" так как Ох направлена вправо, а положительный угол - как бы влево  
        float a_x_start = - power * sin(float(rotate) / 180.0 * M_PI);
        float a_y_start = power * cos(float(rotate) / 180.0 * M_PI);

        // Желаемые ускорения по Ох и Оу
        float a_x = control_x(float(x), float(targer_x), float(h_speed), isCloseToLand, a_x_start);
        float a_y = control_y(float(y), float(targer_y), float(v_speed), a_y_start);

        // Изем угол и ускорение (=мощность)
        float alpha = atan(- a_x / a_y);
        int accel = round(sqrt(a_x * a_x + a_y * a_y));

        // Clamp acceleration to maximum of 4
        if (accel > 4) {
            // Первысисли предел по мощности
            accel = 4;

            // Критическая высота, на которой нужно на полную мощность тормозить по Oy с запасом
            if (3 * float((0.49 * V_Y_MAX * V_Y_MAX - v_speed * v_speed)) / (2 * (G - 4.0)) > y - targer_y){
                alpha = 0;
            }

            // Если по высоте всё терпимо, то больший приоритет отдаём управлению по Ox
            else if (a_x > 4 && h_speed < 0)  {
                alpha = - 3.14 / 2.0;
            }
            else if (a_x < 4 && h_speed > 0) {
                alpha = 3.14 / 2.0;
            }
        }

        // rotate power. rotate is the desired rotation angle. power is the desired thrust power.
        cout << to_string(int(alpha * 180 / 3.1415)) << " "<< to_string(accel) << endl;
    }
}