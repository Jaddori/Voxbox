#include "LuaInput.h"

namespace LuaInput
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		// metatable for input
		luaL_newmetatable( lua, "inputMeta" );
		luaL_Reg inputRegs[] =
		{
			{ "keyDown",			keyDown },
			{ "keyUp",				keyUp },
			{ "keyPressed",			keyPressed },
			{ "keyReleased",		keyReleased },
			{ "keyRepeated",		keyRepeated },
			
			{ "buttonDown",			buttonDown },
			{ "buttonUp",			buttonUp },
			{ "buttonPressed",		buttonPressed },
			{ "buttonReleased",		buttonReleased },

			{ "getMousePosition",	getMousePosition },
			{ "getMouseDelta",		getMouseDelta },
			{ "getMouseWheel",		getMouseWheel },
			{ "textInput",			textInput },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, inputRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Input" );

		// key constants
		lua_newtable( lua );
		lua_setnumber( lua, "Console", SDL_SCANCODE_GRAVE );
		lua_setnumber( lua, "Enter", SDL_SCANCODE_RETURN );
		lua_setnumber( lua, "Backspace", SDL_SCANCODE_BACKSPACE );
		lua_setnumber( lua, "Space", SDL_SCANCODE_SPACE );

		lua_setnumber( lua, "Up", SDL_SCANCODE_UP );
		lua_setnumber( lua, "Down", SDL_SCANCODE_DOWN );
		lua_setnumber( lua, "Zero", SDL_SCANCODE_0 );
		lua_setnumber( lua, "One", SDL_SCANCODE_1 );
		lua_setnumber( lua, "Two", SDL_SCANCODE_2 );
		lua_setnumber( lua, "Three", SDL_SCANCODE_3 );
		lua_setnumber( lua, "Four", SDL_SCANCODE_4 );
		lua_setnumber( lua, "Five", SDL_SCANCODE_5 );
		lua_setnumber( lua, "Six", SDL_SCANCODE_6 );
		lua_setnumber( lua, "Seven", SDL_SCANCODE_7 );
		lua_setnumber( lua, "Eight", SDL_SCANCODE_8 );
		lua_setnumber( lua, "Nine", SDL_SCANCODE_9 );

		lua_setglobal( lua, "Keys" );

		// button constants
		lua_newtable( lua );
		lua_setnumber( lua, "Left", SDL_BUTTON_LEFT );
		lua_setnumber( lua, "Right", SDL_BUTTON_RIGHT );
		lua_setnumber( lua, "Middle", SDL_BUTTON_MIDDLE );
		lua_setglobal( lua, "Buttons" );

		g_coreData = coreData;
	}

	int keyDown( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 1 );

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			int key = getKey( lua, 1 );
			if( key >= 0 )
			{
				lua_pushboolean( lua, g_coreData->input->keyDown( key ) );
				result = 1;
			}
		}

		return result;
	}

	int keyUp( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 1 );

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			int key = getKey( lua, 1 );
			if( key >= 0 )
			{
				lua_pushboolean( lua, g_coreData->input->keyUp( key ) );
				result = 1;
			}
		}

		return result;
	}

	int keyPressed( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 1 );

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			int key = getKey( lua, 1 );
			if( key >= 0 )
			{
				lua_pushboolean( lua, g_coreData->input->keyPressed( key ) );
				result = 1;
			}
		}

		return result;
	}

	int keyReleased( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 1 );

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			int key = getKey( lua, 1 );
			if( key >= 0 )
			{
				lua_pushboolean( lua, g_coreData->input->keyReleased( key ) );
				result = 1;
			}
		}

		return result;
	}

	int keyRepeated( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 1 );

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			int key = getKey( lua, 1 );
			if( key >= 0 )
			{
				lua_pushboolean( lua, g_coreData->input->keyRepeated( key ) );
				result = 1;
			}
		}

		return result;
	}

	int buttonDown( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_NUMBER( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_NUMBER( 1 ) )
			{
				int button = lua_toint( lua, 1 );
				lua_pushboolean( lua, g_coreData->input->buttonDown( button ) );

				result = 1;
			}
		}

		return result;
	}

	int buttonUp( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_NUMBER( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_NUMBER( 1 ) )
			{
				int button = lua_toint( lua, 1 );
				lua_pushboolean( lua, g_coreData->input->buttonUp( button ) );
				result = 1;
			}
		}

		return result;
	}

	int buttonPressed( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_NUMBER( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_NUMBER( 1 ) )
			{
				int button = lua_toint( lua, 1 );
				lua_pushboolean( lua, g_coreData->input->buttonPressed( button ) );
				result = 1;
			}
		}

		return result;
	}

	int buttonReleased( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_NUMBER( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 ) 
		{
			if( LUA_EXPECT_NUMBER( 1 ) )
			{
				int button = lua_toint( lua, 1 );
				lua_pushboolean( lua, g_coreData->input->buttonReleased( button ) );
				result = 1;
			}
		}

		return result;
	}

	int getMousePosition( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				Point result = g_coreData->input->getMousePosition();

				// set result
				lua_setnumber( lua, 1, 1, result.x );
				lua_setnumber( lua, 1, 2, result.y );
			}
		}

		return 0;
	}

	int getMouseDelta( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				Point result = g_coreData->input->getMouseDelta();

				// set result
				lua_setnumber( lua, 1, 1, result.x );
				lua_setnumber( lua, 1, 2, result.y );
			}
		}

		return 0;
	}

	int getMouseWheel( lua_State* lua )
	{
		lua_pushnumber( lua, g_coreData->input->getMouseWheel() );
		return 1;
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
			const char* str = lua_tostring( lua, index );

			// ASCII A starts at 65
			// SDL_SCANCODE_A starts at 4
			// So we transform from ASCII -> SCANCODE (65 -> 0 -> 4)
			key = (int)str[0] - 'A' + SDL_SCANCODE_A;
		}
		else
		{
			//LOG_ASSERT( lua_tonumber( lua, 1 ) || lua_tostring( lua, index ), "Bad argument #%d type.", index );
			LOG_ERROR( "Bad argument #%d type.", index );
		}

		return key;
	}
}