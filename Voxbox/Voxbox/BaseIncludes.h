#pragma once

// Standard libraries
#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>

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