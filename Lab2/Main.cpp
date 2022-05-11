#include <iostream>
#include <windows.h>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

struct polinomData {
    int length;
    double* coef;
    double x;
    double result;
};

DWORD WINAPI polynom(LPVOID param) {
    polinomData* data = static_cast<polinomData*>(param);
    double result = 0;
    double x = data->x;
    int length = data->length;
    double* coef = data->coef;

    for (int i = 0; i < length; i++) {
        double add = coef[i];
        for (int j = 0; j < length - i; j++) {
            add *= x;
        }
        result += add;
        Sleep(15);
    }

    cout << "p(" << x << ") = " << result << "\n";
    return 0;
}

int main() {
    setlocale(LC_ALL, ".1251");
    polinomData* numeratorData = new polinomData;
    polinomData* denominatorData = new polinomData;

    cout << "¬ведите степень числител€:" << "\n";
    cin >> numeratorData->length;
    cout << "¬ведите коэффициенты числител€:" << "\n";
    numeratorData->coef = new double[numeratorData->length];
    for (int i = 0; i < numeratorData->length; i++) {
        cin >> numeratorData->coef[i];
    }

    cout << "¬ведите степень знаменател€:" << "\n";
    cin >> denominatorData->length;
    cout << "¬ведите коэффициенты знаменател€:" << "\n";
    denominatorData->coef = new double[denominatorData->length];
    for (int i = 0; i < denominatorData->length; i++) {
        cin >> denominatorData->coef[i];
    }

    cout << "¬ведите x:" << "\n";
    cin >> numeratorData->x;
    denominatorData-> x = numeratorData->x;

    HANDLE polinom1 = CreateThread(NULL, 0, polynom, numeratorData, NULL, NULL);
    HANDLE polinom2 = CreateThread(NULL, 0, polynom, denominatorData, NULL, NULL);
    if (polinom1 == NULL || polinom2 == NULL) {
        return GetLastError();
    }
    WaitForSingleObject(polinom1, INFINITE);
    WaitForSingleObject(polinom2, INFINITE);
    CloseHandle(polinom1);
    CloseHandle(polinom2);
    cout << "f(" << numeratorData->x << ") = " << numeratorData->result / denominatorData->result;
    return 0;
}