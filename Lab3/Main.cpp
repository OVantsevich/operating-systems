#include <iostream>
#include <windows.h>
#include <ctime>
#include "SyncQueue.h"

using namespace std;

HANDLE start;

struct producerData {
	int serialNumber;
	int data;
	SyncQueue* queue;
};

struct consumerData {
	int data;
	SyncQueue* queue;
};

DWORD WINAPI producer(LPVOID data)
{
	WaitForSingleObject(start, INFINITE);

	producerData* pData = (producerData*)data;

	for (int i = 0; i < pData->data; i++)
	{
		pData->queue->Insert(pData->serialNumber + 1);
		cout << "Произведено число: " << pData->serialNumber + 1 << endl;
		Sleep(7);
	}
	return 0;
}

DWORD WINAPI consumer(LPVOID data)
{
	WaitForSingleObject(start, INFINITE);

	consumerData* cData = (consumerData*)data;

	for (int i = 0; i < cData->data; i++)
	{
		int x = cData->queue->Remove();
		cout << "Употреблено число " << x << endl;
	}
	return 0;
}

SyncQueue* inputQueue() {
	int size = 0;
	cout << "Введите размер очереди: ";
	cin >> size;
	cout << "\n";

	return new SyncQueue(size);
}

int main() {
	setlocale(LC_ALL, ".1251");

	start = CreateEvent(NULL, TRUE, FALSE, NULL);

	SyncQueue* queue = inputQueue();


	cout << "Введите кол-во потков producer: ";
	int pNum = 0;
	cin >> pNum;
	cout << "\n";

	producerData* pData = new producerData[pNum];
	HANDLE* producers = new HANDLE[pNum];
	for (int i = 0; i < pNum; i++)
	{
		cout << "Введите кол-во чисел для потока producer No." << i + 1 << ": ";
		cin >> pData[i].data;
		cout << "\n";
		pData[i].serialNumber = i;
		pData[i].queue = queue;
		producers[i] = CreateThread(NULL, 0, producer, (void*)&pData[i], NULL, NULL);
	}


	cout << "Введите кол-во потков consumer: ";
	int cNum = 0;
	cin >> cNum;
	cout << "\n";

	consumerData* cData = new consumerData[cNum];
	HANDLE* consumers = new HANDLE[cNum];
	for (int i = 0; i < cNum; i++)
	{
		cout << "Введите кол-во чисел для потока conusmer No." << i + 1 << ": ";
		cin >> cData[i].data;
		cout << "\n";
		cData[i].queue = queue;
		consumers[i] = CreateThread(NULL, 0, consumer, (void*)&cData[i], NULL, NULL);
	}


	SetEvent(start);

	WaitForMultipleObjects(pNum, producers, TRUE, INFINITE);
	WaitForMultipleObjects(cNum, consumers, TRUE, INFINITE);
	return 0;
}