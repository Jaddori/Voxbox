#pragma once

#include "Log.h"
#include "glm.hpp"
#include "lua.hpp"

#define LUA_ASSERT_ARGS( expected ) \
	{ \
		int args = lua_gettop( lua ); \
		LOG_ASSERT( args == expected, "Expected " #expected " argument(s) to " __FUNCTION__ ", got %d", args ); \
	}

#define LUA_EXPECT_BOOL( num ) LOG_ASSERT( lua_isboolean( lua, num ), "Expected bool argument #" #num )
#define LUA_EXPECT_NUMBER( num ) LOG_ASSERT( lua_isnumber( lua, num ),"Expected number argument #" #num )
#define LUA_EXPECT_STRING( num ) LOG_ASSERT( lua_isstring( lua, num ),"Expected string argument #" #num )
#define LUA_EXPECT_USERDATA( num ) LOG_ASSERT( lua_isuserdata( lua, num ), "Expected userdata argument #" #num )
#define LUA_EXPECT_TABLE( num ) LOG_ASSERT( lua_istable( lua, num ), "Expected table argument #" #num )

#define lua_tofloat( state, index ) (float)lua_tonumber( state, index )
#define lua_toint( state, index) (int)lua_tonumber( state, index )
#define lua_tobool( state, index ) (bool)lua_toboolean( state, index )

inline float lua_rawfloat( lua_State* lua, int tableIndex, int fieldIndex )
{
	lua_rawgeti( lua, tableIndex, fieldIndex );
	LUA_EXPECT_NUMBER( -1 );
	return lua_tofloat( lua, -1 );
}

inline int lua_rawint( lua_State* lua, int tableIndex, int fieldIndex )
{
	lua_rawgeti( lua, tableIndex, fieldIndex );
	LUA_EXPECT_NUMBER( -1 );
	return lua_toint( lua, -1 );
}

#define lua_rawvec2( state, index, vec ) \
	vec.x = lua_rawfloat( state, index, 1 ); \
	vec.y = lua_rawfloat( state, index, 2 )

#define lua_rawvec3( state, index, vec ) \
	lua_rawvec2( state, index, vec ); \
	vec.z = lua_rawfloat( state, index, 3 )

#define lua_rawvec4( state, index, vec ) \
	lua_rawvec3( state, index, vec ); \
	vec.w = lua_rawfloat( state, index, 4 )

template<typename T>
inline T* getUserdata( lua_State* lua, int index )
{
	lua_getfield( lua, index, "__self" );
	LUA_EXPECT_USERDATA( -1 );
	return (T*)lua_touserdata( lua, -1 );
}