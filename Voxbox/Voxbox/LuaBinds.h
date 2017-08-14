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

	inline void load() { run( loadFunctionReference ); }
	inline void unload() { run( unloadFunctionReference ); }
	inline void update() { run( updateFunctionReference ); }
	inline void render() { run( renderFunctionReference ); }

	bool getValid() const;

private:
	void run( int functionReference );

	lua_State* lua;
	int loadFunctionReference;
	int unloadFunctionReference;
	int updateFunctionReference;
	int renderFunctionReference;
	bool valid;
};