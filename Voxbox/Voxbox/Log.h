#pragma once

#include <fstream>
#include "Array.h"
#include <cassert>
#include <Windows.h>

#define LOG_START( file ) Log::instance().start( file )
#define LOG_STOP() Log::instance().stop()
#define LOG_FINALIZE() Log::instance().finalize()
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
#define LOG_INFORMATIONS() LOG_THRESHOLD( VERBOSITY_INFORMATION )
#define LOG_WARNINGS() LOG_THRESHOLD( VERBOSITY_WARNING )
#define LOG_ERRORS() LOG_THRESHOLD( VERBOSITY_ERROR )

#define LOG_INFO( fmt, ... ) LOG( VERBOSITY_INFORMATION, fmt, __VA_ARGS__ )
#define LOG_WARNING( fmt, ... ) LOG( VERBOSITY_WARNING, fmt, __VA_ARGS__ )
#define LOG_ERROR( fmt, ... ) LOG( VERBOSITY_ERROR, fmt, __VA_ARGS__ )
#define LOG_DEBUG( fmt, ... ) LOG( VERBOSITY_DEBUG, fmt, __VA_ARGS__ )

#define LOG_ASSERT_ARGS( from, expected ) \
	{ \
		int args = lua_gettop( lua ); \
		LOG_ASSERT( args == expected, from " - Expected " #expected " argument(s) to " __FUNCTION__ ", got %d", args ); \
	}

#define LOG_EXPECT_BOOL( from, num ) LOG_ASSERT( lua_isboolean( lua, num ), from " - Expected bool argument to " __FUNCTION__ )
#define LOG_EXPECT_NUMBER( from, num ) LOG_ASSERT( lua_isnumber( lua, num ), from " - Expected number argument to " __FUNCTION__ )
#define LOG_EXPECT_STRING( from, num ) LOG_ASSERT( lua_isstring( lua, num ), from " - Expected string argument to " __FUNCTION__ )
#define LOG_EXPECT_USERDATA( from, num ) LOG_ASSERT( lua_isuserdata( lua, num ), from " - Expected userdata argument to " __FUNCTION__ )
#define LOG_EXPECT_TABLE( from, num ) LOG_ASSERT( lua_istable( lua, num ), from " - Expected table argument #" #num " to " __FUNCTION__ )

enum
{
	VERBOSITY_INFORMATION,
	VERBOSITY_WARNING,
	VERBOSITY_ERROR,
	VERBOSITY_DEBUG
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
	void copyMessages( Array<LogMessage>& destination );

	void setThreshold( int threshold );

	const Array<LogMessage>& getMessages() const;
	int getThreshold() const;

private:
	Log() : file( nullptr ), threshold( VERBOSITY_WARNING ) {}

	FILE* file;
	Array<LogMessage> messages;
	int threshold;

	HANDLE mutex;
};