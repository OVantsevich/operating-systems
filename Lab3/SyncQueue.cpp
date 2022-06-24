#include "SyncQueue.h"

using namespace std;

SyncQueue::SyncQueue(int nSize) {

	if (0 >= nSize) throw "Ошибка!\nНекорректный размер очереди!";

	array = new int[nSize];
	this->nSize = nSize;
	begin = 0;
	end = 0;
	for (int i = 0; i < this->nSize; i++)
		array[i] = 0;

	removeSemaphore = CreateSemaphore(NULL, 0, nSize, NULL);
	insertSemaphore = CreateSemaphore(NULL, nSize, nSize, NULL);
	InitializeCriticalSection(&cs);
}

SyncQueue::~SyncQueue() {

	DeleteCriticalSection(&cs);
	CloseHandle(insertSemaphore);
	CloseHandle(removeSemaphore);
	if (array != nullptr) {
		delete[] array;
		array = nullptr;
	}
}

void SyncQueue::Insert(int nElement) {

	WaitForSingleObject(insertSemaphore, INFINITE);

	EnterCriticalSection(&cs);

	array[end] = nElement;
	end = (end + 1) % nSize;
	ReleaseSemaphore(removeSemaphore, 1, NULL);

	LeaveCriticalSection(&cs);
}

int SyncQueue::Remove() {

	WaitForSingleObject(removeSemaphore, INFINITE);

	EnterCriticalSection(&cs);
	
	int r = array[begin];
	array[begin] = 0;
	begin = (begin + 1) % nSize;
	ReleaseSemaphore(insertSemaphore, 1, NULL);
	
	LeaveCriticalSection(&cs);

	return r;
}

