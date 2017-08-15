#include "LuaRendering.h"

namespace LuaRendering
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
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

		//Font* font = getFont( lua, 1 );
		Font* font = getUserdata<Font>( lua, 1 );
		const char* text = lua_tostring( lua, 2 );

		LOG_ASSERT( font != nullptr, "Font is nullptr in queueText." );
		LOG_ASSERT( text != nullptr, "Text is nullptr in queueText." );

		// get position
		glm::vec2 position;
		lua_rawgeti( lua, 3, 1 );
		position.x = (float)lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 3, 2 );
		position.y = (float)lua_tonumber( lua, -1 );

		// get color
		glm::vec4 color;
		lua_rawgeti( lua, 4, 1 );
		color.r = (float)lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 4, 2 );
		color.g = (float)lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 4, 3 );
		color.b = (float)lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 4, 4 );
		color.a = (float)lua_tonumber( lua, -1 );

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
		lua_rawgeti( lua, 1, 1 );
		position.x = (float)lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 1, 2 );
		position.y = (float)lua_tonumber( lua, -1 );

		// get size
		glm::vec2 size;
		lua_rawgeti( lua, 2, 1 );
		size.x = (float)lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 2, 2 );
		size.y = (float)lua_tonumber( lua, -1 );

		// get uv
		glm::vec4 uv;
		lua_rawgeti( lua, 3, 1 );
		uv.x = (float)lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 3, 2 );
		uv.y = (float)lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 3, 3 );
		uv.z = (float)lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 3, 4 );
		uv.w = (float)lua_tonumber( lua, -1 );

		// get opacity
		float opacity = (float)lua_tonumber( lua, 4 );

		// get texture
		Texture* texture = nullptr;
		if( args == 5 )
			texture = (Texture*)lua_touserdata( lua, 5 );

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
		lua_rawvec3( lua, 1, position );

		// get size
		glm::vec2 size;
		lua_rawvec2( lua, 2, size );

		// get uv
		glm::vec4 uv;
		lua_rawvec4( lua, 3, uv );

		// get spherical
		bool spherical = lua_tobool( lua, 4 );

		// get texture
		Texture* texture = nullptr;
		if( args == 5 )
			texture = getUserdata<Texture>( lua, 5 );

		g_coreData->graphics->queueBillboard( position, size, uv, spherical, texture );

		return 0;
	}
}