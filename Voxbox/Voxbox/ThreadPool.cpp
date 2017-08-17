#include "ThreadPool.h"

ThreadPool::ThreadPool()
	: curTask( 0 )
{
	LOG_INFO( "Constructing." );
	LOG_INFO( "Creating threads." );

	for( int i=0; i<THREAD_POOL_MAX_THREADS; i++ )
	{
		data[i].id = i;
		data[i].alive = true;
		data[i].done = true;
		data[i].signal = CreateSemaphore( NULL, 0, 1, NULL );
		data[i].task = nullptr;
		threads[i] = CreateThread( NULL, 0, threadWork, &data[i], 0, NULL );
	}
}

ThreadPool::~ThreadPool()
{
	LOG_INFO( "Waiting for threads to finish." );

	for( int i=0; i<THREAD_POOL_MAX_THREADS; i++ )
	{
		data[i].alive = false;
		WaitForSingleObject( threads[i], INFINITE );
		CloseHandle( threads[i] );
	}

	LOG_INFO( "Destructing." );
}

void ThreadPool::queueTask( Task* task )
{
	LOG_ASSERT( task != nullptr, "Task's function is nullptr." );

	tasks.enqueue( task );
}

void ThreadPool::schedule()
{
	for( int i=0; i<THREAD_POOL_MAX_THREADS && tasks.getSize() > 0; i++ )
	{
		if( data[i].done )
		{
			data[i].task = tasks.dequeue();
				
			ReleaseSemaphore( data[i].signal, 1, NULL );
			break;
		}
	}
}

DWORD WINAPI ThreadPool::threadWork( LPVOID args )
{
	ThreadData* data = (ThreadData*)args;

	LOG_INFO( "Thread #%d entering task loop.", data->id );

	while( data->alive )
	{
		// wait for something to do
		DWORD waitResult = WaitForSingleObject( data->signal, THREAD_POOL_TIMEOUT );
		if( waitResult == WAIT_OBJECT_0 )
		{
			LOG_DEBUG( "Thread #%d executing task.", data->id );
			data->done = false;

			data->task();

			data->done = true;
		}
	}

	LOG_INFO( "Thread #%d exiting task loop.", data->id );

	return 0;
}