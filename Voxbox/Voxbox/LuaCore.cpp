#include "LuaCore.h"

namespace LuaCore
{
	void bind( lua_State* lua, CoreData* coreData )
	{
		// metatable for core
		luaL_newmetatable( lua, "coreMeta" );
		luaL_Reg coreRegs[] =
		{
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, coreRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Core" );

		// metatable for vec2
		luaL_newmetatable( lua, "vec2Meta" );
		luaL_Reg vec2Regs[] =
		{
			{ "add",		vec2::add },
			{ "addeq",		vec2::addeq },
			{ "sub",		vec2::sub },
			{ "subeq",		vec2::subeq },
			{ "mul",		vec2::mul },
			{ "muleq",		vec2::muleq },
			{ "div",		vec2::div },
			{ "diveq",		vec2::diveq },

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

		// metatable for vec3
		luaL_newmetatable( lua, "vec3Meta" );
		luaL_Reg vec3Regs[] =
		{
			{ "add",		vec3::add },
			{ "addeq",		vec3::addeq },
			{ "sub",		vec3::sub },
			{ "subeq",		vec3::subeq },
			{ "mul",		vec3::mul },
			{ "muleq",		vec3::muleq },
			{ "div",		vec3::div },
			{ "diveq",		vec3::diveq },

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

		// metatable for vec4
		luaL_newmetatable( lua, "vec4Meta" );
		luaL_Reg vec4Regs[] =
		{
			{ "add",		vec4::add },
			{ "addeq",		vec4::addeq },
			{ "sub",		vec4::sub },
			{ "subeq",		vec4::subeq },
			{ "mul",		vec4::mul },
			{ "muleq",		vec4::muleq },
			{ "div",		vec4::div },
			{ "diveq",		vec4::diveq },

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
	}

	// vec2
	int vec2::add( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		// get b
		glm::vec2 b;
		lua_getvec2( lua, 2, b );

		// set result
		glm::vec2 result = a + b;
		lua_setvec2( lua, 3, result );

		return 0;
	}

	int vec2::addeq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		// get b
		glm::vec2 b;
		lua_getvec2( lua, 2, b );

		// set result
		glm::vec2 result = a + b;
		lua_setvec2( lua, 1, result );

		return 0;
	}

	int vec2::sub( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		// get b
		glm::vec2 b;
		lua_getvec2( lua, 2, b );

		// set result
		glm::vec2 result = a - b;
		lua_setvec2( lua, 3, result );

		return 0;
	}

	int vec2::subeq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		// get b
		glm::vec2 b;
		lua_getvec2( lua, 2, b );

		// set result
		glm::vec2 result = a - b;
		lua_setvec2( lua, 1, result );

		return 0;
	}

	int vec2::mul( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );
		LUA_EXPECT_TABLE( 3 );

		glm::vec2 result;

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		if( lua_istable( lua, 2 ) )
		{
			// get b
			glm::vec2 b;
			lua_getvec2( lua, 2, b );

			result = a * b;
		}
		else
		{
			// get b
			float b = lua_tofloat( lua, 2 );
			result = a * b;
		}

		// set result
		lua_setvec2( lua, 3, result );

		return 0;
	}

	int vec2::muleq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );

		glm::vec2 result;

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		if( lua_istable( lua, 2 ) )
		{
			// get b
			glm::vec2 b;
			lua_getvec2( lua, 2, b );

			result = a * b;
		}
		else
		{
			// get b
			float b = lua_tofloat( lua, 2 );
			result = a * b;
		}

		// set result
		lua_setvec2( lua, 1, result );

