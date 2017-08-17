#include "LuaCore.h"

namespace LuaCore
{
	void bind( lua_State* lua, CoreData* coreData )
	{
		// metatable for core
		luaL_newmetatable( lua, "coreMeta" );
		luaL_Reg coreRegs[] =
		{
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, coreRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Core" );
	}
}