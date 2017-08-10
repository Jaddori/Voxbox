#pragma once

#include "BaseIncludes.h"

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
	~Frustum();

	void extractPlanes( const glm::mat4& viewProjection );
	int intersectSphere( const glm::vec3& center, float radius ) const;
	int intersectCube( const glm::vec3& position, float size ) const;

private:
	union
	{
		glm::vec4 planes[6];
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

	float horizontalAngle;
	float verticalAngle;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	bool dirtyViewMatrix;
	bool dirtyFrustum;
};