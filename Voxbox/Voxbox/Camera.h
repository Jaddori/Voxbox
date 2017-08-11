#pragma once

#include "BaseIncludes.h"
#include "DebugShapes.h"
#include "Frustum.h"

class Camera
{
public:
	Camera();
	~Camera();

	void finalize();

	void updatePosition( const glm::vec3& localMovement );
	void updateDirection( int deltaX, int deltaY );
	void updatePerspective( float width, float height );
	void updateOrthographic( float width, float height );

	void setPosition( const glm::vec3& position );
	void setDirection( const glm::vec3& direction );

	const Frustum& getFrustum();
	const glm::vec3& getPosition() const;
	const glm::vec3& getDirection() const;
	const glm::mat4& getViewMatrix();
	const glm::mat4& getProjectionMatrix() const;

	const Frustum& getFinalFrustum() const;
	const glm::vec3& getFinalPosition() const;
	const glm::vec3& getFinalDirection() const;
	const glm::mat4& getFinalViewMatrix() const;
	const glm::mat4& getFinalProjectionMatrix() const;

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

	// final states
	Frustum finalFrustum;

	glm::vec3 finalPosition;
	glm::vec3 finalDirection;
	glm::vec3 finalUp;

	glm::mat4 finalViewMatrix;
	glm::mat4 finalProjectionMatrix;
};