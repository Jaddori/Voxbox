#include "Log.h"

bool Log::start( const char* logfile )
{
#if _DEBUG
	file = fopen( logfile, "w" );
	if( file )
	{
		addMessage( VERBOSITY_INFORMATION, "*** Log started ***" );
	}

	mutex = CreateMutex( NULL, FALSE, NULL );
	
	return ( file != nullptr );
#else
	return true;
#endif
}

void Log::stop()
{
#if _DEBUG
	assert( file );

	addMessage( VERBOSITY_INFORMATION, "*** Log stopped ***" );
	CloseHandle( mutex );
	fclose( file );
#endif
}

void Log::addMessage( int verbosity, const char* message )
{
#if _DEBUG
	assert( file );

	WaitForSingleObject( mutex, INFINITE );

	fprintf( file, "%s\n", message );
	fflush( file );
	if( verbosity >= threshold )
		printf( "%s\n", message );

	LogMessage msg;
	strcpy( msg.message, message );
	msg.verbosity = verbosity;

	messages.add( msg );

	ReleaseMutex( mutex );
#endif
}

void Log::copyMessages( Array<LogMessage>& destination )
{
	SDL_LockMutex( mutex );
	destination.fastCopy( messages );
	SDL_UnlockMutex( mutex );
}

void Log::setThreshold( int t )
{
	threshold = t;
}

const Array<LogMessage>& Log::getMessages() const
{
	return messages;
}

int Log::getThreshold() const
{
	return threshold;
}
