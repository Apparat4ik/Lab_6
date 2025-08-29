#include <iostream>
#include <stdlib.h>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>
#include <numeric>

using namespace std;

random_device rd;
mt19937 gen(rd());


enum class Sellection{
    SQUARE_MT = 1,
    RECTANGLE_MT = 2,
    GAME_LIFE = 3,
    EXIT = 0
};

void Choice_sellect(Sellection& ch, int sl){
    if (sl == 1) ch = Sellection::SQUARE_MT;
    else if (sl == 2) ch = Sellection::RECTANGLE_MT;
    else if (sl == 3) ch = Sellection::GAME_LIFE;
    else if (sl == 0) ch = Sellection::EXIT;
}

// Функция для задания 1
void SquareMatrix() {
    cout << "=== ЗАДАНИЕ 1 ===" << endl;
    uniform_real_distribution<double> dis(0.0, 100.0);
    
    int N;
    cout << "Введите четный порядок матрицы N: ";
    cin >> N;
    
    if (N % 2 != 0) {
        cout << "Ошибка: N должно быть четным!" << endl;
        return;
    }
    
    // Создаем исходную матрицу
    vector<vector<double>> matrix(N, vector<double>(N));
    cout << "Исходная матрица:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = dis(gen);
            cout << fixed << setprecision(2) << setw(8) << matrix[i][j];
        }
        cout << endl;
    }
    
    // Создаем новую матрицу из левой верхней четверти
    int newSize = N / 2;
    vector<vector<double>> newMatrix(newSize, vector<double>(newSize));
    
    cout << "\nЛевая верхняя четверть:" << endl;
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            newMatrix[i][j] = matrix[i][j];
            cout << fixed << setprecision(2) << setw(8) << newMatrix[i][j];
        }
        cout << endl;
    }
}

// Функция для вычисления суммы цифр числа
int digitSum(int number) {
    int sum = 0;
    while (number != 0) {
        sum += number % 10;
        number /= 10;
    }
    return sum;
}

// Функция для задания 2
void RectangleMatrix() {
    cout << "\n=== ЗАДАНИЕ 2 ===" << endl;
    
    uniform_int_distribution<int> dis(1000, 5000);
    
    int M, N;
    cout << "Введите размеры матрицы M и N: ";
    cin >> M >> N;
    
    // Создаем матрицу
    vector<vector<int>> matrix(M, vector<int>(N));
    cout << "Матрица:" << endl;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = dis(gen);
            cout << setw(6) << matrix[i][j];
        }
        cout << endl;
    }
    
    // Находим строку с минимальной суммой цифр
    int minSum = INT_MAX;
    int minRow = 0;
    
    for (int i = 0; i < M; i++) {
        int rowSum = 0;
        for (int j = 0; j < N; j++) {
            rowSum += digitSum(matrix[i][j]);
        }
        
        if (rowSum < minSum) {
            minSum = rowSum;
            minRow = i;
        }
        
        cout << "Строка " << i << ": сумма цифр = " << rowSum << endl;
    }
    
    cout << "\nСтрока с наименьшей суммой цифр: " << minRow << endl;
    cout << "Сумма цифр: " << minSum << endl;
    cout << "Элементы строки: ";
    for (int j = 0; j < N; j++) {
        cout << matrix[minRow][j] << " ";
    }
    cout << endl;
}

// Функция для инициализации случайными клетками
void randomInit(vector<vector<bool>>& grid, double density = 0.3) {

    int height = grid.size();
    int width = grid[0].size();
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = gen() % 2 < density;
        }
    }
}

void setEater(vector<vector<bool>>& grid, int x, int y) {
    grid[y][x] = true;
    grid[y][x+1] = true;
    grid[y+1][x] = true;
    grid[y+1][x+2] = true;
    grid[y+2][x+2] = true;
    grid[y+3][x+2] = true;
    grid[y+3][x+3] = true;
}


void setGlider(vector<vector<bool>>& grid, int x, int y) {
    grid[y][x+1] = true;
    grid[y+1][x+2] = true;
    grid[y+2][x] = true;
    grid[y+2][x+1] = true;
    grid[y+2][x+2] = true;
}

