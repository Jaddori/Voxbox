#pragma once

#include "BaseIncludes.h"
#include "DebugShapes.h"

#define FRUSTUM_NUM_PLANES 6
#define FRUSTUM_NUM_CORNERS 8

class Frustum
{
public:
	enum
	{
		OUTSIDE = 0,
		INSIDE,
		INTERSECT,
	};

	Frustum();
	Frustum( const Frustum& ref );
	~Frustum();

	Frustum& operator=( const Frustum& ref );

	void setCameraParameters( float fov, float aspectRatio, float nearDistance, float farDistance );
	void extractPlanes( const glm::mat4& viewProjection );
	void extractCorners( const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up );
	int intersectPoint( const glm::vec3& point ) const;
	int intersectSphere( const glm::vec3& center, float radius ) const;
	int intersectCube( const glm::vec3& position, float size ) const;

	void addDebugLines( DebugShapes& shapes );

	const glm::vec4* getPlanes() const;
	const glm::vec3* getCorners() const;

private:
	union
	{
		glm::vec4 planes[FRUSTUM_NUM_PLANES];
		struct
		{
			glm::vec4 left;
			glm::vec4 right;
			glm::vec4 top;
			glm::vec4 bottom;
			glm::vec4 near;
			glm::vec4 far;
		};
	};
	union
	{
		glm::vec3 corners[FRUSTUM_NUM_CORNERS];
		struct
		{
			glm::vec3 nearTopLeft;
			glm::vec3 nearTopRight;
			glm::vec3 nearBottomLeft;
			glm::vec3 nearBottomRight;

			glm::vec3 farTopLeft;
			glm::vec3 farTopRight;
			glm::vec3 farBottomLeft;
			glm::vec3 farBottomRight;
		};
	};

	float fov, aspectRatio;
	float nearDistance, farDistance;
	float nearWidth, nearHeight;
	float farWidth, farHeight;
};

class Camera
{
public:
	Camera();
	~Camera();

	void updatePosition( const glm::vec3& localMovement );
	void updateDirection( int deltaX, int deltaY );
	void updatePerspective( float width, float height );
	void updateOrthographic( float width, float height );

	void setPosition( const glm::vec3& position );
	void setDirection( const glm::vec3& direction );

	const Frustum& getFrustum();
	const glm::mat4& getViewMatrix();
	const glm::mat4& getProjectionMatrix() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getDirection() const;

private:
	Frustum frustum;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;

	float horizontalAngle;
	float verticalAngle;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	bool dirtyViewMatrix;
	bool dirtyFrustum;
};