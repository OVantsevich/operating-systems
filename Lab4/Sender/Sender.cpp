#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "\operating-systems\Lab4\Common\Message.h"
#include "\operating-systems\Lab4\Common\Readers.h"

using std::fstream;

void write(string& fileName, HANDLE& mutex, HANDLE& semaphoreRead, HANDLE& semaphoreWrite) {
	WaitForSingleObject(semaphoreWrite, INFINITE);
	WaitForSingleObject(mutex, INFINITE);

	string text;
	string name;
	StringReader sr;
	sr.read(text, "Введите сообщение: ", [](string& str) {return str.size() > 19; }, "Введённое сообщение слишко длинное!!!");
	sr.read(name, "Введите имя процесса: ", [](string& str) {return str.size() > 9; }, "Введённое имя слишко длинное!!!");

	Message* message = new Message(name, text);
	Meta* meta = new Meta;
	fstream f(fileName, std::ios::binary | std::ios::in | std::ios::out);
	if (!f.is_open()) {
		cout << "Ошибка!\nИскомый файл не найден!" << endl;
		return;
	}

	f.read((char*)meta, sizeof(Meta));
	f.seekp(sizeof(Meta) + meta->end * sizeof(Message), std::ios::beg);
	f.write((char*)message, sizeof(Message));

	meta->end = (meta->end + 1) % meta->numberOfMessage;
	f.seekp(0, std::ios::beg);
	f.write((char*)meta, sizeof(Meta));

	f.close();
	delete meta;
	delete message;

	ReleaseMutex(mutex);
	ReleaseSemaphore(semaphoreRead, 1, NULL);
}

int main(int args, char* argv[]) {
	setlocale(LC_ALL, ".1251");

	HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, argv[1]);
	HANDLE semaphoreRead = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, argv[2]);
	HANDLE semaphoreWrite = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, argv[3]);
	if (mutex == NULL || semaphoreRead == NULL || semaphoreWrite == NULL) {
		cout << "Ошибка в переданных данных!!!";
		return 0;
	}

	string fileName = argv[1];
	StringReader sr;
	IntReader ir;
	int numberOfMessage = atoi(argv[2]);

	int chouse = 0;
	ir.read(chouse, "1 - запись\n2 - выход\nВвод: ", [](int& i) {return i != 1 && i != 2; }, "Неверный ввод!");
	while (chouse != 2) {
		system("cls");
		write(fileName, mutex, semaphoreRead, semaphoreWrite);
		system("cls");
		ir.read(chouse, "1 - запись\n2 - выход\nВвод: ", [](int& i) {return i != 1 && i != 2; }, "Неверный ввод!");
	}
	return 0;
}