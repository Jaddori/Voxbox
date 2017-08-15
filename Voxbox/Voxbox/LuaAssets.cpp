#include "LuaAssets.h"
#include "Assets.h"

namespace LuaAssets
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		luaL_newmetatable( lua, "assetsMeta" );
		luaL_Reg assetsRegs[] =
		{
			{ "loadTexture",	loadTexture },
			{ "unloadTexture",	unloadTexture },
			{ "loadFont",		loadFont },
			{ "unloadFont",		unloadFont },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, assetsRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Assets" );

		/*luaL_newmetatable( lua, "textureMeta" );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Texture" );*/

		luaL_newmetatable( lua, "fontMeta" );
		luaL_Reg fontRegs[] =
		{
			{ "getWidth",				getWidth },
			{ "getHorizontalOffset",	getHorizontalOffset },
			{ "getVerticalOffset",		getVerticalOffset },
			{ "getUV",					getUV },
			{ NULL, NULL }
		};
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Font" );

		g_coreData = coreData;
	}

	int loadTexture( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_STRING( 1 );

		int result = 0;

		const char* path = lua_tostring( lua, 1 );
		Texture* texture = g_coreData->assets->loadTexture( path );
		if( texture )
		{
			lua_newtable( lua );
			lua_pushlightuserdata( lua, texture );
			lua_setfield( lua, -2, "__self" );
			lua_pushnumber( lua, texture->getWidth() );
			lua_setfield( lua, -2, "width" );
			lua_pushnumber( lua, texture->getHeight() );
			lua_setfield( lua, -2, "height" );

			result = 1;
		}

		return result;
	}

	int unloadTexture( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		//Texture* texture = getTexture( lua, 1 );
		Texture* texture = getUserdata<Texture>( lua, 1 );
		g_coreData->assets->unloadTexture( texture );

		return 0;
	}

	int loadFont( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_STRING( 1 );
		LUA_EXPECT_STRING( 2 );

		int result = 0;

		const char* infoPath = lua_tostring( lua, 1 );
		const char* texturePath = lua_tostring( lua, 2 );
		Font* font = g_coreData->assets->loadFont( infoPath, texturePath );
		if( font )
		{
			lua_newtable( lua );
			lua_pushlightuserdata( lua, font );
			lua_setfield( lua, -2, "__self" );
			lua_pushnumber( lua, font->getHeight() );
			lua_setfield( lua, -2, "height" );
			luaL_setmetatable( lua, "fontMeta" );

			result = 1;
		}

		return result;
	}

	int unloadFont( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );

		//Font* font = getFont( lua, 1 );
		Font* font = getUserdata<Font>( lua, 1 );
		g_coreData->assets->unloadFont( font );

		return 0;
	}

	int getWidth( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_STRING( 1 );

		//Font* font = getFont( lua, 1 );
		Font* font = getUserdata<Font>( lua, 1 );
		const char* c = lua_tostring( lua, 2 );
		lua_pushnumber( lua, font->getWidth( c[0] ) );

		return 1;
	}

	int getHorizontalOffset( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_STRING( 2 );

		//Font* font = getFont( lua, 1 );
		Font* font = getUserdata<Font>( lua, 1 );
		const char* c = lua_tostring( lua, 2 );
		lua_pushnumber( lua, font->getHorizontalOffset( c[0] ) );

		return 1;
	}

	int getVerticalOffset( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_STRING( 2 );

		//Font* font = getFont( lua, 1 );
		Font* font = getUserdata<Font>( lua, 1 );
		const char* c = lua_tostring( lua, 2 );
		lua_pushnumber( lua, font->getVerticalOffset( c[0] ) );

		return 1;
	}

	int getUV( lua_State* lua )
	{
		LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_STRING( 2 );

		//Font* font = getFont( lua, 1 );
		Font* font = getUserdata<Font>( lua, 1 );
		const char* c = lua_tostring( lua, 2 );

		glm::vec4 uv = font->getUV( c[0] );
		lua_newtable( lua );
		lua_pushnumber( lua, uv.x );
		lua_rawseti( lua, -2, 1 );
		lua_pushnumber( lua, uv.y );
		lua_rawseti( lua, -2, 2 );
		lua_pushnumber( lua, uv.z );
		lua_rawseti( lua, -2, 3 );
		lua_pushnumber( lua, uv.w );
		lua_rawseti( lua, -2, 4 );

		return 1;
	}
}