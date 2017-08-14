#pragma once

#include "BaseIncludes.h"
#include "CoreData.h"
#include "Font.h"
#include "Texture.h"

namespace LuaAssets
{
	void bind( lua_State* lua, CoreData* coreData );

	// texture
	int loadTexture( lua_State* lua );
	int unloadTexture( lua_State* lua );

	// font
	int loadFont( lua_State* lua );
	int unloadFont( lua_State* lua );
	int getWidth( lua_State* lua );
	int getHorizontalOffset( lua_State* lua );
	int getVerticalOffset( lua_State* lua );
	int getUV( lua_State* lua );

	// helper functions
	Texture* getTexture( lua_State* lua, int index );
	Font* getFont( lua_State* lua, int index );
}