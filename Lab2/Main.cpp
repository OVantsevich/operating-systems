#include <iostream>
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

    cout << "Введите степень числителя: ";
    while (!(cin >> numeratorData->length) || numeratorData->length < 0)
    {
        system("cls");
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Ошибка ввода!!!" << std::endl;
        std::cout << "Введите степень числителя: ";
    }

    cout << "Введите коэффициенты числителя: ";
    numeratorData->coef = new double[numeratorData->length];
    for (int i = 0; i < numeratorData->length; i++) {
        while (!(cin >> numeratorData->coef[i]))
        {
            system("cls");
            std::cin.clear();
            while (std::cin.get() != '\n');
            std::cout << "Ошибка ввода!!!" << std::endl;
            std::cout << "Введите коэффициенты числителя: ";
        }
    }

    cout << "Введите x: ";
    while (!(cin >> numeratorData->x))
    {
        system("cls");
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Ошибка ввода!!!" << std::endl;
        std::cout << "Введите x: ";
    }

    return numeratorData;
}

polinomData* denominatorInput(double x) {

    polinomData* denominatorData = new polinomData;

    cout << "Введите степень знаменателя: ";
    while (!(cin >> denominatorData->length) || denominatorData->length < 0)
    {
        system("cls");
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Ошибка ввода!!!" << std::endl;
        std::cout << "Введите степень знаменателя: ";
    }

    cout << "Введите коэффициенты знаменателя: ";
    denominatorData->coef = new double[denominatorData->length];
    for (int i = 0; i < denominatorData->length; i++) {
        while (!(cin >> denominatorData->coef[i]))
        {
            system("cls");
            std::cin.clear();
            while (std::cin.get() != '\n');
            std::cout << "Ошибка ввода!!!" << std::endl;
            std::cout << "Введите коэффициенты знаменателя: ";
        }
    }

    denominatorData->x = x;

    return denominatorData;
}

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
    data->result = result;

    cout << "p(" << x << ") = " << result << "\n";
    return 0;
}

double fractionInput() {
   
    polinomData* data[2];

    data[0] = numeratorInput();
    data[1] = denominatorInput(data[0]->x);

    HANDLE polinomNumerator = CreateThread(NULL, 0, polynom, data[0], NULL, NULL);
    HANDLE polinomDenominator = CreateThread(NULL, 0, polynom, data[1], NULL, NULL);
   
    if (polinomNumerator == NULL || polinomDenominator == NULL) {
        return GetLastError();
    }
    WaitForSingleObject(polinomNumerator, INFINITE);
    WaitForSingleObject(polinomDenominator, INFINITE);
    CloseHandle(polinomNumerator);
    CloseHandle(polinomDenominator);

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