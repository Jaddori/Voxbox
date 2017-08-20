#pragma once

#include "BaseIncludes.h"

#define THREAD_POOL_MAX_THREADS 2
#define THREAD_POOL_TIMEOUT 100

typedef void (Task)( void* args );
struct Job
{
	Task* task;
	void* args;
};

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	void load();
	void unload();

	void queueWork( const Job& job );
	void schedule();

private:
	struct ThreadData
	{
		int id;
		bool alive, done;
		HANDLE signal;
		Job job;
	};

	static DWORD WINAPI threadWork( LPVOID args );

	HANDLE threads[THREAD_POOL_MAX_THREADS];
	ThreadData data[THREAD_POOL_MAX_THREADS];

	Queue<Job> jobs;
	int curTask;
};