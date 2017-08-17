#pragma once

#include "BaseIncludes.h"

#define THREAD_POOL_MAX_THREADS 2
#define THREAD_POOL_TIMEOUT 1000

typedef void (Task)();

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	void queueTask( Task* task );
	void schedule();

private:
	struct ThreadData
	{
		int id;
		bool alive, done;
		HANDLE signal;
		Task* task;
	};

	static DWORD WINAPI threadWork( LPVOID args );

	HANDLE threads[THREAD_POOL_MAX_THREADS];
	ThreadData data[THREAD_POOL_MAX_THREADS];

	Queue<Task*> tasks;
	int curTask;
};