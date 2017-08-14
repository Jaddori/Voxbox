#pragma once

#include "Input.h"

namespace LuaInput
{
	void bind( lua_State* lua, CoreData* coreData );

	int keyDown( lua_State* lua );
	int keyUp( lua_State* lua );
	int keyPressed( lua_State* lua );
	int keyReleased( lua_State* lua );

	int buttonDown( lua_State* lua );
	int buttonUp( lua_State* lua );
	int buttonPressed( lua_State* lua );
	int buttonReleased( lua_State* lua );

	int mousePosition( lua_State* lua );
}