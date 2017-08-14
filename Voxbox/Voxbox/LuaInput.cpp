#include "LuaInput.h"

namespace LuaInput
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		luaL_newmetatable( lua, "inputMeta" );
		luaL_Reg regs[] =
		{
			{ "keyDown",		keyDown },
			{ "keyUp",			keyUp },
			{ "keyPressed",		keyPressed },
			{ "keyReleased",	keyReleased },
			{ "keyRepeated",	keyRepeated },
			
			{ "buttonDown",		buttonDown },
			{ "buttonUp",		buttonUp },
			{ "buttonPressed",	buttonPressed },
			{ "buttonReleased", buttonReleased },

			{ "mousePosition",	mousePosition },
			{ "textInput",		textInput },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, regs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Input" );

		lua_newtable( lua );
		lua_pushnumber( lua, SDL_SCANCODE_GRAVE );
		lua_setfield( lua, -2, "Console" );
		lua_pushnumber( lua, SDL_SCANCODE_RETURN );
		lua_setfield( lua, -2, "Enter" );
		lua_pushnumber( lua, SDL_SCANCODE_BACKSPACE );
		lua_setfield( lua, -2, "Backspace" );
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
		LOG_ASSERT_ARGS( 1 );

		int key = getKey( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->keyDown( key ) );

		return 1;
	}

	int keyUp( lua_State* lua )
	{
		LOG_ASSERT_ARGS( 1 );

		int key = getKey( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->keyUp( key ) );

		return 1;
	}

	int keyPressed( lua_State* lua )
	{
		LOG_ASSERT_ARGS( 1 );

		int key = getKey( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->keyPressed( key ) );

		return 1;
	}

	int keyReleased( lua_State* lua )
	{
		LOG_ASSERT_ARGS( 1 );

		int key = getKey( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->keyReleased( key ) );

		return 1;
	}

	int keyRepeated( lua_State* lua )
	{
		LOG_ASSERT_ARGS( 1 );

		int key = getKey( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->keyRepeated( key ) );

		return 1;
	}

	int buttonDown( lua_State* lua )
	{
		LOG_ASSERT_ARGS( 1 );
		LOG_EXPECT_NUMBER( 1 );

		int button = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->buttonDown( button ) );

		return 1;
	}

	int buttonUp( lua_State* lua )
	{
		LOG_ASSERT_ARGS( 1 );
		LOG_EXPECT_NUMBER( 1 );

		int button = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->buttonDown( button ) );

		return 1;
	}

	int buttonPressed( lua_State* lua )
	{
		LOG_ASSERT_ARGS( 1 );
		LOG_EXPECT_NUMBER( 1 );

		int button = (int)lua_tonumber( lua, 1 );
		lua_pushboolean( lua, g_coreData->input->buttonDown( button ) );

		return 1;
	}

	int buttonReleased( lua_State* lua )
	{
		LOG_ASSERT_ARGS( 1 );
		LOG_EXPECT_NUMBER( 1 );

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

	int textInput( lua_State* lua )
	{
		lua_pushstring( lua, g_coreData->input->getTextInput() );
		return 1;
	}

	int getKey( lua_State* lua, int index )
	{
		int key = -1;

		if( lua_isnumber( lua, index ) )
		{
			key = (int)lua_tonumber( lua, index );
		}
		else if( lua_isstring( lua, index ) )
		{
			key = (int)( lua_tostring( lua, index )[0] );
		}
		else
		{
			LOG_ASSERT( lua_tonumber( lua, 1 ) || lua_tostring( lua, index ), "Bad argument #%d type.", index );
		}

		return key;
	}
}