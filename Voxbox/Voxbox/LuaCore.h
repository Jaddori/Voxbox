#pragma once

#include "BaseIncludes.h"
#include "CoreData.h"

namespace LuaCore
{
	void bind( lua_State* lua, CoreData* coreData );

	int log( lua_State* lua );
};