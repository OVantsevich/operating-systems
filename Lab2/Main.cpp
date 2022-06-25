#include <iostream>
#include "correctInput.h"
#include <windows.h>

using std::cin;
using std::cout;
using std::endl;

struct polinomData {
    int length;
    double* coef;
    double x;
    double result;
};

polinomData* numeratorInput() {

    polinomData* numeratorData = new polinomData;

    correctInput("Введите степень числителя: ", numeratorData->length);
    while (numeratorData->length < 0) {
        system("cls");
        correctInput("Введите степень числителя: ", numeratorData->length);
    } 

    cout << "Введите коэффициенты числителя\n";
    numeratorData->coef = new double[numeratorData->length];
    for (int i = 0; i < numeratorData->length; i++)
        correctInput("", numeratorData->coef[i]);

    correctInput("Введите x: ", numeratorData->x);

    return numeratorData;
}

polinomData* denominatorInput(double x) {

    polinomData* denominatorData = new polinomData;

    correctInput("Введите степень знаменателя: ", denominatorData->length);
    while (denominatorData->length < 0) {
        system("cls");
        correctInput("Введите степень знаменателя: ", denominatorData->length);
    }

    cout << "Введите коэффициенты знаменателя\n";
    denominatorData->coef = new double[denominatorData->length];
    for (int i = 0; i < denominatorData->length; i++)
        correctInput("", denominatorData->coef[i]);

    denominatorData->x = x;

    return denominatorData;
}

DWORD WINAPI polynom(LPVOID param) {

    polinomData* data = static_cast<polinomData*>(param);

    if (nullptr == data) return 0;

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
    data->result = result;

    cout << "p(" << x << ") = " << result << "\n";
    return 0;
}

double fractionInput() {
   
    polinomData* data[2];

    data[0] = numeratorInput();
    data[1] = denominatorInput(data[0]->x);

    HANDLE handle[2];

    handle[0] = CreateThread(NULL, 0, polynom, data[0], NULL, NULL);
    handle[1] = CreateThread(NULL, 0, polynom, data[1], NULL, NULL);

    if (handle[0] == NULL || handle[1] == NULL) {
        return GetLastError();
    }
    WaitForMultipleObjects(2, handle, TRUE, INFINITE);
    CloseHandle(handle[0]);
    CloseHandle(handle[1]);

    if (0 == data[1]->result) throw "\nОшибка - деление на 0\nПроверьте коэффициенты знаменателя";

    cout << "f(" << data[0]->x << ") = " << data[0]->result / data[1]->result;

    delete[] data[0]->coef;
    data[0]->coef = nullptr;
    delete data[0];

    delete[] data[1]->coef;
    data[1]->coef = nullptr;
    delete data[1];
}


int main() {
    setlocale(LC_ALL, ".1251");
    try
    {
        fractionInput();
    }
    catch (const char* error)
    {
        cout << error << endl;
    }
    
    return 0;
}