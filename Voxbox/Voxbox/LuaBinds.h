#pragma once

#include "BaseIncludes.h"
#include "CoreData.h"

#define LUA_MAIN_SCRIPT "./assets/scripts/main.lua"

class LuaBinds
{
public:
	LuaBinds();
	~LuaBinds();

	bool bind( CoreData* coreData );

	void load();
	void unload();
	void update();

	bool getValid() const;

private:
	lua_State* lua;
	int loadFunctionReference;
	int unloadFunctionReference;
	int updateFunctionReference;
	bool valid;
};