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
			{ "hitBlock",	hitBlock },
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
}