#pragma once

#include "BaseIncludes.h"
#include "Camera.h"

namespace LuaCamera
{
	void bind( lua_State* lua, CoreData* coreData );

	int project( lua_State* lua );
	int unproject( lua_State* lua );

	int relativeMovement( lua_State* lua );
	int absoluteMovement( lua_State* lua );
	int updateDirection( lua_State* lua );
	
	int setPosition( lua_State* lua );
	int setDirection( lua_State* lua );
	int setHorizontalAngle( lua_State* lua );
	int setVerticalAngle( lua_State* lua );

	int getPosition( lua_State* lua );
	int getDirection( lua_State* lua );
	int getForward( lua_State* lua );
	int getRight( lua_State* lua );
	int getUp( lua_State* lua );
}