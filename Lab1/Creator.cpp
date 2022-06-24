#include <iostream>
#include <fstream>
#include "employee.h"

void employeeInput(int employeeNumber, employee*& e) {

	if (employeeNumber < 0) throw "Невалидное кол-во работников!!!";

	e = new employee[employeeNumber];
	for (int i = 0; i < employeeNumber; i++) {

		std::cout << "Введите идентификационный номер сотрудника: ";
		while (!(std::cin >> e[i].num))
		{
			system("cls");
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Неверный ввод" << std::endl;
			std::cout << "Введите идентификационный номер сотрудника: ";
		}
		if (1 > e[i].num) {
			system("cls");
			std::cout << "Некорректное значение!!!\n";
			std::cout << "Введите идентификационный номер сотрудника: ";
			while (!(std::cin >> e[i].num))
			{
				system("cls");
				std::cin.clear();
				while (std::cin.get() != '\n');
				std::cout << "Неверный ввод" << std::endl;
				std::cout << "Введите идентификационный номер сотрудника: ";
			}
		}
		for (int j = 0; j < i; j++) {
			if (e[i].num == e[j].num) {
				std::cout << "Сотрудник с таким идентификационным номером уже существует!!!\n";
				std::cout << "Введите идентификационный номер сотрудника: ";
				while (!(std::cin >> e[i].num))
				{
					system("cls");
					std::cin.clear();
					while (std::cin.get() != '\n');
					std::cout << "Неверный ввод" << std::endl;
					std::cout << "Введите идентификационный номер сотрудника: ";
				}
				j = -1;
			}
		}

		std::cout << "Введите имя сотрудника: ";
		std::cin >> e[i].name;

		std::cout << "Введите количество отработанных часов: ";
		while (!(std::cin >> e[i].hours) || e[i].hours < 0)
		{
			system("cls");
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Некорректное значение!!!\n";
			std::cout << "Введите количество отработанных часов: ";
		}

		system("cls");
	}
}

void writeEmployee(const employee* e, int employeeNumber, const char* fileName) {

	std::ofstream out(fileName, std::ios::binary);

	out.write((char*)e, static_cast<std::streamsize>(sizeof(employee)) * employeeNumber);

	out.close();
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, ".1251");

	char* fileName = argv[1];
	int employeeNumber = atoi(argv[2]);
	/*int employeeNumber = 4;
	char* fileName = new char[2]{ 'a','\0' };*/

	try
	{
		employee* e = nullptr;

		employeeInput(employeeNumber, e);

		writeEmployee(e, employeeNumber, fileName);

		delete[] e;
	}
	catch (const char* error)
	{
		std::cout << error << std::endl;
	}

	return 0;
}