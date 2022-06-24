#include <iostream>
#include <windows.h>
#include <ctime>
#include "SyncQueue.h"

using std::cout;
using std::cin;
using std::endl;

HANDLE start;

struct producerData {
	int serialNumber;
	int numberOfCycles;
	SyncQueue* queue;
};

struct consumerData {
	int numberOfCycles;
	SyncQueue* queue;
};

DWORD WINAPI producer(LPVOID data)
{
	WaitForSingleObject(start, INFINITE);

	producerData* pData = (producerData*)data;

	for (int i = 0; i < pData->numberOfCycles; i++)
	{
		pData->queue->Insert(pData->serialNumber);
		cout << "ѕроизведено число: " << pData->serialNumber << endl;
		Sleep(7);
	}

	return 0;
}

DWORD WINAPI consumer(LPVOID data)
{
	WaitForSingleObject(start, INFINITE);

	consumerData* cData = (consumerData*)data;

	for (int i = 0; i < cData->numberOfCycles; i++)
	{
		int x = cData->queue->Remove();
		cout << "”потреблено число: " << x << endl;
		Sleep(7);
	}

	return 0;
}

SyncQueue* createQueue() {

	int size = 0;
	cout << "¬ведите размер очереди: ";
	while (!(cin >> size))
	{
		system("cls");
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "ќшибка ввода" << std::endl;
		std::cout << "¬ведите размер очереди: ";
	}
	cout << "\n";

	return new SyncQueue(size);
}

HANDLE* createProducers(SyncQueue* queue, producerData* pData, int* size) {

	cout << "¬ведите кол-во потков producer: ";
	int pNum = 0;
	while (!(cin >> pNum))
	{
		system("cls");
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "ќшибка ввода" << std::endl;
		std::cout << "¬ведите кол-во потков producer: ";
	}
	cout << "\n";
	*size = pNum;

	pData = new producerData[pNum];
	HANDLE* producers = new HANDLE[pNum];

	for (int i = 0; i < pNum; i++)
	{
		cout << "¬ведите кол-во чисел дл€ потока producer No." << i + 1 << ": ";
		cin >> pData[i].numberOfCycles;
		cout << "\n";
		pData[i].serialNumber = i + 1;
		pData[i].queue = queue;
		producers[i] = CreateThread(NULL, 0, producer, &pData[i], NULL, NULL);
	}

	return producers;
}

HANDLE* createConsumers(SyncQueue* queue, consumerData* cData, int* size) {

	cout << "¬ведите кол-во потков consumer: ";
	int cNum = 0;
	while (!(cin >> cNum))
	{
		system("cls");
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "ќшибка ввода" << std::endl;
		std::cout << "¬ведите кол-во потков consumer: ";
	}
	cout << "\n";
	*size = cNum;

	cData = new consumerData[cNum];
	HANDLE* consumers = new HANDLE[cNum];

	for (int i = 0; i < cNum; i++)
	{
		cout << "¬ведите кол-во чисел дл€ потока conusmer No." << i + 1 << ": ";
		cin >> cData[i].numberOfCycles;
		cout << "\n";
		cData[i].queue = queue;
		consumers[i] = CreateThread(NULL, 0, consumer, &cData[i], NULL, NULL);
	}

	return consumers;
}

void clear(HANDLE* data) {

	if (data != nullptr) {
		for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
			CloseHandle(data[i]);
		delete data;
	}
}

int main() {
	setlocale(LC_ALL, ".1251");

	start = CreateEvent(NULL, TRUE, FALSE, NULL);

	SyncQueue* queue = createQueue();

	consumerData* cData = nullptr;
	producerData* pData = nullptr;
	int cSize;
	int pSize;

	HANDLE* producers = createProducers(queue, pData, &cSize);
	HANDLE* consumers = createConsumers(queue, cData, &pSize);

	SetEvent(start);

	WaitForMultipleObjects(pSize, producers, TRUE, INFINITE);
	WaitForMultipleObjects(cSize, consumers, TRUE, INFINITE);
	clear(producers);
	clear(consumers);

	delete[] cData;
	delete[] pData;

	return 0;
}