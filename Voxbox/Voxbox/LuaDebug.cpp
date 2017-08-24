#include "LuaDebug.h"

namespace LuaDebug
{
	static CoreData* g_coreData;
	static Array<LogMessage> g_logMessages;

	void bind( lua_State* lua, CoreData* coreData )
	{
		lua_register( lua, "log", log );

		lua_pushnumber( lua, VERBOSITY_INFORMATION );
		lua_setglobal( lua, "VERBOSITY_INFORMATION" );
		lua_pushnumber( lua, VERBOSITY_WARNING );
		lua_setglobal( lua, "VERBOSITY_WARNING" );
		lua_pushnumber( lua, VERBOSITY_ERROR );
		lua_setglobal( lua, "VERBOSITY_ERROR" );
		lua_pushnumber( lua, VERBOSITY_DEBUG );
		lua_setglobal( lua, "VERBOSITY_DEBUG" );

		// metatable for log
		luaL_newmetatable( lua, "logMeta" );
		luaL_Reg logRegs[] =
		{
			{ "log",			log },
			{ "setThreshold",	setThreshold },
			{ "getMessages",	getLogMessages },
			{ "getThreshold",	getLogThreshold },
			{ NULL,	NULL }
		};

		luaL_setfuncs( lua, logRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );

		lua_setglobal( lua, "Log" );

		// metatable for debug shapes
		luaL_newmetatable( lua, "debugShapesMeta" );
		luaL_Reg debugShapesRegs[] =
		{
			{ "addLine",		addLine },
			{ "addSphere",		addSphere },
			{ "addAABB",		addAABB },
			{ "addOBB",			addOBB },
			{ "ignoreDepth",	ignoreDepth },
			{ "setVisible",		setVisible },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, debugShapesRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );

		lua_setglobal( lua, "DebugShapes" );

		// metatable for system info
		luaL_newmetatable( lua, "systemInfoMeta" );
		luaL_Reg systemInfoRegs[] =
		{
			{ "poll",		poll },
			{ "getCores",	getCores },
			{ "getThreads",	getThreads },
			{ "getRam",		getRam },
			{ "getVsync",	getVsync },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, systemInfoRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );

		lua_setglobal( lua, "SystemInfo" );

		g_coreData = coreData;
	}

