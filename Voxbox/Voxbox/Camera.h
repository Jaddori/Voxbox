#pragma once

#include "BaseIncludes.h"
#include "DebugShapes.h"
#include "Frustum.h"
#include "Swap.h"

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

	const Swap<Frustum>& getFrustum();
	const Swap<glm::vec3>& getPosition() const;
	const Swap<glm::vec3>& getDirection() const;
	const Swap<glm::mat4>& getViewMatrix() const;
	const Swap<glm::mat4>& getProjectionMatrix() const;

private:
	Swap<Frustum> frustum;

	Swap<glm::vec3> position;
	Swap<glm::vec3> direction;
	Swap<glm::vec3> up;

	float horizontalAngle;
	float verticalAngle;

	Swap<glm::mat4> viewMatrix;
	Swap<glm::mat4> projectionMatrix;

	bool dirtyViewMatrix;
	bool dirtyFrustum;
};