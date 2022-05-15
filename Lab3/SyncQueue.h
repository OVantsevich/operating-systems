#pragma once
#include <Windows.h>
class SyncQueue {
public:
	SyncQueue(int nSize);
	~SyncQueue();
	void Insert(int nElement);
	int Remove();
private:
	bool close;
	int* array;
	int currentPos;
	int nSize;
	HANDLE full;
	HANDLE empty;
	CRITICAL_SECTION cs;
};
