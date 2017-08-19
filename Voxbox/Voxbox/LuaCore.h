#pragma once

#include "BaseIncludes.h"

namespace LuaCore
{
	void bind( lua_State* lua, CoreData* coreData );

	// vec2
	namespace vec2
	{
		int create( lua_State* lua );

		int add( lua_State* lua );
		int sub( lua_State* lua );
		int mul( lua_State* lua );
		int div( lua_State* lua );
		int neg( lua_State* lua );
		int eq( lua_State* lua );

		int length( lua_State* lua );
		int normalize( lua_State* lua );
		int distance( lua_State* lua );
		int direction( lua_State* lua );
		int print( lua_State* lua );
		int tostring( lua_State* lua );
		int copy( lua_State* lua );
	}

	// vec3
	namespace vec3
	{
		int create( lua_State* lua );

		int add( lua_State* lua );
		int sub( lua_State* lua );
		int mul( lua_State* lua );
		int div( lua_State* lua );
		int neg( lua_State* lua );
		int eq( lua_State* lua );

		int length( lua_State* lua );
		int normalize( lua_State* lua );
		int distance( lua_State* lua );
		int direction( lua_State* lua );
		int print( lua_State* lua );
		int tostring( lua_State* lua );
		int copy( lua_State* lua );
	}

	// vec4
	namespace vec4
	{
		int create( lua_State* lua );

		int add( lua_State* lua );
		int sub( lua_State* lua );
		int mul( lua_State* lua );
		int div( lua_State* lua );
		int neg( lua_State* lua );
		int eq( lua_State* lua );

		int length( lua_State* lua );
		int normalize( lua_State* lua );
		int distance( lua_State* lua );
		int direction( lua_State* lua );
		int print( lua_State* lua );
		int tostring( lua_State* lua );
		int copy( lua_State* lua );
	}
}