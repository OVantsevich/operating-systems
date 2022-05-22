#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <fstream>
namespace fs=std::filesystem;

#pragma warning(disable : 4996)

using namespace std;

CRITICAL_SECTION cs;
HANDLE writeSemaphore;
HANDLE readSemaphore;
int messagesNumber;
int senderNumber;
char fileName[20];
fstream* file;

struct Message {
	char name[10];
	char text[20];

	Message() {}

	Message(char* name, char* text) {
		if (name != nullptr)
			strcpy(this->name, name);
		if (text != nullptr)
			strcpy(this->text, text);
	}
};

void read() {
	WaitForSingleObject(writeSemaphore, INFINITE);
	EnterCriticalSection(&cs);

	Message* message = new Message();

	file->seekg(0, ios::end);
	int length = file->tellg();
	file->seekg(length - sizeof(Message), ios::beg);

	file->read((char*)message, sizeof(Message));
	cout << "Имя : " << message->name << "\nТекст : " << message->text << "\n"<< endl;

	char* fullName = new char[25];
	strcpy(fullName, fileName);
	strcat(fullName, ".bin");

	std::filesystem::resize_file(fileName, length - sizeof(Message));

	LeaveCriticalSection(&cs);
	ReleaseSemaphore(readSemaphore, 1, NULL);
}

void input() {

	cout << "Введите имя файла : ";
	cin >> fileName;
	cout << "\n";

	cout << "Введите кол-во сообщений : ";
	cin >> messagesNumber;
	cout << "\n";

	cout << "Введите кол-во процессв sender : ";
	cin >> senderNumber;
	cout << "\n";
	ofstream fout(fileName, ios::binary);
	fout.close();
}

void createProcesses() {
	
}

void main() {
	setlocale(LC_ALL, ".1251");

	input();
	STARTUPINFO* si = new STARTUPINFO[senderNumber];
	PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[senderNumber];

	char data[50] = "Sender.exe ";
	strcat(data, fileName);
	strcat(data, " ");
	for (int i = 0; i < senderNumber; i++) {
		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);

		CreateProcess(NULL, data, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i]);
	}
	
	writeSemaphore = CreateSemaphore(NULL, 0, messagesNumber, "Write");
	readSemaphore = CreateSemaphore(NULL, messagesNumber, messagesNumber, "Read");
	InitializeCriticalSection(&cs);



	system("cls");

	cout << "1 - Чтение\n";
	cout << "2 - Выход\n";
	int i;
	file = new fstream(fileName, ios::binary | ios::in | ios::out);
	while (true) {

		cout << "Введите команду : ";
		cin >> i;
		
		if (i % 2)
			read();
		else
			break;
	}

	file->close();
	delete file;

}
