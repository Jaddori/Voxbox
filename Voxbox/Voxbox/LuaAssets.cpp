#include "LuaAssets.h"

namespace LuaAssets
{
	static CoreData* g_coreData;

	void bind( lua_State* lua, CoreData* coreData )
	{
		g_coreData = coreData;
	}

	int loadFont( lua_State* lua )
	{
		return 0;
	}

	int loadTexture( lua_State* lua )
	{
		return 0;
	}
}