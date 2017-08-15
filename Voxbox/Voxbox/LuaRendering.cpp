#include "LuaRendering.h"

namespace LuaRendering
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		// metatable for graphics
		luaL_newmetatable( lua, "graphicsMeta" );
		luaL_Reg graphicsRegs[] =
		{
			{ "queueChunk",			queueChunk },
			{ "queueText",			queueText },
			{ "queueQuad",			queueQuad },
			{ "queueBillboard",		queueBillboard },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, graphicsRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Graphics" );

		g_coreData = coreData;
	}

	int queueChunk( lua_State* lua )
	{
		return 0;
	}

	int queueText( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 4 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_STRING( 2 );
		LUA_EXPECT_TABLE( 3 );
		LUA_EXPECT_TABLE( 4 );

		Font* font = lua_getuserdata<Font>( lua, 1 );
		const char* text = lua_tostring( lua, 2 );

		LOG_ASSERT( font != nullptr, "Font is nullptr." );
		LOG_ASSERT( text != nullptr, "Text is nullptr." );

		// get position
		glm::vec2 position;
		lua_getvec2( lua, 3, position );

		// get color
		glm::vec4 color;
		lua_getvec4( lua, 4, color );

		g_coreData->graphics->queueText( font, text, position, color );

		return 0;
	}

	int queueQuad( lua_State* lua )
	{
		int args = lua_gettop( lua );
		LOG_ASSERT( args >= 4, "Expected at least 4 arguments to queueQuad." );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );
		LUA_EXPECT_NUMBER( 4 );
		if( args == 5 )
			LUA_EXPECT_TABLE( 5 );

		// get position
		glm::vec2 position;
		lua_getvec2( lua, 1, position );

		// get size
		glm::vec2 size;
		lua_getvec2( lua, 2, size );

		// get uv
		glm::vec4 uv;
		lua_getvec4( lua, 3, uv );

		// get opacity
		float opacity = lua_tofloat( lua, 4 );

		// get texture
		Texture* texture = nullptr;
		if( args == 5 )
			texture = lua_getuserdata<Texture>( lua, 5 );

		g_coreData->graphics->queueQuad( position, size, uv, opacity, texture );

		return 0;
	}

	int queueBillboard( lua_State* lua )
	{
		int args = lua_gettop( lua );
		LOG_ASSERT( args == 4 || args == 5, "Expected at least 4 arguments." );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_TABLE( 2 );
		LUA_EXPECT_TABLE( 3 );
		LUA_EXPECT_BOOL( 4 );
		if( args == 5 )
			LUA_EXPECT_TABLE( 5 );

		// get position
		glm::vec3 position;
		lua_getvec3( lua, 1, position );

		// get size
		glm::vec2 size;
		lua_getvec2( lua, 2, size );

		// get uv
		glm::vec4 uv;
		lua_getvec4( lua, 3, uv );

		// get spherical
		bool spherical = lua_tobool( lua, 4 );

		// get texture
		Texture* texture = nullptr;
		if( args == 5 )
			texture = lua_getuserdata<Texture>( lua, 5 );

		g_coreData->graphics->queueBillboard( position, size, uv, spherical, texture );

		return 0;
	}
}