	int log( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_NUMBER( 1 );
		LUA_EXPECT_STRING( 2 );*/

		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_NUMBER( 1 ) &&
				LUA_EXPECT_STRING( 2 ) )
			{
				int verbosity = lua_toint( lua, 1 );
				const char* str = lua_tostring( lua, 2 );

				LOG( verbosity, "%s", str );
			}
		}

		return 0;
	}

	int setThreshold( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_NUMBER( 1 );*/

		LUA_EXPECT_ARGS( 1 ) 
		{
			if( LUA_EXPECT_NUMBER( 1 ) )
			{
				// get threshold
				int threshold = lua_toint( lua, 1 );

				if( threshold < VERBOSITY_INFORMATION || threshold > VERBOSITY_DEBUG )
				{
					LOG_ERROR( "Bad threshold value: %d", threshold );
				}
				else
					Log::instance().setThreshold( threshold );
			}
		}

		return 0;
	}

	int getLogMessages( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_NUMBER( 3 );*/

		LUA_EXPECT_ARGS( 3 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) &&
				LUA_EXPECT_NUMBER( 3 ) )
			{
				Log::instance().copyMessages( g_logMessages );

				int prevCount = lua_toint( lua, 3 ) - 1;
				if( prevCount < 0 )
					prevCount = 0;

				const int MESSAGE_COUNT = g_logMessages.getSize();
				for( int i=prevCount; i<MESSAGE_COUNT; i++ )
				{
					lua_setstring( lua, 1, i+1, g_logMessages[i].message );
					lua_setnumber( lua, 2, i+1, g_logMessages[i].verbosity );
				}
			}
		}

		return 0;
	}

	int getLogThreshold( lua_State* lua )
	{
		lua_pushnumber( lua, Log::instance().getThreshold() );
		return 1;
	}

	int addLine( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );*/

		LUA_EXPECT_ARGS( 3 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) &&
				LUA_EXPECT_TABLE( 3 ) )
			{
				// get start
				glm::vec3 start;
				lua_getvec3( lua, 1, start );

				// get end
				glm::vec3 end;
				lua_getvec3( lua, 2, end );

				// get color
				glm::vec4 color;
				lua_getvec4( lua, 3, color );

				g_coreData->debugShapes->addLine( { start, end, color } );
			}
		}

		return 0;
	}

	int addSphere( lua_State* lua )
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
				// get position
				glm::vec3 position;
				lua_getvec3( lua, 1, position );

				// get radius
				float radius = lua_tofloat( lua, 2 );

				// get color
				glm::vec4 color;
				lua_getvec4( lua, 3, color );

				g_coreData->debugShapes->addSphere( { position, radius, color } );
			}
		}

		return 0;
	}

	int addAABB( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );*/

		LUA_EXPECT_ARGS( 3 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) &&
				LUA_EXPECT_TABLE( 3 ) )
			{
				// get min position
				glm::vec3 minPosition;
				lua_getvec3( lua, 1, minPosition );

				// get max position
				glm::vec3 maxPosition;
				lua_getvec3( lua, 2, maxPosition );

				// get color
				glm::vec4 color;
				lua_getvec4( lua, 3, color );

				g_coreData->debugShapes->addAABB( { minPosition, maxPosition, color } );
			}
		}

		return 0;
	}

	int addOBB( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 6 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );
		LUA_EXPECT_TABLE( 4 );
		LUA_EXPECT_TABLE( 5 );
		LUA_EXPECT_TABLE( 6 );*/

		LUA_EXPECT_ARGS( 6 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) &&
				LUA_EXPECT_TABLE( 3 ) &&
				LUA_EXPECT_TABLE( 4 ) &&
				LUA_EXPECT_TABLE( 5 ) &&
				LUA_EXPECT_TABLE( 6 ) )
			{
				// get position
				glm::vec3 position;
				lua_getvec3( lua, 1, position );

				// get x axis
				glm::vec3 xAxis;
				lua_getvec3( lua, 2, xAxis );

				// get y axis
				glm::vec3 yAxis;
				lua_getvec3( lua, 3, yAxis );

				// get z axis
				glm::vec3 zAxis;
				lua_getvec3( lua, 4, zAxis );

				// get extents
				glm::vec3 extents;
				lua_getvec3( lua, 5, extents );

				// get color
				glm::vec4 color;
				lua_getvec4( lua, 6, color );

				g_coreData->debugShapes->addOBB( { position, xAxis, yAxis, zAxis, extents, color } );
			}
		}

		return 0;
	}

	int ignoreDepth( lua_State* lua )
	{
		int args = lua_gettop( lua );

		if( args <= 1 )
		{
			LOG_ERROR( "Expected less than 2 arguments. Got %d.", args );
		}
		else
		{
			bool ignore = true;
			if( args == 1 )
			{
				LUA_EXPECT_BOOL( 1 );

				ignore = lua_tobool( lua, 1 );
			}

			g_coreData->debugShapes->setIgnoreDepth( ignore );
		}

		return 0;
	}

	int setVisible( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_BOOL( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_BOOL( 1 ) )
			{
				// get visible
				bool visible = lua_tobool( lua, 1 );

				g_coreData->debugShapes->setVisible( visible );
			}
		}

		return 0;
	}

	// system info
	int poll( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 0 );

		g_coreData->systemInfo->poll();

		return 0;
	}

	int getCores( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 0 );

		// push result
		int cores = g_coreData->systemInfo->getCores();
		lua_pushnumber( lua, cores );

		return 1;
	}

	int getThreads( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 0 );

		// push result
		int threads = g_coreData->systemInfo->getThreads();
		lua_pushnumber( lua, threads );

		return 1;
	}

	int getRam( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 0 );

		// push result
		int ram = g_coreData->systemInfo->getRam();
		lua_pushnumber( lua, ram );

		return 1;
	}

	int getVsync( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 0 );

		// push result
		bool vsync = g_coreData->systemInfo->getVsync();
		lua_pushboolean( lua, vsync );

		return 1;
	}
}