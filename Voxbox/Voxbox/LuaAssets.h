#pragma once

#include "BaseIncludes.h"
#include "CoreData.h"
#include "Font.h"
#include "Texture.h"

namespace LuaAssets
{
	void bind( lua_State* lua, CoreData* coreData );

	int loadFont( lua_State* lua );
	int loadTexture( lua_State* lua );
}