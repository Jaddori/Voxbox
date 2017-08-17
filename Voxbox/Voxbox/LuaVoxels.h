#pragma once

#include "BaseIncludes.h"
#include "Chunk.h"
#include "Region.h"
#include "World.h"

namespace LuaVoxels
{
	void bind( lua_State* lua, CoreData* coreData );

	// Chunk

	// Region

	// World
	int hitBlock( lua_State* lua );
	int localToWorld( lua_State* lua );
	int worldToLocal( lua_State* lua );
}