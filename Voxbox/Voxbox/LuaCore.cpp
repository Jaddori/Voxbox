#include "LuaCore.h"

namespace LuaCore
{
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
}