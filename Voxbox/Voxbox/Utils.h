#pragma once

#include "glm.hpp"
#ifdef _WIN32
#include "gtc\constants.hpp"
#else
#include "constants.hpp"
#endif

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

inline void projectVector( const glm::vec3& onto, const glm::vec3& v, glm::vec3& result )
{
	float ontoLength = glm::length( onto );
	result = ( glm::dot( v, onto ) / ( ontoLength * ontoLength ) ) * onto;
}

enum
{
	UNIT_BYTES = 0,
	UNIT_KILOBYTES = 1024,
	UNIT_MEGABYTES = 1024*1024,
	UNIT_GIGABYTES = 1024*1024*1024
};

inline float convertBytes( int bytes, int newUnit )
{
	return (float)bytes / (float)newUnit;
}

inline float shrinkBytes( int bytes, int& newUnit )
{
	newUnit = UNIT_BYTES;
	if( bytes > UNIT_GIGABYTES )
		newUnit = UNIT_GIGABYTES;
	else if( bytes > UNIT_MEGABYTES )
		newUnit = UNIT_MEGABYTES;
	else if( bytes > UNIT_KILOBYTES )
		newUnit = UNIT_KILOBYTES;
	
	return convertBytes( bytes, newUnit );
}

template<typename T>
inline void swap( T& a, T& b )
{
	T temp = a;
	a = b;
	b = temp;
}
