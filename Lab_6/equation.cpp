#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>

#define EPS 0.001
#define MAX_ITER 100

using namespace std;


ostream& operator<<(ostream& os, vector<vector<double>>& vc){
    for (vector<double> row : vc){
        for (double it : row){
            os << setprecision(4) << it << '\t';
        }
        os << endl;
    }
    return os;
}


ostream& operator<<(ostream& os, vector<double>& vc){
    for (double it : vc){
        os << setprecision(4) << it << '\t';
    }
    return os;
}



// Метод LU-разложения
vector<double> solveLU(vector<vector<double>> A, vector<double> b, int n) {
    cout << "\n=== МЕТОД LU-РАЗЛОЖЕНИЯ ===" << endl;
    
    vector<vector<double>> L(n, vector<double>(n, 0.0));
    vector<vector<double>> U(n, vector<double>(n, 0.0));
    vector<double> y(n), x(n);

    // Инициализация диагонали L единицами
    for (int i = 0; i < n; i++) {
        L[i][i] = 1.0;
    }

    // LU-разложение
    for (int i = 0; i < n; i++) {
        // Вычисление элементов матрицы U
        for (int k = i; k < n; k++) {
            double sum = 0.0;
            for (int j = 0; j < i; j++) {
                sum += L[i][j] * U[j][k];
            }
            U[i][k] = A[i][k] - sum;
        }

        // Проверка на нулевой диагональный элемент
        if (abs(U[i][i]) == 0) {
            cout << "Ошибка: нулевой диагональный элемент U[" << i << "][" << i << "]" << endl;
            return vector<double>();
        }

        // Вычисление элементов матрицы L
        for (int k = i + 1; k < n; k++) {
            double sum = 0.0;
            for (int j = 0; j < i; j++) {
                sum += L[k][j] * U[j][i];
            }
            L[k][i] = (A[k][i] - sum) / U[i][i];
        }
    }

    // Вывод матриц L и U
    cout << "\nМатрица L:" << endl;
    cout << L << endl;
    cout << "\nМатрица U:" << endl;
    cout << U << endl;

    // Решение системы Ly = b (прямой ход)
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++) {
            sum += L[i][j] * y[j];
        }
        y[i] = b[i] - sum;
    }

    cout << "\nВектор y:" << endl;
    cout << y << endl;

    // Решение системы Ux = y (обратный ход)
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += U[i][j] * x[j];
        }
        x[i] = (y[i] - sum) / U[i][i];
    }

    // Вычисление определителя
    double det = 1.0;
    for (int i = 0; i < n; i++) {
        det *= U[i][i];
    }
    cout << "\nОпределитель матрицы A: " << det << endl;

    return x;
}


vector<double> solveSeidel(const vector<vector<double>>& A, const vector<double>& b, int n) {
    cout << "\n=== МЕТОД ЗЕЙДЕЛЯ ===" << endl;

    // Проверка диагональных элементов
    for (int i = 0; i < n; i++) {
        if (abs(A[i][i]) == 0) {
            cout << "Ошибка: нулевой диагональный элемент A[" << i << "][" << i << "]" << endl;
            return {};
        }
    }

    // Проверка условия сходимости (диагональное преобладание)
    bool convergent = true;
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                sum += abs(A[i][j]);
            }
        }
        if (abs(A[i][i]) <= sum) {
            cout << "Предупреждение: диагональное преобладание не выполнено для строки " << i + 1 << endl;
            convergent = false;
        }
    }

    if (convergent) {
        cout << "Условие диагонального преобладания выполнено - метод должен сходиться" << endl;
    }

    // Итерационный процесс
    vector<double> x_old(n, 0.0);  // Начальное приближение x(0) = 0
    vector<double> x_new(n, 0.0);
    int k = 0;

    cout << "\nИтерационный процесс:" << endl;
    cout << "k\tx1\t\tx2\t\tx3\t\tx4\t\t||x(k+1)-x(k)||" << endl;

    do {
        x_old = x_new;  // Сохраняем предыдущее приближение для вычисления нормы

        // Вычисление нового приближения по методу Зейделя
        // Формула (19): x_i^(k) = 1/a_ii * (f_i - сумма a_ij * x_j)
        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            
            // Сумма с уже вычисленными значениями x_new[j] для j < i (текущая итерация)
            for (int j = 0; j < i; j++) {
                sum += A[i][j] * x_new[j];
            }
            
            // Сумма со старыми значениями x_old[j] для j > i (предыдущая итерация)
            for (int j = i + 1; j < n; j++) {
                sum += A[i][j] * x_old[j];
            }
            
            // Вычисляем новое значение согласно формуле (19)
            x_new[i] = (b[i] - sum) / A[i][i];
        }

        // Вычисление нормы разности
        double norm = 0.0;
        for (int i = 0; i < n; i++) {
            norm = max(norm, abs(x_new[i] - x_old[i]));
        }

        cout << k << "\t";
        for (int i = 0; i < n; i++) {
            cout << fixed << setprecision(4) << x_new[i] << "\t";
        }
        cout << scientific << setprecision(3) << norm << endl;

        // Проверка условия останова
        if (norm < EPS) {
            cout << "\nСходимость достигнута за " << k + 1 << " итераций" << endl;
            return x_new;
        }

        k++;

    } while (k < MAX_ITER);

    cout << "Предупреждение: максимальное количество итераций достигнуто, метод расходится" << endl;
    return {};
}


// Функция для вывода исходной системы
void printOriginalSystem(const vector<vector<double>>& A, const vector<double>& b, int n) {
    cout << "=== ИСХОДНАЯ СИСТЕМА УРАВНЕНИЙ ===" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j == 0) {
                cout << (A[i][j] >= 0 ? " " : "") << A[i][j] << "x" << j + 1;
            } else {
                cout << (A[i][j] >= 0 ? " + " : " - ") << abs(A[i][j]) << "x" << j + 1;
            }
        }
        cout << " = " << b[i] << endl;
    }
}

int main() {
    // Система уравнений с подставленными значениями M=-1.21, N=0.2, P=0.88
    int n = 4;
    vector<vector<double>> A = {
        {-1.21, -0.04,  0.21, -18.0},
        { 0.25, -1.23,  0.2,  -0.09},
        {-0.21,  0.2,   0.8,  -0.13},
        { 0.15, -1.31,  0.06,  0.88}
    };
    vector<double> b = {-1.24, 0.88, 2.56, -1.21};
    
    printOriginalSystem(A, b, n);

    // Решение методом LU-разложения
    vector<double> solution_lu = solveLU(A, b, n);
    if (!solution_lu.empty()) {
        cout << "\nРешение методом LU-разложения:" << endl;
        cout << solution_lu << endl;
    }

    // Решение методом простой итерации
    vector<double> solution_seidel = solveSeidel(A, b, n);
    if (!solution_seidel.empty()) {
        cout << "\nРешение методом простой итерации:" << endl;
        cout << solution_seidel << endl;
    } else {
        cout << "Решение не получится найти данным методом. Найдена расходимость" << endl;
    }

    return 0;
}
