#include "LuaDebug.h"

namespace LuaDebug
{
	static Array<LogMessage> g_logMessages;

	void bind( lua_State* lua, CoreData* coreData )
	{
		lua_register( lua, "log", log );

		lua_pushnumber( lua, VERBOSITY_INFORMATION );
		lua_setglobal( lua, "VERBOSITY_INFORMATION" );
		lua_pushnumber( lua, VERBOSITY_WARNING );
		lua_setglobal( lua, "VERBOSITY_WARNING" );
		lua_pushnumber( lua, VERBOSITY_ERROR );
		lua_setglobal( lua, "VERBOSITY_ERROR" );
		lua_pushnumber( lua, VERBOSITY_DEBUG );
		lua_setglobal( lua, "VERBOSITY_DEBUG" );

		luaL_newmetatable( lua, "logMeta" );
		luaL_Reg regs[] =
		{
			{ "log",			log },
			{ "getMessages",	getLogMessages },
			{ "getThreshold",	getLogThreshold },
			{ NULL,	NULL }
		};

		luaL_setfuncs( lua, regs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );

		lua_setglobal( lua, "Log" );
	}

	int log( lua_State* lua )
	{
		assert( lua_gettop( lua ) == 2 );
		assert( lua_isnumber( lua, 1 ) );
		assert( lua_isstring( lua, 2 ) );

		int verbosity = (int)lua_tonumber( lua, 1 );
		const char* str = lua_tostring( lua, 2 );

		LOG( verbosity, "%s", str );

		return 0;
	}

	int getLogMessages( lua_State* lua )
	{
		Log::instance().copyMessages( g_logMessages );

		lua_newtable( lua ); // message table
		lua_newtable( lua ); // verbosity table

		const int MESSAGE_COUNT = g_logMessages.getSize();
		for( int i=0; i<MESSAGE_COUNT; i++ )
		{
			lua_pushstring( lua, g_logMessages[i].message );
			lua_rawseti( lua, -3, i );

			lua_pushnumber( lua, g_logMessages[i].verbosity );
			lua_rawseti( lua, -2, i );
		}

		return 2;
	}

	int getLogThreshold( lua_State* lua )
	{
		lua_pushnumber( lua, Log::instance().getThreshold() );
		return 1;
	}
}