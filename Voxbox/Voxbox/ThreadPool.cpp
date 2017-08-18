#include "ThreadPool.h"

ThreadPool::ThreadPool()
	: curTask( 0 )
{
	LOG_INFO( "Constructing." );
}

ThreadPool::~ThreadPool()
{
	LOG_INFO( "Destructing." );
}

void ThreadPool::load()
{
	LOG_INFO( "Creating threads." );

	for( int i=0; i<THREAD_POOL_MAX_THREADS; i++ )
	{
		data[i].id = i;
		data[i].alive = true;
		data[i].done = true;
		data[i].signal = CreateSemaphore( NULL, 0, 1, NULL );
		threads[i] = CreateThread( NULL, 0, threadWork, &data[i], 0, NULL );
	}
}

void ThreadPool::unload()
{
	LOG_INFO( "Waiting for threads to finish." );

	for( int i=0; i<THREAD_POOL_MAX_THREADS; i++ )
		data[i].alive = false;

	WaitForMultipleObjects( THREAD_POOL_MAX_THREADS, threads, TRUE, INFINITE );

	for( int i=0; i<THREAD_POOL_MAX_THREADS; i++ )
		CloseHandle( threads[i] );
}

void ThreadPool::queueWork( const Job& job)
{
	LOG_ASSERT( job.task != nullptr, "Job task is nullptr." );

	jobs.enqueue( job );
}

void ThreadPool::schedule()
{
	for( int i=0; i<THREAD_POOL_MAX_THREADS && jobs.getSize() > 0; i++ )
	{
		if( data[i].done )
		{
			data[i].done = false;
			data[i].job = jobs.dequeue();
				
			ReleaseSemaphore( data[i].signal, 1, NULL );
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
			LOG_INFO( "Thread #%d executing task.", data->id );

			data->job.task( data->job.args );

			data->done = true;
		}
	}

	LOG_INFO( "Thread #%d exiting task loop.", data->id );

	return 0;
}