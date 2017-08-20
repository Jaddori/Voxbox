#pragma once

#include "BaseIncludes.h"
#include "LuaAssets.h"
#include "Camera.h"
#include "Frustum.h"
#include "Graphics.h"
#include "Shader.h"

namespace LuaRendering
{
	void bind( lua_State* lua, CoreData* coreData );

	int queueChunk( lua_State* lua );
	int queueBlock( lua_State* lua );
	int queueText( lua_State* lua );
	int queueQuad( lua_State* lua );
	int queueBillboard( lua_State* lua );
}