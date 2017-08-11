#pragma once

#include "BaseIncludes.h"
#include "DebugShapes.h"

class FrustumPlane
{
public:
	FrustumPlane();
	FrustumPlane( const FrustumPlane& ref );
	~FrustumPlane();

	FrustumPlane& operator=( const FrustumPlane& ref );

	void setPlane3Points( const glm::vec3& first, const glm::vec3& second, const glm::vec3& third );
	void setNormalAndPoint( const glm::vec3& normal, const glm::vec3& point );
	float distance( const glm::vec3& point ) const;

	const glm::vec3& getNormal() const;

private:
	glm::vec3 normal, point;
	float d;
};

class Frustum
{
public:
	Frustum();
	Frustum( const Frustum& ref );
	~Frustum();

	Frustum& operator=( const Frustum& ref );

	void setCameraParameters( float fov, float aspectRatio, float nearDistance, float farDistance );

	void updateFrustum( const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up );
	bool pointCollision( glm::vec3& point ) const;
	bool aabbCollision( const glm::vec3& minPosition, const glm::vec3& maxPosition ) const;

	void addDebugLines( DebugShapes& debugShapes ) const;

private:
	enum
	{
		FRUSTUM_TOP,
		FRUSTUM_BOTTOM,
		FRUSTUM_LEFT,
		FRUSTUM_RIGHT,
		FRUSTUM_NEAR,
		FRUSTUM_FAR,
		FRUSTUM_PLANE_AMOUNT
	};

	FrustumPlane planes[6];

	//helper functions
	bool pointPlaneCollision(int plane, const glm::vec3& point) const;
	glm::vec3 getPositiveVertex( const glm::vec3& minPosition, const glm::vec3& maxPosition,const glm::vec3& normal ) const;
	glm::vec3 getNegativeVertex( const glm::vec3& minPosition, const glm::vec3& maxPosition,const glm::vec3& normal ) const;
public:

	glm::vec3 nearTopLeft, nearTopRight, nearBottomRight, nearBottomLeft,
		farTopLeft, farTopRight, farBottomRight, farBottomLeft;
	float nearDistance, farDistance, aspectRatio, fov, tang;
	float nearWidth, nearHeight, farWidth, farHeight;
	const float ONE_DEGREE_RADIAN = 3.14159265358979323846f / 180;
};