#include <Windows.h>
#include <fstream>
#include "Common/Readers.h"
#include "Common/Message.h"

using std::fstream;
using std::ofstream;
using std::ifstream;

bool read(string& fileName, HANDLE& mutex, HANDLE& semaphoreRead, HANDLE& semaphoreWrite) {
	WaitForSingleObject(semaphoreRead, INFINITE);
	WaitForSingleObject(mutex, INFINITE);

	string text;
	string name;

	Message* message = new Message(name, text);
	Meta* meta = new Meta;
	fstream f(fileName, std::ios::binary | std::ios::out | std::ios::in);
	if (!f.is_open()) {
		cout << "Ошибка!\nИскомый файл не найден!" << endl;
		return false;
	}

	f.seekp(0, std::ios::beg);
	f.read((char*)meta, sizeof(Meta));
	f.seekg(sizeof(Meta) + meta->begin * sizeof(Message), std::ios::beg);
	f.read((char*)message, sizeof(Message));

	meta->begin = (meta->begin + 1 ) % meta->numberOfMessage;
	f.seekp(0, std::ios::beg);
	f.write((char*)meta, sizeof(Meta));
	cout << message;

	f.close();
	delete meta;
	delete message;

	ReleaseMutex(mutex);
	ReleaseSemaphore(semaphoreWrite, 1, NULL);

	return true;
}

void input(string& fileName, int& numberOfMessage, int& numberOfSender) {

	StringReader sr;
	IntReader ir;

	sr.read(fileName, "Введите имя файла: ", [](string& str) {if (str.find(' ') != string::npos || str.size() > 15) return true; return false; }, "Невалидное имя файла!");
	ir.read(numberOfMessage, "Введите кол-во сообщейний: ", [](int& i) {return i < 1; }, "Невалидное кол-во сообщений!");
	ir.read(numberOfSender, "Введите кол-во потоков Sender: ", [](int& i) {return i < 1; }, "Невалидное кол-во потоков Sender:!");
}

bool createBinaryFile(const string& fileName, const int& numberOfMessage) {

	ofstream file(fileName, std::ios::binary);
	if(!file.is_open()) {
		cout << "Ошибка!\nИскомый файл не найден!" << endl;
		return false;
	}
	Message* message = new Message[numberOfMessage];
	
	Meta* meta = new Meta(0, 0, numberOfMessage);
	file.write((char*)meta, sizeof(Meta));
	file.write((char*)message, sizeof(Message) * numberOfMessage);

	delete meta;
	delete[] message;
	file.close();
	return true;
}

bool Sender(string& fileName, string& semaphoreRead, string& semaphoreWrite) {

	string args = "Sender.exe " + fileName + " " + semaphoreRead + " " + semaphoreWrite;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	BOOL process = CreateProcess(NULL, (char*)args.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	if (!process) {
		cout << "ошибка при создании процесса!!!" << endl;
		return false;
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return true;
}

bool createSenders(const int& numberOfSender, string& fileName, string& semaphoreRead, string& semaphoreWrite) {
	for (int i = 0; i < numberOfSender; i++) {
		if (!Sender(fileName, semaphoreRead, semaphoreWrite)) return false;
	}
	return true;
}

bool readCycle(string& fileName, HANDLE& mutex, HANDLE& semaphoreRead, HANDLE& semaphoreWrite) {

	IntReader ir;
	int chouse = 0;
	ir.read(chouse, "1 - Чтение\n2 - выход\nВвод: ", [](int& i) {return i != 1 && i != 2; }, "Неверный ввод!");
	while (chouse != 2) {
		system("cls");
		if(!read(fileName, mutex, semaphoreRead, semaphoreWrite)) return false;
		ir.read(chouse, "1 - Чтение\n2 - выход\nВвод: ", [](int& i) {return i != 1 && i != 2; }, "Неверный ввод!");
	}
	return true;
}

int main() {
	setlocale(LC_ALL, ".1251");

	string fileName;
	int numberOfMessage = 0;
	int numberOfSender = 0;
	string semaphoreReadS("semaphoreRead");
	string semaphoreWriteS("semaphoreWrite");
	string endOfSendersExeptionS("endOfSendersExeption");

	input(fileName, numberOfMessage, numberOfSender);

	HANDLE semaphoreRead = CreateSemaphore(NULL, 0, numberOfMessage, (char*)semaphoreReadS.c_str());
	HANDLE semaphoreWrite = CreateSemaphore(NULL, numberOfMessage, numberOfMessage, (char*)semaphoreWriteS.c_str());
	HANDLE mutex = CreateMutex(NULL, FALSE, (char*)fileName.c_str());

	if (!createBinaryFile(fileName, numberOfMessage)) return 0;

	if (!createSenders(numberOfSender, fileName, semaphoreReadS, semaphoreWriteS)) return 0;

	if (!readCycle(fileName, mutex, semaphoreRead, semaphoreWrite)) return 0;

	CloseHandle(semaphoreRead);
	CloseHandle(semaphoreWrite);
	CloseHandle(mutex);

	return 0;
}
