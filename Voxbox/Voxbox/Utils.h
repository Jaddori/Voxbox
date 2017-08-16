#pragma once

#include "glm.hpp"
#include "gtc\constants.hpp"

bool rayCheck( const glm::vec3& rayStart, const glm::vec3& rayEnd, const glm::vec3& minPosition, const glm::vec3& maxPosition, glm::vec3* hitPoint = nullptr );

float distanceToLine( const glm::vec3& lineStart, const glm::vec3& lineEnd, const glm::vec3& point );

inline bool inside( const glm::vec3& point, const glm::vec3& minPosition, const glm::vec3& maxPosition )
{
	return ( point.x >= minPosition.x &&
		point.y >= minPosition.y &&
		point.z >= minPosition.z &&
		point.x <= maxPosition.x &&
		point.y <= maxPosition.y &&
		point.z <= maxPosition.z );
}

inline void clampInt( int& value, int minValue, int maxValue )
{
	if( value < minValue )
		value = minValue;
	else if( value > maxValue )
		value = maxValue;
}