// Функция для подсчета живых соседей
int countNeighbors(const vector<vector<bool>>& grid, int x, int y) {
    int count = 0;
    int height = grid.size();
    int width = grid[0].size();
    
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            
            int nx = (x + j + width) % width;
            int ny = (y + i + height) % height;
            
            if (grid[ny][nx]) {
                count++;
            }
        }
    }
    return count;
}

// Функция для обновления поколения
void updateGeneration(vector<vector<bool>>& grid) {
    int height = grid.size();
    int width = grid[0].size();
    vector<vector<bool>> newGrid = grid;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int neighbors = countNeighbors(grid, j, i);
            
            if (grid[i][j]) {
                // Клетка жива
                newGrid[i][j] = (neighbors == 2 || neighbors == 3);
            } else {
                // Клетка мертва
                newGrid[i][j] = (neighbors == 3);
            }
        }
    }
    
    grid = newGrid;
}

// Функция для отображения текущего состояния
void displayGrid(const vector<vector<bool>>& grid) {
    int height = grid.size();
    int width = grid[0].size();
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << (grid[i][j] ? "■" : " ");
        }
        cout << endl;
    }
}

// Функция для получения количества живых клеток
int getLiveCount(const vector<vector<bool>>& grid) {
    int count = 0;
    int height = grid.size();
    int width = grid[0].size();
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j]) count++;
        }
    }
    return count;
}

void GameLife() {
    cout << "\n=== ЗАДАНИЕ 3: СТОЛКНОВЕНИЕ ПОЖИРАТЕЛЯ С ГЛАЙДЕРОМ ===" << endl;
    
    int width = 40;
    int height = 20;
    int generations = 150;
    
    vector<vector<bool>> grid(height, vector<bool>(width, false));
    
    int choice;
    cout << "Выберите режим:" << endl;
    cout << "1. Случайная генерация" << endl;
    cout << "2. Пожиратель (eater)" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    bool right_choice = false;
    
    while(!right_choice){
        switch (choice) {
            case 1:
                right_choice = true;
                randomInit(grid, 0.3);
                break;
                
            case 2:
                right_choice = true;
                setEater(grid, width - 15, height/2 + 1);
                setGlider(grid, 20, height/2 - 2);
                break;
                
            default:
                cout << "Неверный выбор" << endl;
                break;
        }
    }
    
    int alive_count;
    for (int gen = 0; gen < generations; gen++) {
        system("clear");
        alive_count = getLiveCount(grid);
        cout << "Поколение: " << gen + 1 << "/" << generations << endl;
        cout << "Живых клеток: " << alive_count << endl;\
        
        if (alive_count == 0){break;}
        
        displayGrid(grid);
        
        updateGeneration(grid);
        
        this_thread::sleep_for(chrono::milliseconds(150));
    }
    
    cout << "\nСимуляция завершена" << endl;
}


int main() {
    int  inpt;
    while (true) {
        cout << "\n=== ГЛАВНОЕ МЕНЮ ===" << endl;
        cout << "1. Квадратная матрица и ее четверть" << endl;
        cout << "2. Матрица и сумма цифр" << endl;
        cout << "3. Игра Жизнь Конвея" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите задание: ";
        cin >> inpt;
        Sellection sellect;
        
        Choice_sellect(sellect, inpt);
        
        switch (sellect) {
            case Sellection::SQUARE_MT:
                SquareMatrix();
                break;
            case Sellection::RECTANGLE_MT:
                RectangleMatrix();
                break;
            case Sellection::GAME_LIFE:
                GameLife();
                break;
            case Sellection::EXIT:
                cout << "Выход из программы..." << endl;
                return 0;
                break;
            default:
                cout << "Неверный выбор!" << endl;
        }
        
        if (sellect != Sellection::EXIT) {
            cout << "\nНажмите Enter для продолжения..." << endl;
            cin.get();
            system("clear");
        }
    }
    
    return 0;
}
