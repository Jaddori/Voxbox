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

	// load main script
	if( luaL_loadfile( lua, LUA_MAIN_SCRIPT ) != 0 )
	{
		LOG( VERBOSITY_ERROR, "LuaBinds.cpp - Failed to load main script." );
		LOG( VERBOSITY_ERROR, "%s", lua_tostring( lua, -1 ) );
		valid = false;
	}
	else
	{
		// run main script
		if( lua_pcall( lua, 0, 0, 0 ) != 0 )
		{
			LOG( VERBOSITY_ERROR, "LuaBinds.cpp - Failed to run main script." );
			LOG( VERBOSITY_ERROR, "%s", lua_tostring( lua, -1 ) );
			valid = false;
		}
		else
		{
			// get load function
			lua_getglobal( lua, "load" );
			if( !lua_isfunction( lua, -1 ) )
			{
				LOG( VERBOSITY_ERROR, "LuaBinds.cpp - Failed to find load function." );
				valid = false;
			}
			else
				loadFunctionReference = luaL_ref( lua, LUA_REGISTRYINDEX );

			// get unload function
			lua_getglobal( lua, "unload" );
			if( !lua_isfunction( lua, -1 ) )
			{
				LOG( VERBOSITY_ERROR, "LuaBinds.cpp - Failed to find unload function." );
				valid = false;
			}
			else
				unloadFunctionReference = luaL_ref( lua, LUA_REGISTRYINDEX );

			// get update function
			lua_getglobal( lua, "update" );
			if( !lua_isfunction( lua, -1 ) )
			{
				LOG( VERBOSITY_ERROR, "LuaBinds.cpp - Failed to find update function." );
				valid = false;
			}
			else
				updateFunctionReference = luaL_ref( lua, LUA_REGISTRYINDEX );
		}
	}

	return valid;
}

void LuaBinds::load()
{
	if( valid )
	{
		lua_rawgeti( lua, LUA_REGISTRYINDEX, loadFunctionReference );
		if( lua_pcall( lua, 0, 0, 0 ) != 0 )
		{
			LOG( VERBOSITY_ERROR, "LuaBinds.cpp - Failed to run load function." );
			LOG( VERBOSITY_ERROR, "%s", lua_tostring( lua, -1 ) );
			valid = false;
		}
	}
	else
		LOG( VERBOSITY_WARNING, "LuaBinds.cpp - Trying to call load function with invalid state." );
}

void LuaBinds::unload()
{
	if( valid )
	{
		lua_rawgeti( lua, LUA_REGISTRYINDEX, unloadFunctionReference );
		if( lua_pcall( lua, 0, 0, 0 ) != 0 )
		{
			LOG( VERBOSITY_ERROR, "LuaBinds.cpp - Failed to run unload function." );
			LOG( VERBOSITY_ERROR, "%s", lua_tostring( lua, -1 ) );
			valid = false;
		}
	}
	else
		LOG( VERBOSITY_WARNING, "LuaBinds.cpp - Trying to call unload function with invalid state." );
}

void LuaBinds::update()
{
	if( valid )
	{
		lua_rawgeti( lua, LUA_REGISTRYINDEX, updateFunctionReference );
		if( lua_pcall( lua, 0, 0, 0 ) != 0 )
		{
			LOG( VERBOSITY_ERROR, "LuaBinds.cpp - Failed to run update function." );
			LOG( VERBOSITY_ERROR, "%s", lua_tostring( lua, -1 ) );
			valid = false;
		}
	}
	else
		LOG( VERBOSITY_WARNING, "LuaBinds.cpp - Trying to call update function with invalid state." );
}

bool LuaBinds::getValid() const
{
	return valid;
}