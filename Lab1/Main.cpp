#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include "employee.h"

using namespace std;

void creator(char filename[], int numsEmployees) {
	char* buff = new char[10];
	char args[100] = "Creator.exe";
	strcat_s(args, " ");
	strcat_s(args, _itoa(numsEmployees, buff, 10));
	strcat_s(args, " ");
	strcat_s(args, filename);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
void reporter(char binFile[], char txtFile[], double salary) {
	char args[100] = "Reporter.exe ";
	char* buff = new char[20];
	strcat_s(args, binFile);
	strcat_s(args, " ");
	strcat_s(args, txtFile);
	strcat_s(args, " ");
	strcat_s(args, _gcvt(salary, 10, buff));
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	CreateProcess(NULL, args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

int main(int argc, char* argv[]) {
	char binFile[30];
	char txtFile[30];
	int numsEmpl;
	int salary;
	cout << "Enter binFile name: ";
	cin >> binFile;
	cout << "Enter number of employees: ";
	cin >> numsEmpl;
	cout << endl;
	creator(binFile, numsEmpl);
	employee* e = new employee[numsEmpl];
	fstream fin(binFile, ios::in | ios::binary | ios::ate);
	fin.seekg(0, ios::end);
	int n = fin.tellg() / sizeof(employee);
	fin.seekg(0, ios::beg);
	fin.read((char*)e, sizeof(employee) * n);
	for (int i = 0; i < numsEmpl; i++) {
		cout << e[i].num << " " << e[i].name << " " << e[i].hours << endl;
	}
	fin.close();
	cout << "Creator stops working" << endl << endl;
	cout << "Enter report filename: ";
	cin >> txtFile;
	cout << "Enter salary: ";
	cin >> salary;
	cout << endl;
	reporter(binFile, txtFile, salary);
	fstream txtfin(txtFile);
	string str;
	while (getline(txtfin, str)) {
		cout << str << endl;
	}
	system("pause");
	return 0;
}