		return 0;
	}

	int vec2::div( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		// get b
		glm::vec2 b;
		lua_getvec2( lua, 2, b );

		// set result
		glm::vec2 result = a / b;
		lua_setvec2( lua, 3, result );

		return 0;
	}

	int vec2::diveq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );

		glm::vec2 result;

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		if( lua_istable( lua, 2 ) )
		{
			// get b
			glm::vec2 b;
			lua_getvec2( lua, 2, b );

			result = a / b;
		}
		else
		{
			// get b
			float b = lua_tofloat( lua, 2 );
			result = a / b;
		}

		// set result
		lua_setvec2( lua, 1, result );

		return 0;
	}

	int vec2::length( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec2
		glm::vec2 v;
		lua_getvec2( lua, 1, v );

		// push result
		float len = glm::length( v );
		lua_pushnumber( lua, len );

		return 1;
	}

	int vec2::normalize( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec2
		glm::vec2 v;
		lua_getvec2( lua, 1, v );

		// set result
		v = glm::normalize( v );
		lua_setvec2( lua, 1, v );

		return 0;
	}

	int vec2::distance( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		// get b
		glm::vec2 b;
		lua_getvec2( lua, 2, b );

		// push result
		float dist = glm::distance( a, b );
		lua_pushnumber( lua, dist );

		return 1;
	}

	int vec2::direction( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec2 a;
		lua_getvec2( lua, 1, a );

		// get b
		glm::vec2 b;
		lua_getvec2( lua, 2, b );

		// set result
		glm::vec2 dir = glm::normalize( b-a );
		lua_setvec2( lua, 3, dir );

		return 0;
	}

	int vec2::print( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec2
		glm::vec2 v;
		lua_getvec2( lua, 1, v );

		// print vec2
		LOG_DEBUG( "(%f,%f)", v.x, v.y );

		return 0;
	}

	int vec2::tostring( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec2
		glm::vec2 v;
		lua_getvec2( lua, 1, v );

		// push result
		char buf[32];
		snprintf( buf, 32, "(%f,%f)", v.x, v.y );
		lua_pushstring( lua, buf );

		return 1;
	}

	int vec2::copy( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get vec2
		glm::vec2 v;
		lua_getvec2( lua, 1, v );

		// set result
		lua_setvec2( lua, 2, v );

		return 0;
	}

	// vec3
	int vec3::add( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		// get b
		glm::vec3 b;
		lua_getvec3( lua, 2, b );

		// set result
		glm::vec3 result = a + b;
		lua_setvec3( lua, 3, result );

		return 0;
	}

	int vec3::addeq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		// get b
		glm::vec3 b;
		lua_getvec3( lua, 2, b );

		// set result
		glm::vec3 result = a + b;
		lua_setvec3( lua, 1, result );

		return 0;
	}

	int vec3::sub( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		// get b
		glm::vec3 b;
		lua_getvec3( lua, 2, b );

		// set result
		glm::vec3 result = a - b;
		lua_setvec3( lua, 3, result );

		return 0;
	}

	int vec3::subeq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		// get b
		glm::vec3 b;
		lua_getvec3( lua, 2, b );

		// set result
		glm::vec3 result = a - b;
		lua_setvec3( lua, 1, result );

		return 0;
	}

	int vec3::mul( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );
		LUA_EXPECT_TABLE( 3 );

		glm::vec3 result;

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		if( lua_istable( lua, 2 ) )
		{
			// get b
			glm::vec3 b;
			lua_getvec3( lua, 2, b );

			result = a * b;
		}
		else
		{
			// get b
			float b = lua_tofloat( lua, 2 );
			result = a * b;
		}

		// set result
		lua_setvec3( lua, 3, result );

		return 0;
	}

	int vec3::muleq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );

		glm::vec3 result;

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		if( lua_istable( lua, 2 ) )
		{
			// get b
			glm::vec3 b;
			lua_getvec3( lua, 2, b );

			result = a * b;
		}
		else
		{
			// get b
			float b = lua_tofloat( lua, 2 );
			result = a * b;
		}

		// set result
		lua_setvec3( lua, 1, result );

		return 0;
	}

	int vec3::div( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		// get b
		glm::vec3 b;
		lua_getvec3( lua, 2, b );

		// set result
		glm::vec3 result = a / b;
		lua_setvec3( lua, 3, result );

		return 0;
	}

	int vec3::diveq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );

		glm::vec3 result;

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		if( lua_istable( lua, 2 ) )
		{
			// get b
			glm::vec3 b;
			lua_getvec3( lua, 2, b );

			result = a / b;
		}
		else
		{
			// get b
			float b = lua_tofloat( lua, 2 );
			result = a / b;
		}

		// set result
		lua_setvec3( lua, 1, result );

		return 0;
	}

	int vec3::length( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec3
		glm::vec3 v;
		lua_getvec3( lua, 1, v );

		// push result
		float len = glm::length( v );
		lua_pushnumber( lua, len );

		return 1;
	}

	int vec3::normalize( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec3
		glm::vec3 v;
		lua_getvec3( lua, 1, v );

		// set result
		v = glm::normalize( v );
		lua_setvec3( lua, 1, v );

		return 0;
	}

	int vec3::distance( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		// get b
		glm::vec3 b;
		lua_getvec3( lua, 2, b );

		// push result
		float dist = glm::distance( a, b );
		lua_pushnumber( lua, dist );

		return 1;
	}

	int vec3::direction( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec3 a;
		lua_getvec3( lua, 1, a );

		// get b
		glm::vec3 b;
		lua_getvec3( lua, 2, b );

		// set result
		glm::vec3 dir = glm::normalize( b - a );
		lua_setvec3( lua, 3, dir );

		return 0;
	}

	int vec3::print( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec3
		glm::vec3 v;
		lua_getvec3( lua, 1, v );

		// print vec3
		LOG_DEBUG( "(%f,%f)", v.x, v.y );

		return 0;
	}

	int vec3::tostring( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec3
		glm::vec3 v;
		lua_getvec3( lua, 1, v );

		// push result
		char buf[32];
		snprintf( buf, 32, "(%f,%f,%f)", v.x, v.y, v.z );
		lua_pushstring( lua, buf );

		return 1;
	}

	int vec3::copy( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get vec3
		glm::vec3 v;
		lua_getvec3( lua, 1, v );

		// set result
		lua_setvec3( lua, 2, v );

		return 0;
	}

	// vec4
	int vec4::add( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		// get b
		glm::vec4 b;
		lua_getvec4( lua, 2, b );

		// set result
		glm::vec4 result = a + b;
		lua_setvec4( lua, 3, result );

		return 0;
	}

	int vec4::addeq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		// get b
		glm::vec4 b;
		lua_getvec4( lua, 2, b );

		// set result
		glm::vec4 result = a + b;
		lua_setvec4( lua, 1, result );

		return 0;
	}

	int vec4::sub( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		// get b
		glm::vec4 b;
		lua_getvec4( lua, 2, b );

		// set result
		glm::vec4 result = a - b;
		lua_setvec4( lua, 3, result );

		return 0;
	}

	int vec4::subeq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		// get b
		glm::vec4 b;
		lua_getvec4( lua, 2, b );

		// set result
		glm::vec4 result = a - b;
		lua_setvec4( lua, 1, result );

		return 0;
	}

	int vec4::mul( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );
		LUA_EXPECT_TABLE( 3 );

		glm::vec4 result;

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		if( lua_istable( lua, 2 ) )
		{
			// get b
			glm::vec4 b;
			lua_getvec4( lua, 2, b );

			result = a * b;
		}
		else
		{
			// get b
			float b = lua_tofloat( lua, 2 );
			result = a * b;
		}

		// set result
		lua_setvec4( lua, 3, result );

		return 0;
	}

	int vec4::muleq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );

		glm::vec4 result;

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		if( lua_istable( lua, 2 ) )
		{
			// get b
			glm::vec4 b;
			lua_getvec4( lua, 2, b );

			result = a * b;
		}
		else
		{
			// get b
			float b = lua_tofloat( lua, 2 );
			result = a * b;
		}

		// set result
		lua_setvec4( lua, 1, result );

		return 0;
	}

	int vec4::div( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		// get b
		glm::vec4 b;
		lua_getvec4( lua, 2, b );

		// set result
		glm::vec4 result = a / b;
		lua_setvec4( lua, 3, result );

		return 0;
	}

	int vec4::diveq( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LOG_ASSERT( lua_istable( lua, 2 ) || lua_isnumber( lua, 2 ), "Expected table or number as argument #2." );

		glm::vec4 result;

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		if( lua_istable( lua, 2 ) )
		{
			// get b
			glm::vec4 b;
			lua_getvec4( lua, 2, b );

			result = a / b;
		}
		else
		{
			// get b
			float b = lua_tofloat( lua, 2 );
			result = a / b;
		}

		// set result
		lua_setvec4( lua, 1, result );

		return 0;
	}

	int vec4::length( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec4
		glm::vec4 v;
		lua_getvec4( lua, 1, v );

		// push result
		float len = glm::length( v );
		lua_pushnumber( lua, len );

		return 1;
	}

	int vec4::normalize( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec4
		glm::vec4 v;
		lua_getvec4( lua, 1, v );

		// set result
		v = glm::normalize( v );
		lua_setvec4( lua, 1, v );

		return 0;
	}

	int vec4::distance( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		// get b
		glm::vec4 b;
		lua_getvec4( lua, 2, b );

		// push result
		float dist = glm::distance( a, b );
		lua_pushnumber( lua, dist );

		return 1;
	}

	int vec4::direction( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );

		// get a
		glm::vec4 a;
		lua_getvec4( lua, 1, a );

		// get b
		glm::vec4 b;
		lua_getvec4( lua, 2, b );

		// set result
		glm::vec4 dir = glm::normalize( b - a );
		lua_setvec4( lua, 3, dir );

		return 0;
	}

	int vec4::print( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec4 
		glm::vec4 v;
		lua_getvec4( lua, 1, v );

		// print vec4
		LOG_DEBUG( "(%f,%f,%f,%f)", v.x, v.y, v.z, v.w );

		return 0;
	}

	int vec4::tostring( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		// get vec4
		glm::vec4 v;
		lua_getvec4( lua, 1, v );

		// push result
		char buf[32];
		snprintf( buf, 32, "(%f,%f,%f,%f)", v.x, v.y, v.z, v.w );
		lua_pushstring( lua, buf );

		return 1;
	}

	int vec4::copy( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );

		// get vec4
		glm::vec4 v;
		lua_getvec4( lua, 1, v );

		// set result
		lua_setvec4( lua, 2, v);

		return 0;
	}
}