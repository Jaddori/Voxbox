#include "LuaInput.h"

namespace LuaInput
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		/*lua_register( lua, "keyDown", keyDown );
		lua_register( lua, "keyUp", keyUp );
		lua_register( lua, "keyPressed", keyPressed );
		lua_register( lua, "keyReleased", keyReleased );

		lua_register( lua, "buttonDown", buttonDown );
		lua_register( lua, "buttonUp", buttonUp );
		lua_register( lua, "buttonPressed", buttonPressed );
		lua_register( lua, "buttonReleased", buttonReleased );

		lua_register( lua, "mousePosition", mousePosition );*/

		luaL_newmetatable( lua, "inputMeta" );
		luaL_Reg regs[] =
		{
			{ "keyDown",		keyDown },
			{ "keyUp",			keyUp },
			{ "keyPressed",		keyPressed },
			{ "keyReleased",	keyReleased },
			
			{ "buttonDown",		buttonDown },
			{ "buttonUp",		buttonUp },
			{ "buttonPressed",	buttonPressed },
			{ "buttonReleased", buttonReleased },

			{ "mousePosition",	mousePosition },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, regs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Input" );

		lua_newtable( lua );
		lua_pushnumber( lua, SDL_SCANCODE_GRAVE );
		lua_setfield( lua, -2, "Console" );
		lua_setglobal( lua, "Keys" );

		lua_newtable( lua );
		lua_pushnumber( lua, SDL_BUTTON_LEFT );
		lua_setfield( lua, -2, "Left" );
		lua_pushnumber( lua, SDL_BUTTON_RIGHT );
		lua_setfield( lua, -2, "Right" );
		lua_pushnumber( lua, SDL_BUTTON_MIDDLE );
		lua_setfield( lua, -2, "Middle" );
		lua_setglobal( lua, "Buttons" );

		g_coreData = coreData;
	}

	int keyDown( lua_State* lua )
	{
		LOG_ASSERT( lua_gettop( lua ) == 1, "LuaInput.cpp - Bad arguments to keyDown." );

		int key = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->keyDown( key ) );

		return 1;
	}

	int keyUp( lua_State* lua )
	{
		LOG_ASSERT( lua_gettop( lua ) == 1, "LuaInput.cpp - Bad arguments to keyDown." );

		int key = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->keyUp( key ) );

		return 1;
	}

	int keyPressed( lua_State* lua )
	{
		LOG_ASSERT( lua_gettop( lua ) == 1, "LuaInput.cpp - Bad arguments to keyDown." );

		int key = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->keyPressed( key ) );

		return 1;
	}

	int keyReleased( lua_State* lua )
	{
		LOG_ASSERT( lua_gettop( lua ) == 1, "LuaInput.cpp - Bad arguments to keyDown." );

		int key = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->keyReleased( key ) );

		return 1;
	}

	int buttonDown( lua_State* lua )
	{
		LOG_ASSERT( lua_gettop( lua ) == 1, "LuaInput.cpp - Bad arguments to buttonDown." );

		int button = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->buttonDown( button ) );

		return 1;
	}

	int buttonUp( lua_State* lua )
	{
		LOG_ASSERT( lua_gettop( lua ) == 1, "LuaInput.cpp - Bad arguments to buttonDown." );

		int button = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->buttonDown( button ) );

		return 1;
	}

	int buttonPressed( lua_State* lua )
	{
		LOG_ASSERT( lua_gettop( lua ) == 1, "LuaInput.cpp - Bad arguments to buttonDown." );

		int button = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->buttonDown( button ) );

		return 1;
	}

	int buttonReleased( lua_State* lua )
	{
		LOG_ASSERT( lua_gettop( lua ) == 1, "LuaInput.cpp - Bad arguments to buttonDown." );

		int button = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->buttonDown( button ) );

		return 1;
	}

	int mousePosition( lua_State* lua )
	{
		Point point = g_coreData->input->getMousePosition();

		lua_pushnumber( lua, point.x );
		lua_pushnumber( lua, point.y );

		return 2;
	}
}