#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class GridElement{
    // Родительский абстрактный класс для
    // удобного хранения элементов и общей функции getResistance
    public:
        virtual float getResistance() const = 0;

};

class Resistor: public GridElement{
    // Отедльный резистор
    protected:
        string name;
        float resistance;
    public:
        Resistor(float res, string name_val) : resistance(res), name(name_val){}
        float getResistance() const override {return resistance;}
        string get_name() const {return name;}
};

class SeriesConnection: public GridElement{
    // Последовательное соединение
    protected:
        // Может состоять не только из резисторов, но и из параллельных и последовательных соеденений
        vector<GridElement*> elements;
    public:
        void addElement(GridElement* el) {elements.push_back(el);}
        float getResistance() const override {
            // ∑R = R1 + R2 + ... + Rn
            float res = 0;
            for (GridElement* el : elements){
                res += el->getResistance();
            }
            return res;
        }
};

class ParallelConnection: public GridElement{
    protected:
        // Может состоять не только из резисторов, но и из параллельных и последовательных соеденений
        vector<GridElement*> elements;
    public:
        void addElement(GridElement* el) {elements.push_back(el);}
        float getResistance() const override {
            // ∑R = 1 / (1 / R1 + 1 / R2 + ... + 1 / Rn)
            float res = 0;
            for (GridElement* el : elements){
                res += 1 / (el->getResistance());
            }
            return 1 / res;
        }
};

class GridParser {
    // Парсер строки в общий GridElement 
    private:
        string input;
        size_t pos;
        // Работаем с уже заранее созданными в main резисторами
        vector<Resistor>& resistors;
        
        void skipWhitespace() {
            // Игнорировать пробел
            while (pos < input.size() && isspace(input[pos])) {
                pos++;
            }
        }

        bool isResistorChar(char c) {
            // Нужно чтобы понимать, когда закончилось имя резистора
            // на пробел isalnum будет false
            return isalnum(c);
        }
        
        GridElement* parseElement() {
            skipWhitespace();
            
            char current = input[pos];
            
            if (current == '(') {
                return parseSeries();
            } else if (current == '[') {
                return parseParallel();
            } else {
                return parseResistor();
            }
            // По-хорошему нужна обработка неподдерживаемых частей строк,
            // но для простоты можно думать, что это забота внешней функции
        }
        
        GridElement* parseResistor() {
            // Обработка резистора
            skipWhitespace();
            string name;
            
            while (pos < input.size() && isResistorChar(input[pos])) {
                // Пока не наткнулись на пробел, читаем имя резистора
                name += input[pos];
                pos++;
            }
    
            for (const Resistor& resistor : resistors) {
                if (resistor.get_name() == name){
                    return const_cast<Resistor*>(&resistor); // Указатель на реальный объект
                }
            }
            // Нужна защита от резисторов с несуществующими именами (даже IDE подсвечивает, хы) 
        }

        GridElement* parseSeries() {
            // Обаботка последовательного соединения
            pos++;
            
            SeriesConnection* seriesCon = new SeriesConnection();
            
            while (pos < input.size() && input[pos] != ')') {
                GridElement* elem = parseElement();
                seriesCon->addElement(elem);
                skipWhitespace();
            }
            pos++;

            // думаем, что открывашаяся скобочка всегда закрывается

            return seriesCon;
        }
        
        GridElement* parseParallel() {
            // Обаботка параллельного соединения
            pos++;
            
            ParallelConnection* parallelCon = new ParallelConnection();
            
            while (pos < input.size() && input[pos] != ']') {
                GridElement* elem = parseElement();
                parallelCon->addElement(elem);
                skipWhitespace();
            }
            
            pos++;

            // думаем, что открывашаяся скобочка всегда закрывается

            return parallelCon;
        }
        
    public:

        GridParser(vector<Resistor>& resistorsLink) 
            : resistors(resistorsLink) {}
        
        GridElement* parse(string& inStr) {
            input = inStr;
            pos = 0;
            skipWhitespace();
            
            GridElement* result = parseElement();
            
            return result;
        }
};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int n;
    cin >> n; cin.ignore();
    vector<Resistor> resistors;
    for (int i = 0; i < n; i++) {
        string name;
        int r;
        cin >> name >> r; cin.ignore();
        // Создаём резисторы
        resistors.push_back(Resistor(float(r), name));
        cerr << r << endl;
    }

    string circuit;
    getline(cin, circuit);

    // С помощью парсера получаем один эквивалентный элемент сети
    GridParser parser = GridParser(resistors);
    GridElement* eqvElement = parser.parse(circuit);
    float eqv_r = eqvElement-> getResistance();

    // Точность - 1 знак после запятой
    cout << fixed;
    cout.precision(1);
    cout << round(eqv_r * 10) / 10 << endl;
}