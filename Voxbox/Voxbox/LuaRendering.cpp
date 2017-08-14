#include "LuaRendering.h"

using namespace LuaAssets;

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
		LOG_ASSERT_ARGS( 4 );
		LOG_EXPECT_TABLE( 1 );
		LOG_EXPECT_STRING( 2 );
		LOG_EXPECT_TABLE( 3 );
		LOG_EXPECT_TABLE( 4 );

		Font* font = getFont( lua, 1 );
		const char* text = lua_tostring( lua, 2 );

		LOG_ASSERT( font != nullptr, "Font is nullptr in queueText." );
		LOG_ASSERT( text != nullptr, "Text is nullptr in queueText." );

		// get position
		glm::vec2 position;
		lua_rawgeti( lua, 3, 1 );
		position.x = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 3, 2 );
		position.y = lua_tonumber( lua, -1 );

		// get color
		glm::vec4 color;
		lua_rawgeti( lua, 4, 1 );
		color.r = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 4, 2 );
		color.g = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 4, 3 );
		color.b = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 4, 4 );
		color.a = lua_tonumber( lua, -1 );

		g_coreData->graphics->queueText( font, text, position, color );

		return 0;
	}

	int queueQuad( lua_State* lua )
	{
		int args = lua_gettop( lua );
		LOG_ASSERT( args >= 4, "Expected at least 4 arguments to queueQuad." );
		LOG_EXPECT_TABLE( 1 );
		LOG_EXPECT_TABLE( 2 );
		LOG_EXPECT_TABLE( 3 );
		LOG_EXPECT_NUMBER( 4 );
		if( args == 5 )
			LOG_EXPECT_TABLE( 5 );

		// get position
		glm::vec2 position;
		lua_rawgeti( lua, 1, 1 );
		position.x = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 1, 2 );
		position.y = lua_tonumber( lua, -1 );

		// get size
		glm::vec2 size;
		lua_rawgeti( lua, 2, 1 );
		size.x = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 2, 2 );
		size.y = lua_tonumber( lua, -1 );

		// get uv
		glm::vec4 uv;
		lua_rawgeti( lua, 3, 1 );
		uv.x = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 3, 2 );
		uv.y = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 3, 3 );
		uv.z = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 3, 4 );
		uv.w = lua_tonumber( lua, -1 );

		// get opacity
		float opacity = lua_tonumber( lua, 4 );

		// get texture
		Texture* texture = nullptr;
		if( args == 5 )
			texture = (Texture*)lua_touserdata( lua, 5 );

		g_coreData->graphics->queueQuad( position, size, uv, opacity, texture );

		return 0;
	}

	int queueBillboard( lua_State* lua )
	{
		return 0;
	}
}