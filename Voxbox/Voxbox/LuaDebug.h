#pragma once

#include "BaseIncludes.h"
#include "DebugShapes.h"

namespace LuaDebug
{
	void bind( lua_State* lua, CoreData* coreData );

	// log
	int log( lua_State* lua );

	int setThreshold( lua_State* lua );

	int getLogMessages( lua_State* lua );
	int getLogThreshold( lua_State* lua );

	// debug shapes
	int addLine( lua_State* lua );
	int addSphere( lua_State* lua );
	int addAABB( lua_State* lua );
	int addOBB( lua_State* lua );
};