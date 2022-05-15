#include "SyncQueue.h"

using namespace std;

SyncQueue::SyncQueue(int nSize) {
	if (nSize >= 0) {
		array = new int[nSize];
		this->nSize = nSize;
		currentPos = 0;
		for (int i = 0; i < this->nSize; i++)
			array[i] = 0;
	}
	else {
		array = nullptr;
		this->nSize = 0;
		currentPos = 0;
	}
	full = CreateSemaphore(NULL, 0, nSize, NULL);
	empty = CreateSemaphore(NULL, nSize, nSize, NULL);
	InitializeCriticalSection(&cs);
}

SyncQueue::~SyncQueue() {
	DeleteCriticalSection(&cs);
	CloseHandle(empty);
	CloseHandle(full);
	if (array != nullptr)
		delete array;
}

void SyncQueue::Insert(int nElement) {
	WaitForSingleObject(empty, INFINITE);

	EnterCriticalSection(&cs);
	if (currentPos < nSize) {
		array[currentPos] = nElement;
		++currentPos;

		ReleaseSemaphore(full, 1, NULL);
	}
	LeaveCriticalSection(&cs);
}

int SyncQueue::Remove() {
	WaitForSingleObject(full, INFINITE);

	EnterCriticalSection(&cs);
	if (currentPos > 0) {
		int r = array[currentPos - 1];
		array[currentPos - 1] = 0;
		--currentPos;

		LeaveCriticalSection(&cs);
		ReleaseSemaphore(empty, 1, NULL);

		return r;
	}

	LeaveCriticalSection(&cs);

	return 0;
}