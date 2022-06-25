#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "employee.h"
#include "CorrectInput.h"
//#include <crtdbg.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;

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
	BOOL process = CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	if (!process) throw "Ошибка в создании процесса!!!\n";

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
	BOOL process = CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	if (!process) throw "Ошибка в создании процесса!!!\n";

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
		cout << e[i].num << " " << e[i].name << " " << e[i].hours << endl;
	}
	cout << endl;

	delete[] e;
	in.close();
}

void outReport(char* reportFileName) {

	std::ifstream outReport(reportFileName);
	if (!outReport.is_open()) throw "Файл отчёта не найден!!!\n";

	string str;
	while (std::getline(outReport, str)) {
		cout << str << endl;
	}
	cout << endl;
	outReport.close();
}

char* checkName() {

	string name;
	cout << "Введите имя файла: ";

	std::getline(cin, name);
	while (string::npos != name.find(' ') || name.size() > 20) {
		system("cls");
		cout << "Неверное имя файла!" << endl;
		cout << "Введите имя файла: ";
		std::getline(cin, name);
	}

	char* res = new char[name.size() + 1];
	res[name.size()] = '\0';
	name.copy(res, name.size());

	return res;
}

char* create() {

	char* inputFileName;
	int employeeNumber;

	inputFileName = checkName();
	corretInput<int>("Введите кол-во работников: ", employeeNumber);
	while ((employeeNumber < 0)) {
		cout << "Некорректное значение!!!\n";
		corretInput<int>("Введите кол-во работников: ", employeeNumber);
	}
	cout << endl;

	creator(inputFileName, employeeNumber);

	outEmployee(inputFileName, employeeNumber);

	return inputFileName;
}

void report(char* inputFileName) {

	char outputFileName[30];
	int salary;

	cout << "Введите имя файла отчёта: ";
	cin >> outputFileName;
	corretInput("Введите зарплату: ", salary);
	cout << endl;

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
		//cout <<_CrtDumpMemoryLeaks();
	}
	catch (const char* error)
	{
		cout << error << endl;
	}

	system("pause");
	return 0;
}