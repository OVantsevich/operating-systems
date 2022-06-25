#include <iostream>
#include <windows.h>
#include <ctime>
#include "SyncQueue.h"
#include <string>
#include "correctInput.h"

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
	correctInput<int>("¬ведите размер очереди: ", size);
	while (0 >= size) {
		system("cls");
		cout << " екорректный размер очереди!!!" << endl;
		correctInput<int>("¬ведите размер очереди: ", size);
	}

	return new SyncQueue(size);
}

HANDLE* createProducers(SyncQueue* queue, producerData* pData, int* size) {

	int pNum = 0;
	correctInput<int>("¬ведите кол-во потков producer: ", pNum);
	while (0 >= pNum) {
		system("cls");
		cout << " екорректное кол-во потков producer!!!" << endl;
		correctInput<int>("¬ведите кол-во потков producer: ", pNum);
	}
	*size = pNum;
	
	pData = new producerData[pNum];
	HANDLE* producers = new HANDLE[pNum];

	for (int i = 0; i < pNum; i++)
	{
		std::string message("¬ведите кол-во чисел дл€ потока producer No.");
		message += std::to_string(i + 1);
		message += ": ";
		correctInput<int>(message.c_str(), pData[i].numberOfCycles);
		while (0 >= pData[i].numberOfCycles) {
			system("cls");
			cout << " екорректное кол-во чисел дл€ потока!!!" << endl;
			correctInput<int>(message.c_str(), pData[i].numberOfCycles);
		}

		pData[i].serialNumber = i + 1;
		pData[i].queue = queue;
		producers[i] = CreateThread(NULL, 0, producer, &pData[i], NULL, NULL);
	}

	return producers;
}

HANDLE* createConsumers(SyncQueue* queue, consumerData* cData, int* size) {

	int cNum = 0;
	correctInput<int>("¬ведите кол-во потков consumer: ", cNum);
	while (0 >= cNum) {
		system("cls");
		cout << " екорректное кол-во потков consumer!!!" << endl;
		correctInput<int>("¬ведите кол-во потков consumer: ", cNum);
	}
	*size = cNum;

	cData = new consumerData[cNum];
	HANDLE* consumers = new HANDLE[cNum];

	for (int i = 0; i < cNum; i++)
	{
		std::string message("¬ведите кол-во чисел дл€ потока conusmer No.");
		message += std::to_string(i + 1);
		message += ": ";
		correctInput<int>(message.c_str(), cData[i].numberOfCycles);
		while (0 >= cData[i].numberOfCycles) {
			system("cls");
			cout << " екорректное кол-во чисел дл€ потока!!!" << endl;
			correctInput<int>(message.c_str(), cData[i].numberOfCycles);
		}

		cData[i].queue = queue;
		consumers[i] = CreateThread(NULL, 0, consumer, &cData[i], NULL, NULL);
	}

	return consumers;
}

void clear(HANDLE* data, int dataSize) {

	if (data != nullptr) {
		for (int i = 0; i < dataSize; i++)
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
	clear(producers, pSize);
	clear(consumers, cSize);

	delete[] cData;
	delete[] pData;

	return 0;
}