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
			{ "marchBlock",	marchBlock },
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
		glm::vec3 location;
		bool didHit = g_coreData->world->hitBlock( rayStart, rayEnd, location );

		if( didHit )
		{
			lua_setvec3( lua, 3, location );
		}

		lua_pushboolean( lua, didHit );
		return 1;
	}

	int marchBlock( lua_State* lua )
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
		glm::vec3 location;
		bool didHit = g_coreData->world->marchBlock( rayStart, rayEnd, location );

		if( didHit )
		{
			lua_setvec3( lua, 3, location );
		}

		lua_pushboolean( lua, didHit );
		return 1;
	}
}