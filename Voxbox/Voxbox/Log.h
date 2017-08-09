#pragma once

#include <fstream>
#include "Array.h"
#include <cassert>

#define LOG( verbosity, fmt, ... ) \
	{ \
		char buffer[128] = {}; \
		snprintf( buffer, 128, fmt, __VA_ARGS__ ); \
		Log::instance().addMessage( verbosity, buffer ); \
	}
#define LOG_ASSERT( condition, fmt, ... ) \
	{ \
		if( !(condition) ) \
			LOG( VERBOSITY_ERROR, fmt, __VA_ARGS__ ); \
		assert( (condition) ); \
	}

enum
{
	VERBOSITY_INFORMATION,
	VERBOSITY_WARNING,
	VERBOSITY_ERROR,
};

struct LogMessage
{
	char message[128];
	int verbosity;
};

class Log
{
public:
	static Log& instance()
	{
		static Log log;
		return log;
	}
	~Log(){}
	
	bool start( const char* logfile );
	void stop();

	void addMessage( int verbosity, const char* message );

	void setThreshold( int threshold );

	const Array<LogMessage>& getMessages() const;

private:
	Log() : file( nullptr ), threshold( VERBOSITY_WARNING ) {}

	FILE* file;
	Array<LogMessage> messages;
	int threshold;
};