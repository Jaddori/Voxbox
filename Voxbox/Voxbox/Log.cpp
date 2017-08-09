#include "Log.h"

bool Log::start( const char* logfile )
{
#if _DEBUG
	file = fopen( logfile, "w" );
	if( file )
	{
		addMessage( VERBOSITY_INFORMATION, "*** Log started ***" );
	}
	
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
	fclose( file );
#endif
}

void Log::addMessage( int verbosity, const char* message )
{
#if _DEBUG
	assert( file );

	fprintf( file, "%s\n", message );
	if( verbosity >= threshold )
		printf( "%s\n", message );

	LogMessage msg;
	strcpy( msg.message, message );
	msg.verbosity = verbosity;

	messages.add( msg );
#endif
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