#include "LuaRendering.h"

namespace LuaRendering
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		lua_register( lua, "queueChunk", queueChunk );
		lua_register( lua, "queueText", queueText );
		lua_register( lua, "queueQuad", queueQuad );
		lua_register( lua, "queueBillboard", queueBillboard );

		g_coreData = coreData;
	}

	int queueChunk( lua_State* lua )
	{
		return 0;
	}

	int queueText( lua_State* lua )
	{
		LOG_ASSERT( lua_gettop( lua ) == 4, "LuaRendering.cpp - Bad arguments to queueText." );

		Font* font = (Font*)lua_touserdata( lua, 1 );
		const char* text = lua_tostring( lua, 2 );

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
		LOG_ASSERT( args >= 4, "LuaRendering.cpp - Bad arguments to queueQuad." );

		// get position
		glm::vec2 position;
		lua_rawgeti( lua, 1, 1 );
		position.x = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 1, 2 );
		position.y = lua_tonumber( lua, -1 );

		// get uv
		glm::vec4 uv;
		lua_rawgeti( lua, 2, 1 );
		uv.x = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 2, 2 );
		uv.y = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 2, 3 );
		uv.z = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 2, 4 );
		uv.w = lua_tonumber( lua, -1 );

		// get size
		glm::vec2 size;
		lua_rawgeti( lua, 3, 1 );
		size.x = lua_tonumber( lua, -1 );
		lua_rawgeti( lua, 3, 2 );
		size.y = lua_tonumber( lua, -1 );

		// get opacity
		float opacity = lua_tonumber( lua, 4 );

		// get texture
		Texture* texture = nullptr;
		if( args == 5 )
			texture = (Texture*)lua_touserdata( lua, 5 );

		g_coreData->graphics->queueQuad( position, uv, size, opacity, texture );

		return 0;
	}

	int queueBillboard( lua_State* lua )
	{
		return 0;
	}
}