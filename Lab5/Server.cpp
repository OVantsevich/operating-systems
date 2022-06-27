#include <iostream>
#include <fstream>
#include <Windows.h>
#include "common/Order.h"
#include "common/Readers.h"

using std::ifstream;
using std::ofstream;
using std::fstream;

char READ = 'r';
char MODIFY = 'm';
char ORDER_NOT_FOUND = 'n';
char ORDER_FOUND = 'f';
char EXIT = 'e';
char END_OPERATION = 'd';
char END_MODIFY = 'n';

bool findOrder(string& fileName, int orderNumber, Order& order) {

	ifstream in(fileName, std::ios::binary);
	if (!in.is_open()) {
		cout << "Ошибка!\nИскомый файл не найден!" << endl;
		return false;
	}

	Order *curOrder = new Order();

	while (in.peek() != EOF)
	{
		in.read((char*)curOrder, sizeof(Order));
		if (curOrder->n == orderNumber) {
			order = curOrder;
			in.close();
			return true;
		}
	}

	delete curOrder;

	in.close();

	return true;
}

//void modify(Order order) {
//	fstream f(fileName, std::ios::binary | std::ios::in | std::ios::out);
//	int pos = 0;
//	int orderSize = sizeof(Order);
//	while (f.peek() != EOF)
//	{
//		Order o;
//		f.read((char*)&o, sizeof(Order));
//		if (order.n == o.n) {
//			f.seekp(pos * orderSize, std::ios::beg);
//			f.write((char*)&order, sizeof(Order));
//			f.close();
//			return;
//		}
//		else {
//			pos++;
//		}
//	}
//}

bool createProcessClient(HANDLE write, HANDLE read) {
	char comLine[333];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	sprintf_s(comLine, "Client.exe %d %d", (int)write, (int)read);
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(NULL, comLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "Ошибка при создании процесса!" << endl;
		return false;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return true;
}

DWORD WINAPI client(LPVOID data)
{
	HANDLE writePipe = nullptr;
	HANDLE readPipe = nullptr;
	HANDLE clientWritePipe = nullptr;
	HANDLE clientReadPipe = nullptr;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	HANDLE mutexRead = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "mutexRead");
	HANDLE mutexWrite = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "mutexWrite");

	string fileName((char*)data);

	if (mutexRead == NULL || mutexWrite == NULL) {
		cout << "Ошибка связи!" << endl;
		return 0;
	}

	if (!(CreatePipe(&clientReadPipe, &writePipe, &sa, 0) && CreatePipe(&readPipe, &clientWritePipe, &sa, 0))) {
		cout << "Ошибка связи!" << endl;
		return 0;
	}
	if (!createProcessClient(clientWritePipe, clientReadPipe)) return 0;

	while (true) {
		DWORD bytesRead;
		DWORD bytesWrite;

		char clientWant;
		if (!ReadFile(readPipe, &clientWant, sizeof(clientWant), &bytesRead, NULL)) {
			cout << "Ошибка связи!"; 
			return 0;
		}

		if (clientWant == READ) {
			WaitForSingleObject(mutexWrite, INFINITE);

			int orderNumber = 0;
			if (!ReadFile(readPipe, &orderNumber, sizeof(int), &bytesRead, NULL) || orderNumber < 1) {
				cout << "Ошибка связи!";
				return 0;
			}
			Order* order = new Order();
			
			if (findOrder(fileName, orderNumber, *order)) {
				WriteFile(writePipe, &ORDER_FOUND, sizeof(ORDER_FOUND), &bytesWrite, NULL);
				WriteFile(writePipe, order, sizeof(Order), &bytesWrite, NULL);
			}
			else {
				WriteFile(writePipe, &ORDER_NOT_FOUND, sizeof(ORDER_NOT_FOUND), &bytesWrite, NULL);
			}

			char end;
			ReadFile(readPipe, &end, sizeof(end), &bytesRead, NULL);
			ReleaseMutex(mutexWrite);
		}
		else if (clientWant == MODIFY) {

			Order order;
			ReadFile(readPipe, &order, sizeof(Order), &bytesRead, NULL);
			//modify(order);
			WriteFile(writePipe, &END_MODIFY, sizeof(END_MODIFY), &bytesWrite, NULL);

			char end;
			ReadFile(readPipe, &end, sizeof(end), &bytesRead, NULL);

		}
	}
	return 0;
}

bool outFile(string& fileName, int numberOfOrders) {

	ifstream in(fileName, std::ios::binary);

	Order* order = new Order();

	if (!in.is_open()) {
		cout << "Ошибка!\nИскомый файл не найден!" << endl;
		return false;
	}

	while (in.peek() != EOF) {
		in.read((char*)order, sizeof(Order));
		cout << order;
	}

	delete order;

	in.close();

	return true;
}

bool createClients(int numberOfClients, string& fileName, HANDLE*& clients) {

	for (int i = 0; i < numberOfClients; i++) {
		clients[i] = CreateThread(NULL, NULL, client, (char*)fileName.c_str(), 0, NULL);
		if (!clients[i]) return false;
	}
	WaitForMultipleObjects(numberOfClients, clients, TRUE, INFINITE);
	return true;
}

bool createBinaryFile(string& fileName) {

	ofstream out(fileName, std::ios::binary);

	if (!out.is_open()) {
		cout << "Ошибка!\nИскомый файл не найден!" << endl;
		return false;
	}

	int numberOfOrders = 0;;
	IntReader ir;

	ir.read(numberOfOrders, "Введите кол-во записей: \n", [](int& i) {return i < 1; }, "Невалидное кол-во записей!");

	Order* orders = new Order[numberOfOrders];
	for (int i = 0; i < numberOfOrders; i++) {
		system("cls");
		orders[i].inputOrder();
	}
	system("cls");

	out.write((char*)orders, static_cast<std::streamsize>(sizeof(Order)) * numberOfOrders);

	delete[] orders;

	out.close();

	outFile(fileName, numberOfOrders);

	return true;
}

int main() {
	setlocale(LC_ALL, ".1251");

	string fileName;
	string mutexReadS("mutexRead");
	string mutexWriteS("mutexWrite");
	int numberOfClients;

	IntReader ir;
	StringReader sr;
	sr.read(fileName, "Введите имя файла: ", [](string& str) { if (str.find(' ') != string::npos || str.size() > 15) return true; return false; }, "Невалидное имя файла!");
	ir.read(numberOfClients, "Введите кол-во процессов Client: ", [](int& i) {return i < 1; }, "Невалидное ол-во процессов Client!");

	createBinaryFile(fileName);

	HANDLE mutexRead = CreateMutex(NULL, FALSE, (char*)mutexReadS.c_str());
	HANDLE mutexWrite = CreateMutex(NULL, FALSE, (char*)mutexWriteS.c_str());

	HANDLE *clients = new HANDLE[numberOfClients];
	createClients(numberOfClients, fileName, clients);
	

	system("cls");
	system("pause");
	return 0;
}