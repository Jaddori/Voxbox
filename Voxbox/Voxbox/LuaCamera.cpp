#include "LuaCamera.h"

namespace LuaCamera
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		// metatable for camera
		luaL_newmetatable( lua, "cameraMeta" );
		luaL_Reg cameraRegs[] =
		{
			{ "project",			project },
			{ "unproject",			unproject },

			{ "relativeMovement",	relativeMovement },
			{ "absoluteMovement",	absoluteMovement },
			{ "updateDirection",	updateDirection },
			
			{ "setPosition",		setPosition },
			{ "setDirection",		setDirection },

			{ "getPosition",		getPosition },
			{ "getDirection",		getDirection },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, cameraRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Camera" );

		g_coreData = coreData;
	}

	int project( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get world coordinates
				glm::vec3 worldCoordinates;
				lua_getvec3( lua, 1, worldCoordinates );

				Point result;
				g_coreData->perspectiveCamera->project( worldCoordinates, result );

				// set result
				lua_setnumber( lua, 2, 1, result.x );
				lua_setnumber( lua, 2, 2, result.y );
			}
		}

		return 0;
	}

	int unproject( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_NUMBER( 2 );
		LUA_EXPECT_TABLE( 3 );*/

		LUA_EXPECT_ARGS( 3 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_NUMBER( 2 ) &&
				LUA_EXPECT_TABLE( 3 ) )
			{
				// get window coordinates
				Point windowCoordinates;
				windowCoordinates.x = lua_getint( lua, 1, 1 );
				windowCoordinates.y = lua_getint( lua, 1, 2 );

				// get depth
				float depth = lua_tofloat( lua, 2 );

				glm::vec3 result;
				g_coreData->perspectiveCamera->unproject( windowCoordinates, depth, result );

				// set result
				lua_setvec3( lua, 3, result );
			}
		}

		return 0;
	}

	int relativeMovement( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get local movement
				glm::vec3 localMovement;
				lua_getvec3( lua, 1, localMovement );

				g_coreData->perspectiveCamera->relativeMovement( localMovement );
			}
		}

		return 0;
	}

	int absoluteMovement( lua_State* lua )
	{
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get world movement
				glm::vec3 worldMovement;
				lua_getvec3( lua, 1, worldMovement );

				g_coreData->perspectiveCamera->absoluteMovement( worldMovement );
			}
		}

		return 0;
	}

	int updateDirection( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get delta x & y
				int deltaX = lua_getint( lua, 1, 1 );
				int deltaY = lua_getint( lua, 1, 2 );

				g_coreData->perspectiveCamera->updateDirection( deltaX, deltaY );
			}
		}

		return 0;
	}

	int setPosition( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get position
				glm::vec3 position;
				lua_getvec3( lua, 1, position );

				g_coreData->perspectiveCamera->setPosition( position );
			}
		}

		return 0;
	}

	int setDirection( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get direction
				glm::vec3 direction;
				lua_getvec3( lua, 1, direction );

				g_coreData->perspectiveCamera->setDirection( direction );
			}
		}

		return 0;
	}

	int getPosition( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				glm::vec3 position = g_coreData->perspectiveCamera->getPosition();

				// set result
				lua_setvec3( lua, 1, position );
			}
		}

		return 0;
	}

	int getDirection( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				glm::vec3 direction = g_coreData->perspectiveCamera->getDirection();

				// set result
				lua_setvec3( lua, 1, direction );
			}
		}

		return 0;
	}
}