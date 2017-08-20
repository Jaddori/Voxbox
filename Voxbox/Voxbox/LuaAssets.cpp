#include "LuaAssets.h"
#include "Assets.h"

namespace LuaAssets
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		// meta table for assets
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

		// meta table for fonts
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
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_STRING( 1 );

		int result = 0;

		const char* path = lua_tostring( lua, 1 );
		Texture* texture = g_coreData->assets->loadTexture( path );
		if( texture )
		{
			lua_newtable( lua );
			lua_setuserdata( lua, texture );
			lua_setnumber( lua, "width", texture->getWidth() );
			lua_setnumber( lua, "height", texture->getHeight() );

			result = 1;
		}

		return result;*/

		int result = 0;

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_STRING( 1 ) )
			{
				const char* path = lua_tostring( lua, 1 );
				Texture* texture = g_coreData->assets->loadTexture( path );
				if( texture )
				{
					lua_newtable( lua );
					lua_setuserdata( lua, texture );
					lua_setnumber( lua, "width", texture->getWidth() );
					lua_setnumber( lua, "height", texture->getHeight() );

					result = 1;
				}
			}
		}

		return result;
	}

	int unloadTexture( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				Texture* texture = lua_getuserdata<Texture>( lua, 1 );
				g_coreData->assets->unloadTexture( texture );
			}
		}

		return 0;
	}

	int loadFont( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_STRING( 1 );
		LUA_EXPECT_STRING( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_STRING( 1 ) &&
				LUA_EXPECT_STRING( 2 ) )
			{
				const char* infoPath = lua_tostring( lua, 1 );
				const char* texturePath = lua_tostring( lua, 2 );

				Font* font = g_coreData->assets->loadFont( infoPath, texturePath );
				if( font )
				{
					lua_newtable( lua );
					lua_setuserdata( lua, font );
					lua_setnumber( lua, "height", font->getHeight() );
					luaL_setmetatable( lua, "fontMeta" );

					result = 1;
				}
			}
		}

		return result;
	}

	int unloadFont( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 1 );
		LUA_EXPECT_TABLE( 1 );*/

		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) )
			{
				Font* font = lua_getuserdata<Font>( lua, 1 );
				g_coreData->assets->unloadFont( font );
			}
		}

		return 0;
	}

	int getWidth( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_STRING( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_STRING( 2 ) )
			{
				Font* font = lua_getuserdata<Font>( lua, 1 );
				const char* c = lua_tostring( lua, 2 );
				lua_pushnumber( lua, font->getWidth( c[0] ) );
				result = 1;
			}
		}

		return result;
	}

	int getHorizontalOffset( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_STRING( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 2 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_STRING( 2 ) )
			{
				Font* font = lua_getuserdata<Font>( lua, 1 );
				const char* c = lua_tostring( lua, 2 );
				lua_pushnumber( lua, font->getHorizontalOffset( c[0] ) );
				result = 1;
			}
		}

		return result;
	}

	int getVerticalOffset( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 2 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_STRING( 2 );*/

		int result = 0;
		LUA_EXPECT_ARGS( 1 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_STRING( 2 ) )
			{
				Font* font = lua_getuserdata<Font>( lua, 1 );
				const char* c = lua_tostring( lua, 2 );
				lua_pushnumber( lua, font->getVerticalOffset( c[0] ) );
				result = 1;
			}
		}

		return result;
	}

	int getUV( lua_State* lua )
	{
		/*LUA_ASSERT_ARGS( 3 );
		LUA_EXPECT_TABLE( 1 );
		LUA_EXPECT_STRING( 2 );
		LUA_EXPECT_TABLE( 3 );*/

		LUA_EXPECT_ARGS( 3 )
		{
			if( LUA_EXPECT_TABLE( 1 ) &&
				LUA_EXPECT_STRING( 2 ) &&
				LUA_EXPECT_TABLE( 3 ) )
			{
				Font* font = lua_getuserdata<Font>( lua, 1 );
				const char* c = lua_tostring( lua, 2 );

				glm::vec4 uv = font->getUV( c[0] );

				// set result
				lua_setvec4( lua, 3, uv );
			}
		}

		return 0;
	}
}