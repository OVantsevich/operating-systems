#pragma once
template <class T>
void correctInput(const char* message, T& dest) {
    std::cout << message;
    while (!(std::cin >> dest))
    {
        system("cls");
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Неверный ввод!" << std::endl;
        std::cout << message;
    }
}