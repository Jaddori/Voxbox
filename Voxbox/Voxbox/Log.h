#pragma once

#include <fstream>
#include "Array.h"
#include <cassert>

#define LOG_START( file ) Log::instance().start( file )
#define LOG_STOP() Log::instance().stop()
#define LOG( verbosity, fmt, ... ) \
	{ \
		char buffer[1024] = {}; \
		snprintf( buffer, 1024, fmt, __VA_ARGS__ ); \
		Log::instance().addMessage( verbosity, buffer ); \
	}
#define LOG_ASSERT( condition, fmt, ... ) \
	{ \
		if( !(condition) ) \
			LOG( VERBOSITY_ERROR, fmt, __VA_ARGS__ ); \
		assert( (condition) ); \
	}
#define LOG_THRESHOLD( threshold ) Log::instance().setThreshold( threshold )
#define LOG_INFORMATION() LOG_THRESHOLD( VERBOSITY_INFORMATION )
#define LOG_WARNINGS() LOG_THRESHOLD( VERBOSITY_WARNING )
#define LOG_ERRORS() LOG_THRESHOLD( VERBOSITY_ERROR )

enum
{
	VERBOSITY_INFORMATION,
	VERBOSITY_WARNING,
	VERBOSITY_ERROR,
};

struct LogMessage
{
	char message[1024];
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
	int getThreshold() const;

private:
	Log() : file( nullptr ), threshold( VERBOSITY_WARNING ) {}

	FILE* file;
	Array<LogMessage> messages;
	int threshold;
};