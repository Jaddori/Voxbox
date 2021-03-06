#include "LuaCore.h"

namespace LuaCore
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		// metatable for core
		luaL_newmetatable( lua, "coreMeta" );
		luaL_Reg coreRegs[] =
		{
			{ "convertBytes",	convertBytes },
			{ "shrinkBytes",	shrinkBytes },
			{ "exit",			exit },
			{ "restart",		restart },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, coreRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Core" );

		lua_pushnumber( lua, UNIT_BYTES );
		lua_setglobal( lua, "UNIT_BYTES" );
		lua_pushnumber( lua, UNIT_KILOBYTES );
		lua_setglobal( lua, "UNIT_KILOBYTES" );
		lua_pushnumber( lua, UNIT_MEGABYTES );
		lua_setglobal( lua, "UNIT_MEGABYTES" );
		lua_pushnumber( lua, UNIT_GIGABYTES );
		lua_setglobal( lua, "UNIT_GIGABYTES" );

		// metatable for vec2
		luaL_newmetatable( lua, "vec2Meta" );
		luaL_Reg vec2Regs[] =
		{
			{ "create",		vec2::create },

			{ "__add",		vec2::add },
			{ "__sub",		vec2::sub },
			{ "__mul",		vec2::mul },
			{ "__div",		vec2::div },
			{ "__unm",		vec2::neg },
			{ "__eq",		vec2::eq },

			{ "length",		vec2::length },
			{ "normalize",	vec2::normalize },
			{ "distance",	vec2::distance },
			{ "direction",	vec2::direction },
			{ "print",		vec2::print },
			{ "tostring",	vec2::tostring },
			{ "copy",		vec2::copy },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, vec2Regs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Vec2" );

		lua_register( lua, "vec2", vec2::create );

		// metatable for vec3
		luaL_newmetatable( lua, "vec3Meta" );
		luaL_Reg vec3Regs[] =
		{
			{ "create",		vec3::create },

			{ "__add",		vec3::add },
			{ "__sub",		vec3::sub },
			{ "__mul",		vec3::mul },
			{ "__div",		vec3::div },
			{ "__unm",		vec3::neg },
			{ "__eq",		vec3::eq },

			{ "length",		vec3::length },
			{ "normalize",	vec3::normalize },
			{ "distance",	vec3::distance },
			{ "direction",	vec3::direction },
			{ "print",		vec3::print },
			{ "tostring",	vec3::tostring },
			{ "copy",		vec3::copy },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, vec3Regs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Vec3" );

		lua_register( lua, "vec3", vec3::create );

		// metatable for vec4
		luaL_newmetatable( lua, "vec4Meta" );
		luaL_Reg vec4Regs[] =
		{
			{ "create",		vec4::create },

			{ "__add",		vec4::add },
			{ "__sub",		vec4::sub },
			{ "__mul",		vec4::mul },
			{ "__div",		vec4::div },
			{ "__unm",		vec4::neg },
			{ "__eq",		vec4::eq },

			{ "length",		vec4::length },
			{ "normalize",	vec4::normalize },
			{ "distance",	vec4::distance },
			{ "direction",	vec4::direction },
			{ "print",		vec4::print },
			{ "tostring",	vec4::tostring },
			{ "copy",		vec4::copy },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, vec4Regs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Vec4" );

		lua_register( lua, "vec4", vec4::create );

		g_coreData = coreData;
	}

	int convertBytes( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_NUMBER( 1 );
		LUA_EXPECT_NUMBER( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_NUMBER( 1 ) &&
				LUA_EXPECT_NUMBER( 2 ) )
			{
				// get bytes
				int bytes = lua_toint( lua, 1 );

				// get new unit
				int newUnit = lua_toint( lua, 2 );

				// push result
				float convertedBytes = ::convertBytes( bytes, newUnit );
				lua_pushnumber( lua, convertedBytes );

				result = 1;
			}
		}

		return result;
	}

	int shrinkBytes( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_NUMBER( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_NUMBER( 1 ) )
			{
				// get bytes
				int bytes = lua_toint( lua, 1 );

				// push result
				int newUnit;
				float shrunkenBytes = ::shrinkBytes( bytes, newUnit );

				lua_pushnumber( lua, shrunkenBytes );
				lua_pushnumber( lua, newUnit );

				result = 2;
			}
		}

		return result;
	}

	int exit( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 0 );

		*g_coreData->running = false;
		*g_coreData->executing = false;

		return 0;
	}

	int restart( lua_State* lua )
	{
		//LUA_ASSERT_ARGS( 0 );

		*g_coreData->running = false;

		return 0;
	}

	// vec2
	int vec2::create( lua_State* lua )
	{
		int result = 0;

		int args = lua_gettop( lua );
		if( args != 0 && args != 2 )
		{
			LOG_ERROR( "Expected 0 or 2 arguments. Got %d.", args );
		}
		else
		{
			lua_newtable( lua );

			float x = 0.0f, y = 0.0f;

			if( args == 2 )
			{
				if( LUA_EXPECT_NUMBER( 1 ) &&
					LUA_EXPECT_NUMBER( 2 ) )
				{
					x = lua_tofloat( lua, 1 );
					y = lua_tofloat( lua, 2 );
				}
			}

			lua_setnumber( lua, -2, 1, x );
			lua_setnumber( lua, -2, 2, y );

			luaL_setmetatable( lua, "vec2Meta" );
			result = 1;
		}

		return result;
	}

	int vec2::add( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec2 a;
				lua_getvec2( lua, 1, a );

				// get b
				glm::vec2 b;
				lua_getvec2( lua, 2, b );

				// push result
				glm::vec2 v = a + b;
				lua_newtable( lua );
				lua_setvec2( lua, -2, v );
				luaL_setmetatable( lua, "vec2Meta" );

				result = 1;
			}
		}

		return result;
	}

	int vec2::sub( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec2 a;
				lua_getvec2( lua, 1, a );

				// get b
				glm::vec2 b;
				lua_getvec2( lua, 2, b );

				// push result
				glm::vec2 v = a - b;
				lua_newtable( lua );
				lua_setvec2( lua, -2, v );
				luaL_setmetatable( lua, "vec2Meta" );

				result = 1;
			}
		}

		return result;
	}

	int vec2::mul( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				if( !lua_istable( lua, 2 ) && !lua_isnumber( lua, 2 ) )
				{
					LOG_ERROR( "Expected table or number as argument #2." );
				}
				else
				{
					glm::vec2 v;

					// get a
					glm::vec2 a;
					lua_getvec2( lua, 1, a );

					if( lua_istable( lua, 2 ) )
					{
						// get b
						glm::vec2 b;
						lua_getvec2( lua, 2, b );
						v = a * b;
					}
					else
					{
						// get b
						float b = lua_tofloat( lua, 2 );
						v = a * b;
					}

					// push result
					lua_newtable( lua );
					lua_setvec2( lua, -2, v );
					luaL_setmetatable( lua, "vec2Meta" );

					result = 1;
				}
			}
		}

		return result;
	}

	int vec2::div( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				if( !lua_istable( lua, 2 ) && !lua_isnumber( lua, 2 ) )
				{
					LOG_ERROR( "Expected table or number as argument #2." );
				}
				else
				{
					glm::vec2 v;

					// get a
					glm::vec2 a;
					lua_getvec2( lua, 1, a );

					if( lua_istable( lua, 2 ) )
					{
						// get b
						glm::vec2 b;
						lua_getvec2( lua, 2, b );
						v = a / b;
					}
					else
					{
						// get b
						float b = lua_tofloat( lua, 2 );
						v = a / b;
					}

					// push result
					lua_newtable( lua );
					lua_setvec2( lua, -2, v );
					luaL_setmetatable( lua, "vec2Meta" );

					result = 1;
				}
			}
		}

		return result;
	}

	int vec2::neg( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec2
				glm::vec2 v;
				lua_getvec2( lua, 1, v );

				// push result
				glm::vec2 u = -v;
				lua_newtable( lua );
				lua_setvec2( lua, -2, u );
				luaL_setmetatable( lua, "vec2Meta" );

				result = 1;
			}
		}

		return result;
	}

	int vec2::eq( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec2 a;
				lua_getvec2( lua, 1, a );

				// get b
				glm::vec2 b;
				lua_getvec2( lua, 2, b );

				// push result
				lua_pushboolean( lua, a == b );

				result = 1;
			}
		}

		return result;
	}

	int vec2::length( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec2
				glm::vec2 v;
				lua_getvec2( lua, 1, v );

				// push result
				float len = glm::length( v );
				lua_pushnumber( lua, len );

				result = 1;
			}
		}

		return result;
	}

	int vec2::normalize( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec2
				glm::vec2 v;
				lua_getvec2( lua, 1, v );

				// set result
				v = glm::normalize( v );
				lua_setvec2( lua, 1, v );
			}
		}

		return 0;
	}

	int vec2::distance( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec2 a;
				lua_getvec2( lua, 1, a );

				// get b
				glm::vec2 b;
				lua_getvec2( lua, 2, b );

				// push result
				float dist = glm::distance( a, b );
				lua_pushnumber( lua, dist );

				result = 1;
			}
		}

		return result;
	}

	int vec2::direction( lua_State* lua )
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
				// get a
				glm::vec2 a;
				lua_getvec2( lua, 1, a );

				// get b
				glm::vec2 b;
				lua_getvec2( lua, 2, b );

				// set result
				glm::vec2 dir = glm::normalize( b-a );
				lua_setvec2( lua, 3, dir );
			}
		}

		return 0;
	}

	int vec2::print( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec2
				glm::vec2 v;
				lua_getvec2( lua, 1, v );

				// print vec2
				LOG_DEBUG( "(%.2f,%.2f)", v.x, v.y );
			}
		}

		return 0;
	}

	int vec2::tostring( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 ) 
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec2
				glm::vec2 v;
				lua_getvec2( lua, 1, v );

				// push result
				char buf[32];
				snprintf( buf, 32, "(%.2f,%.2f)", v.x, v.y );
				lua_pushstring( lua, buf );

				result = 1;
			}
		}

		return result;
	}

	int vec2::copy( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get vec2
				glm::vec2 v;
				lua_getvec2( lua, 1, v );

				// set result
				lua_setvec2( lua, 2, v );
			}	
		}

		return 0;
	}

	// vec3
	int vec3::create( lua_State* lua )
	{
		int result = 0;

		int args = lua_gettop( lua );
		if( args != 0 && args != 3 )
		{
			LOG_ERROR( "Expected 0 or 3 arguments. Got %d.", args );
		}
		else
		{
			lua_newtable( lua );

			float x = 0.0f, y = 0.0f, z = 0.0f;

			if( args == 3 )
			{
				LUA_EXPECT_NUMBER( 1 );
				LUA_EXPECT_NUMBER( 2 );
				LUA_EXPECT_NUMBER( 3 );

				x = lua_tofloat( lua, 1 );
				y = lua_tofloat( lua, 2 );
				z = lua_tofloat( lua, 3 );
			}

			lua_setnumber( lua, -2, 1, x );
			lua_setnumber( lua, -2, 2, y );
			lua_setnumber( lua, -2, 3, z );

			luaL_setmetatable( lua, "vec3Meta" );

			result = 1;
		}

		return result;
	}

	int vec3::add( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec3 a;
				lua_getvec3( lua, 1, a );

				// get b
				glm::vec3 b;
				lua_getvec3( lua, 2, b );

				// push result
				glm::vec3 v = a + b;
				lua_newtable( lua );
				lua_setvec3( lua, -2, v );
				luaL_setmetatable( lua, "vec3Meta" );

				result = 1;
			}
		}

		return result;
	}

	int vec3::sub( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec3 a;
				lua_getvec3( lua, 1, a );

				// get b
				glm::vec3 b;
				lua_getvec3( lua, 2, b );

				// push result
				glm::vec3 v = a - b;
				lua_newtable( lua );
				lua_setvec3( lua, -2, v );
				luaL_setmetatable( lua, "vec3Meta" );

				result = 1;
			}
		}
		
		return result;
	}

	int vec3::mul( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				if( !lua_istable( lua, 2 ) && !lua_isnumber( lua, 2 ) )
				{
					LOG_ERROR( "Expected table or number as argument #2." );
				}
				else
				{
					glm::vec3 v;

					// get a
					glm::vec3 a;
					lua_getvec3( lua, 1, a );

					if( lua_istable( lua, 2 ) )
					{
						// get b
						glm::vec3 b;
						lua_getvec3( lua, 2, b );
						v = a * b;
					}
					else
					{
						// get b
						float b = lua_tofloat( lua, 2 );
						v = a * b;
					}

					// push result
					lua_newtable( lua );
					lua_setvec3( lua, -2, v );
					luaL_setmetatable( lua, "vec3Meta" );

					result = 1;
				}
			}
		}

		return result;
	}

	int vec3::div( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				if( !lua_istable( lua, 2 ) && !lua_isnumber( lua, 2 ) )
				{
					LOG_ERROR( "Expected table or number as argument #2." );
				}
				else
				{
					glm::vec3 v;

					// get a
					glm::vec3 a;
					lua_getvec3( lua, 1, a );

					if( lua_istable( lua, 2 ) )
					{
						// get b
						glm::vec3 b;
						lua_getvec3( lua, 2, b );
						v = a / b;
					}
					else
					{
						// get b
						float b = lua_tofloat( lua, 2 );
						v = a / b;
					}

					// push result
					lua_newtable( lua );
					lua_setvec3( lua, -2, v );
					luaL_setmetatable( lua, "vec3Meta" );

					result = 1;
				}
			}
		}

		return result;
	}

	int vec3::neg( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec3
				glm::vec3 v;
				lua_getvec3( lua, 1, v );

				// push result
				glm::vec3 u = -v;
				lua_newtable( lua );
				lua_setvec3( lua, -2, u );
				luaL_setmetatable( lua, "vec3Meta" );

				result = 1;
			}
		}

		return result;
	}

	int vec3::eq( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec3 a;
				lua_getvec3( lua, 1, a );

				// get b
				glm::vec3 b;
				lua_getvec3( lua, 2, b );

				// push result
				lua_pushboolean( lua, a == b );

				result = 1;
			}
		}

		return result;
	}

	int vec3::length( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec3
				glm::vec3 v;
				lua_getvec3( lua, 1, v );

				// push result
				float len = glm::length( v );
				lua_pushnumber( lua, len );

				result = 1;
			}
		}

		return result;
	}

	int vec3::normalize( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 );
		if( LUA_EXPECT_TABLE( 1 ) )
		{
			// get vec3
			glm::vec3 v;
			lua_getvec3( lua, 1, v );

			// set result
			v = glm::normalize( v );
			lua_setvec3( lua, 1, v );
		}

		return 0;
	}

	int vec3::distance( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec3 a;
				lua_getvec3( lua, 1, a );

				// get b
				glm::vec3 b;
				lua_getvec3( lua, 2, b );

				// push result
				float dist = glm::distance( a, b );
				lua_pushnumber( lua, dist );

				result = 1;
			}
		}

		return result;
	}

	int vec3::direction( lua_State* lua )
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
				// get a
				glm::vec3 a;
				lua_getvec3( lua, 1, a );

				// get b
				glm::vec3 b;
				lua_getvec3( lua, 2, b );

				// set result
				glm::vec3 dir = glm::normalize( b - a );
				lua_setvec3( lua, 3, dir );
			}
		}

		return 0;
	}

	int vec3::print( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec3
				glm::vec3 v;
				lua_getvec3( lua, 1, v );

				// print vec3
				LOG_DEBUG( "(%.2f,%.2f,%.2f)", v.x, v.y, v.z );
			}
		}

		return 0;
	}

	int vec3::tostring( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec3
				glm::vec3 v;
				lua_getvec3( lua, 1, v );

				// push result
				char buf[32];
				snprintf( buf, 32, "(%.2f,%.2f,%.2f)", v.x, v.y, v.z );
				lua_pushstring( lua, buf );

				result = 1;
			}
		}

		return result;
	}

	int vec3::copy( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get vec3
				glm::vec3 v;
				lua_getvec3( lua, 1, v );

				// set result
				lua_setvec3( lua, 2, v );
			}
		}

		return 0;
	}

	// vec4
	int vec4::create( lua_State* lua )
	{
		int result = 0;

		int args = lua_gettop( lua );
		if( args != 0 && args != 4 )
		{
			LOG_ERROR( "Expected 0 or 4 arguments. Got %d.", args );
		}
		else
		{
			lua_newtable( lua );

			float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

			if( args == 4 )
			{
				if( LUA_EXPECT_NUMBER( 1 ) &&
					LUA_EXPECT_NUMBER( 2 ) &&
					LUA_EXPECT_NUMBER( 3 ) &&
					LUA_EXPECT_NUMBER( 4 ) )
				{
					x = lua_tofloat( lua, 1 );
					y = lua_tofloat( lua, 2 );
					z = lua_tofloat( lua, 3 );
					w = lua_tofloat( lua, 4 );
				}
			}

			lua_setnumber( lua, -2, 1, x );
			lua_setnumber( lua, -2, 2, y );
			lua_setnumber( lua, -2, 3, z );
			lua_setnumber( lua, -2, 4, w );

			luaL_setmetatable( lua, "vec4Meta" );

			result = 1;
		}

		return result;
	}

	int vec4::add( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec4 a;
				lua_getvec4( lua, 1, a );

				// get b
				glm::vec4 b;
				lua_getvec4( lua, 2, b );

				// push result
				glm::vec4 v = a + b;
				lua_newtable( lua );
				lua_setvec4( lua, -2, v );
				luaL_setmetatable( lua, "vec4Meta" );

				result = 1;
			}
		}

		return result;
	}

	int vec4::sub( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec4 a;
				lua_getvec4( lua, 1, a );

				// get b
				glm::vec4 b;
				lua_getvec4( lua, 2, b );

				// push result
				glm::vec4 v = a - b;
				lua_newtable( lua );
				lua_setvec4( lua, -2, v );
				luaL_setmetatable( lua, "vec4Meta" );

				result = 1;
			}
		}

		return result;
	}

	int vec4::mul( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				if( !lua_istable( lua, 2 ) && !lua_istable( lua, 2 ) )
				{
					LOG_ERROR( "Expected table or number as argument #2." );
				}
				else
				{
					glm::vec4 v;

					// get a
					glm::vec4 a;
					lua_getvec4( lua, 1, a );

					if( lua_istable( lua, 2 ) )
					{
						// get b
						glm::vec4 b;
						lua_getvec4( lua, 2, b );
						v = a * b;
					}
					else
					{
						// get b
						float b = lua_tofloat( lua, 2 );
						v = a * b;
					}

					// push result
					lua_newtable( lua );
					lua_setvec4( lua, -2, v );
					luaL_setmetatable( lua, "vec4Meta" );

					result = 1;
				}
			}
		}

		return result;
	}

	int vec4::div( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				if( !lua_istable( lua, 2 ) && !lua_isnumber( lua, 2 ) )
				{
					LOG_ERROR( "Expected table or number as argument #2." );
				}
				else
				{
					glm::vec4 v;

					// get a
					glm::vec4 a;
					lua_getvec4( lua, 1, a );

					if( lua_istable( lua, 2 ) )
					{
						// get b
						glm::vec4 b;
						lua_getvec4( lua, 2, b );
						v = a / b;
					}
					else
					{
						// get b
						float b = lua_tofloat( lua, 2 );
						v = a / b;
					}

					// push result
					lua_newtable( lua );
					lua_setvec4( lua, -2, v );
					luaL_setmetatable( lua, "vec4Meta" );

					result = 1;
				}
			}
		}

		return result;
	}

	int vec4::neg( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec4
				glm::vec4 v;
				lua_getvec4( lua, 1, v );

				// push result
				glm::vec4 u = -v;
				lua_newtable( lua );
				lua_setvec4( lua, -2, u );
				luaL_setmetatable( lua, "vec4Meta" );

				result = 1;
			}
		}

		return result;
	}

	int vec4::eq( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec4 a;
				lua_getvec4( lua, 1, a );

				// get b
				glm::vec4 b;
				lua_getvec4( lua, 2, b );

				// push result
				lua_pushboolean( lua, a == b );

				result = 1;
			}
		}

		return result;
	}

	int vec4::length( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec4
				glm::vec4 v;
				lua_getvec4( lua, 1, v );

				// push result
				float len = glm::length( v );
				lua_pushnumber( lua, len );

				result = 1;
			}
		}

		return result;
	}

	int vec4::normalize( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec4
				glm::vec4 v;
				lua_getvec4( lua, 1, v );

				// set result
				v = glm::normalize( v );
				lua_setvec4( lua, 1, v );
			}
		}

		return 0;
	}

	int vec4::distance( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get a
				glm::vec4 a;
				lua_getvec4( lua, 1, a );

				// get b
				glm::vec4 b;
				lua_getvec4( lua, 2, b );

				// push result
				float dist = glm::distance( a, b );
				lua_pushnumber( lua, dist );

				result = 1;
			}
		}

		return result;
	}

	int vec4::direction( lua_State* lua )
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
				// get a
				glm::vec4 a;
				lua_getvec4( lua, 1, a );

				// get b
				glm::vec4 b;
				lua_getvec4( lua, 2, b );

				// set result
				glm::vec4 dir = glm::normalize( b - a );
				lua_setvec4( lua, 3, dir );
			}
		}

		return 0;
	}

	int vec4::print( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec4 
				glm::vec4 v;
				lua_getvec4( lua, 1, v );

				// print vec4
				LOG_DEBUG( "(%.2f,%.2f,%.2f,%.2f)", v.x, v.y, v.z, v.w );
			}
		}

		return 0;
	}

	int vec4::tostring( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				// get vec4
				glm::vec4 v;
				lua_getvec4( lua, 1, v );

				// push result
				char buf[32];
				snprintf( buf, 32, "(%.2f,%.2f,%.2f,%.2f)", v.x, v.y, v.z, v.w );
				lua_pushstring( lua, buf );

				result = 1;
			}
		}

		return result;
	}

	int vec4::copy( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );*/

		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_TABLE( 2 ) )
			{
				// get vec4
				glm::vec4 v;
				lua_getvec4( lua, 1, v );

				// set result
				lua_setvec4( lua, 2, v);
			}
		}

		return 0;
	}
}