#pragma once

#include "glm.hpp"
#include "gtc\constants.hpp"

bool rayCheck( const glm::vec3& rayStart, const glm::vec3& rayEnd, const glm::vec3& minPosition, const glm::vec3& maxPosition );