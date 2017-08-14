#include "LuaBinds.h"

LuaBinds::LuaBinds()
	: lua( nullptr ),
	loadFunctionReference( -1 ),
	unloadFunctionReference( -1 ),
	updateFunctionReference( -1 )
{
	lua = luaL_newstate();
	luaL_openlibs( lua );
}

LuaBinds::~LuaBinds()
{
	lua_close( lua );
}

bool LuaBinds::bind( CoreData* coreData )
{
	valid = true;

	lua_pushnumber( lua, WINDOW_WIDTH );
	lua_setglobal( lua, "WINDOW_WIDTH" );
	lua_pushnumber( lua, WINDOW_HEIGHT );
	lua_setglobal( lua, "WINDOW_HEIGHT" );

	LuaAssets::bind( lua, coreData );
	LuaDebug::bind( lua, coreData );
	LuaInput::bind( lua, coreData );
	LuaRendering::bind( lua, coreData );

	// load main script
	LOG_INFO( "Loading main script." );
	if( luaL_loadfile( lua, LUA_MAIN_SCRIPT ) != 0 )
	{
		LOG_ERROR( "Failed to load main script." );
		LOG_ERROR( "%s", lua_tostring( lua, -1 ) );
		valid = false;
	}
	else
	{
		// run main script
		LOG_INFO( "Running main script." );
		if( lua_pcall( lua, 0, 0, 0 ) != 0 )
		{
			LOG_ERROR( "Failed to run main script." );
			LOG_ERROR( "%s", lua_tostring( lua, -1 ) );
			valid = false;
		}
		else
		{
			// get load function
			lua_getglobal( lua, "mainLoad" );
			if( !lua_isfunction( lua, -1 ) )
			{
				LOG_ERROR( "Failed to find load function." );
				valid = false;
			}
			else
				loadFunctionReference = luaL_ref( lua, LUA_REGISTRYINDEX );

			// get unload function
			lua_getglobal( lua, "mainUnload" );
			if( !lua_isfunction( lua, -1 ) )
			{
				LOG_ERROR( "Failed to find unload function." );
				valid = false;
			}
			else
				unloadFunctionReference = luaL_ref( lua, LUA_REGISTRYINDEX );

			// get update function
			lua_getglobal( lua, "mainUpdate" );
			if( !lua_isfunction( lua, -1 ) )
			{
				LOG_ERROR( "Failed to find update function." );
				valid = false;
			}
			else
				updateFunctionReference = luaL_ref( lua, LUA_REGISTRYINDEX );

			// get render function
			lua_getglobal( lua, "mainRender" );
			if( !lua_isfunction( lua, -1 ) )
			{
				LOG_ERROR( "Failed to find render function." );
				valid = false;
			}
			else
				renderFunctionReference = luaL_ref( lua, LUA_REGISTRYINDEX );
		}
	}

	return valid;
}

bool LuaBinds::getValid() const
{
	return valid;
}

void LuaBinds::run( int functionReference )
{
	LOG_ASSERT( valid, "Trying to run function with invalid state." );

	lua_rawgeti( lua, LUA_REGISTRYINDEX, functionReference );
	if( lua_pcall( lua, 0, 0, 0 ) != 0 )
	{
		LOG_ERROR( "Failed to run function reference." );
		LOG_ERROR( "%s", lua_tostring( lua, -1 ) );
		valid = false;
	}
}