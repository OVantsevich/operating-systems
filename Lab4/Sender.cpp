//#include <windows.h>
//#include <iostream>
//#include <fstream>
//
//#pragma warning(disable : 4996)
//
//using namespace std;
//
//HANDLE writeSemaphore;
//HANDLE readSemaphore;
//CRITICAL_SECTION cs;
//fstream* file;
//char* name;
//
//struct Message {
//	char name[10];
//	char text[20];
//
//	Message() {}
//
//	Message(char* name, char* text) {
//		if (name != nullptr)
//			strcpy(this->name, name);
//		if (text != nullptr)
//			strcpy(this->text, text);
//	}
//};
//
//void write() {
//	WaitForSingleObject(readSemaphore, INFINITE);
//	EnterCriticalSection(&cs);
//
//	char* m;
//	cout << "Введите сообщение : ";
//	m = new char[20];
//	cin >> m;
//	cout << "\n";
//
//	Message* message = new Message(name, m);
//	file->seekp(0, ios::end);
//	int length = file->tellp();
//	file->seekp(length, ios::beg);
//	file->write((char*)message, sizeof(Message));
//
//	LeaveCriticalSection(&cs);
//	ReleaseSemaphore(writeSemaphore, 1, NULL);
//}
//
//int main(int argc, char* argv[]) {
//	setlocale(LC_ALL, ".1251");
//
//	writeSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Write");
//	readSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Read");
//	InitializeCriticalSection(&cs);
//
//	file = new fstream(argv[1], ios::binary | ios::in | ios::out);
//
//	cout << "Введите имя : ";
//	name = new char[10];
//	cin >> name;
//	cout << "\n";
//
//	cout << "1 - Запись\n";
//	cout << "2 - Выход\n";
//	int i;
//	while (true) {
//
//		cout << "Введите команду : ";
//		cin >> i;
//
//		if (i % 2)
//			write();
//		else
//			break;
//	}
//
//	file->close();
//	delete file;
//}