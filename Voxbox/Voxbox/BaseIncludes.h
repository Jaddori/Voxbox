#pragma once

// Standard libraries
#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>
#include <ctime>

// Windowing
#include "SDL.h"

#define WINDOW_X SDL_WINDOWPOS_UNDEFINED
#define WINDOW_Y SDL_WINDOWPOS_UNDEFINED
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// Rendering
#include "GL\glew.h"

#define CAMERA_FOV 45.0f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 100.0f
#define CAMERA_HORIZONTAL_SENSITIVITY 0.01f
#define CAMERA_VERTICAL_SENSITIVITY 0.01f

// Maths
#include "glm.hpp"
#include "gtc\type_ptr.hpp"
#include "gtc\matrix_transform.hpp"

const float PI = glm::pi<float>();
const float EPSILON = glm::epsilon<float>();

// Core
#include "Log.h"
#include "Array.h"