#pragma once

// Standard libraries
#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>
#include <ctime>
#ifdef _WIN32
#include <Windows.h>
#endif

// Windowing
#include "SDL.h"

#define WINDOW_X SDL_WINDOWPOS_UNDEFINED
#define WINDOW_Y SDL_WINDOWPOS_UNDEFINED
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_VIEWPORT glm::vec4( 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT )

// Rendering
#ifdef _WIN32
#include "GL\glew.h"
#else
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#define CAMERA_FOV 45.0f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 100.0f
#define CAMERA_HORIZONTAL_SENSITIVITY 0.01f
#define CAMERA_VERTICAL_SENSITIVITY 0.01f

// Maths
#include "glm.hpp"
#ifdef _WIN32
#include "gtc\type_ptr.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\noise.hpp"
#else
#include "type_ptr.hpp"
#include "matrix_transform.hpp"
#include "noise.hpp"
#endif

const float PI = glm::pi<float>();
const float EPSILON = glm::epsilon<float>();

// Lua
#include "lua.hpp"
#include "LuaUtils.h"

// Core
#include "Log.h"
#include "Array.h"
#include "Queue.h"
#include "Swap.h"
#include "SwapArray.h"
#include "CoreData.h"
#include "Utils.h"

struct Point
{
	int x, y;
};
