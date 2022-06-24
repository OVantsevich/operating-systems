#include <iostream>
#include <fstream>
#include <algorithm>
#include "employee.h"

bool employeeNumComparator(employee a, employee b) {
	return (a.num < b.num);
}

int readEmployee(const char* inputFileName, employee*& e) {

	std::ifstream in(inputFileName, std::ios::binary | std::ios::ate);

	if (!in.is_open()) throw "Искомый файл отсутствует!!!\n";

	int employeeNumber = in.tellg() / sizeof(employee);
	in.seekg(0, std::ios::beg);

	e = new employee[employeeNumber];

	in.read((char*)e, static_cast<std::streamsize>(sizeof(employee)) * employeeNumber);

	std::sort(e, e + employeeNumber, employeeNumComparator);

	in.close();

	return employeeNumber;
}

void createReport(const char* outputFileName, const char* inputFileName, int employeeNumber, const employee* e, double salary) {

	if (salary < 0) throw "Невалидное значение зарплаты!!!";
	if (employeeNumber <= 0) throw "Невалидное кол-во работников!!!";

	std::ofstream out(outputFileName);

	out << "Отчет по файлу " << inputFileName << std::endl;
	out << "Номер сотрудника, Имя сотрудника, Часы, Зарплата." << std::endl;

	for (int i = 0; i < employeeNumber; i++) {
		out << e[i].num << " " << e[i].name << " " << e[i].hours << " " << static_cast<double>(e[i].hours * salary) << std::endl;
	}

	out.close();
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, ".1251");

	char* inputFileName = argv[1];
	char* outputFileName = argv[2];
	double salary = atoi(argv[3]);
	//char* inputFileName = new char[2]{ 'a','\0' };
	//char* outputFileName = new char[6]{ 'o','.','t','x','t','\0' };
	//double salary = 0;

	try
	{
		employee* e = nullptr;

		int employeeNumber = readEmployee(inputFileName, e);

		createReport(outputFileName, inputFileName, employeeNumber, e, salary);

		delete[] e;

	}
	catch (const char* error)
	{
		std::cout << error << std::endl;
	}

	return 0;
}