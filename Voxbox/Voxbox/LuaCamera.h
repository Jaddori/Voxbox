#pragma once

#include "BaseIncludes.h"
#include "Camera.h"

namespace LuaCamera
{
	void bind( lua_State* lua, CoreData* coreData );

	int project( lua_State* lua );
	int unproject( lua_State* lua );

	int updatePosition( lua_State* lua );
	int updateDirection( lua_State* lua );
	
	int setPosition( lua_State* lua );
	int setDirection( lua_State* lua );

	int getPosition( lua_State* lua );
	int getDirection( lua_State* lua );
}