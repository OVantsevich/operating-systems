#pragma once
#include <Windows.h>
class SyncQueue {
public:
	SyncQueue(int nSize);
	~SyncQueue();
	void Insert(int nElement);
	int Remove();
private:
	int* array;
	int currentPos;
	int nSize;
	HANDLE insertSemaphore;
	HANDLE removeSemaphore;
	CRITICAL_SECTION cs;
};
 