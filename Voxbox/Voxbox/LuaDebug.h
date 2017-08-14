#pragma once

#include "BaseIncludes.h"
#include "DebugShapes.h"

namespace LuaDebug
{
	void bind( lua_State* lua, CoreData* coreData );

	int log( lua_State* lua );

	int getLogMessages( lua_State* lua );
	int getLogThreshold( lua_State* lua );
};