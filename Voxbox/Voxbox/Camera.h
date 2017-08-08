#pragma once

#include "BaseIncludes.h"

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

	const glm::mat4& getViewMatrix();
	const glm::mat4& getProjectionMatrix() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getDirection() const;

private:
	glm::vec3 position;
	glm::vec3 direction;

	float horizontalAngle;
	float verticalAngle;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	bool dirtyViewMatrix;
};