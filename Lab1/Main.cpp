#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "employee.h"
//#include <crtdbg.h>

void creator(char outputFileName[], int numsEmployees) {

	char buff[20];
	char args[100] = "Creator.exe ";

	strcat_s(args, outputFileName);
	strcat_s(args, " ");
	sprintf_s(buff, sizeof(buff) / sizeof(buff[0]), "%d", numsEmployees);
	strcat_s(args, buff);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	WaitForSingleObject(pi.hProcess, INFINITE);
	
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
void reporter(char inputFileName[], char outputFileName[], int salary) {

	char args[100] = "Reporter.exe ";
	char buff[20];

	strcat_s(args, inputFileName);
	strcat_s(args, " ");
	strcat_s(args, outputFileName);
	strcat_s(args, " ");
	sprintf_s(buff, sizeof(buff) / sizeof(buff[0]), "%d", salary);
	strcat_s(args, buff);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	
	WaitForSingleObject(pi.hProcess, INFINITE);
	
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void outEmployee(const char* inputFileName, int employeeNumber) {

	std::ifstream in(inputFileName, std::ios::binary);
	if (!in.is_open())	throw "Искомый файл отсутствует!!!\n";

	employee* e = new employee[employeeNumber];
	in.read((char*)e, static_cast<std::streamsize>(sizeof(employee)) * employeeNumber);

	for (int i = 0; i < employeeNumber; i++) {
		std::cout << e[i].num << " " << e[i].name << " " << e[i].hours << std::endl;
	}
	std::cout << std::endl;
	
	delete[] e;
	in.close();
}

void outReport(char* reportFileName) {

	std::ifstream outReport(reportFileName);
	if (!outReport.is_open()) throw "Файл отчёта не найден!!!\n";

	std::string str;
	while (std::getline(outReport, str)) {
		std::cout << str << std::endl;
	}
	std::cout << std::endl;
	outReport.close();
}

char* create() {

	char *inputFileName = new char[30];
	int employeeNumber;

	std::cout << "Введите имя файла: ";
	std::cin >> inputFileName;
	std::cout << "Введите кол-во работников: ";
	std::cin >> employeeNumber;
	while (employeeNumber < 0) {
		std::cout << "Некорректное значение!!!\n";
		std::cout << "Введите кол-во работников: ";
		std::cin >> employeeNumber;
	}
	std::cout << std::endl;

	creator(inputFileName, employeeNumber);

	outEmployee(inputFileName, employeeNumber);

	return inputFileName;
}

void report(char* inputFileName) {

	char outputFileName[30];
	int salary;

	std::cout << "Введите имя файла отчёта: ";
	std::cin >> outputFileName;
	std::cout << "Введите зарплату: ";
	std::cin >> salary;
	std::cout << std::endl;

	strcat_s(outputFileName, ".txt");

	reporter(inputFileName, outputFileName, salary);

	outReport(outputFileName);

}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, ".1251");

	try
	{
		char* inputFileName = create();
		report(inputFileName);
		delete[] inputFileName;
		//std::cout <<_CrtDumpMemoryLeaks();
	}
	catch (const char* error)
	{
		std::cout << error << std::endl;
	}
	

	system("pause");
	return 0;
}