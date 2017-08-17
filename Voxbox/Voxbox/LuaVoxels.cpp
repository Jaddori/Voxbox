#include "LuaVoxels.h"

namespace LuaVoxels
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		// metatable for chunk

		// metatable for region

		// metatable for world
		luaL_newmetatable( lua, "worldMeta" );
		luaL_Reg worldRegs[] =
		{
			{ "hitBlock",		hitBlock },
			{ "localToWorld",	localToWorld },
			{ "worldToLocal",	worldToLocal },
			{ "loadWorld",		loadWorld },
			{ "saveWorld",		saveWorld },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, worldRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "World" );

		g_coreData = coreData;
	}

	int hitBlock( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get ray start
		glm::vec3 rayStart;
		lua_getvec3( lua, 1, rayStart );

		// get ray end
		glm::vec3 rayEnd;
		lua_getvec3( lua, 2, rayEnd );

		// set result
		RegionIndex regionIndex;
		float hitLength = g_coreData->world->hitBlock( rayStart, rayEnd, regionIndex );

		if( hitLength > 0.0f )
		{
			lua_setnumber( lua, 3, 1, regionIndex.chunkIndex.block.x );
			lua_setnumber( lua, 3, 2, regionIndex.chunkIndex.block.y );
			lua_setnumber( lua, 3, 3, regionIndex.chunkIndex.block.z );

			lua_setnumber( lua, 3, "chunk", regionIndex.chunkIndex.chunk );
			lua_setnumber( lua, 3, "region", regionIndex.region );
			lua_setnumber( lua, 3, "length", hitLength );
		}

		lua_pushboolean( lua, ( hitLength > 0.0f ) );
		return 1;
	}

	int localToWorld( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get region index
		RegionIndex regionIndex;

		regionIndex.chunkIndex.block.x = lua_getint( lua, 1, 1 );
		regionIndex.chunkIndex.block.y = lua_getint( lua, 1, 2 );
		regionIndex.chunkIndex.block.z = lua_getint( lua, 1, 3 );

		lua_getfield( lua, 1, "chunk" );
		regionIndex.chunkIndex.chunk = lua_toint( lua, -1 );

		lua_getfield( lua, 1, "region" );
		regionIndex.region = lua_toint( lua, -1 );

		// set result
		BlockIndex blockIndex;

		// TEMP: Should this be a stand alone function in Utils.h?
		g_coreData->world->localToWorld( regionIndex, blockIndex );

		lua_setnumber( lua, 2, 1, blockIndex.x );
		lua_setnumber( lua, 2, 2, blockIndex.y );
		lua_setnumber( lua, 2, 3, blockIndex.z );

		return 0;
	}

	int worldToLocal( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get block index
		BlockIndex blockIndex;
		
		blockIndex.x = lua_getint( lua, 1, 1 );
		blockIndex.y = lua_getint( lua, 1, 2 );
		blockIndex.z = lua_getint( lua, 1, 3 );

		// set result
		RegionIndex regionIndex;

		// TEMP: Should this be a stand alone function in Utils.h?
		g_coreData->world->worldToLocal( blockIndex, regionIndex );

		lua_setnumber( lua, 2, 1, regionIndex.chunkIndex.block.x );
		lua_setnumber( lua, 2, 2, regionIndex.chunkIndex.block.y );
		lua_setnumber( lua, 2, 3, regionIndex.chunkIndex.block.z );

		lua_setnumber( lua, 2, "chunk", regionIndex.chunkIndex.chunk );
		lua_setnumber( lua, 2, "region", regionIndex.region );
		lua_setnumber( lua, 2, "length", 0.0f );

		return 0;
	}

	int loadWorld( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_STRING( 1 );

		// get path
		const char* path = lua_tostring( lua, 1 );
		g_coreData->world->loadWorld( path );

		return 0;
	}

	int saveWorld( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_STRING( 1 );

		// get path
		const char* path = lua_tostring( lua, 1 );
		g_coreData->world->saveWorld( path );

		return 0;
	}
}