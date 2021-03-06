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

	void project( const glm::vec3& worldCoordinates, Point& result );
	void unproject( const Point& windowCoordinates, float depth, glm::vec3& result );

	void relativeMovement( const glm::vec3& localMovement );
	void absoluteMovement( const glm::vec3& worldMovement );
	void updateDirection( int deltaX, int deltaY );
	void updatePerspective( float width, float height );
	void updateOrthographic( float width, float height );

	void setPosition( const glm::vec3& position );
	void setDirection( const glm::vec3& direction );
	void setHorizontalAngle( float angle );
	void setVerticalAngle( float angle );

	const Frustum& getFrustum() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getDirection() const;
	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;

	const glm::vec3& getForward() const;
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;

private:
	Swap<Frustum> frustum;

	Swap<glm::vec3> position;
	Swap<glm::vec3> direction;
	Swap<glm::vec3> right;
	Swap<glm::vec3> up;

	float horizontalAngle;
	float verticalAngle;

	Swap<glm::mat4> viewMatrix;
	Swap<glm::mat4> projectionMatrix;

	bool dirtyViewMatrix;
	bool dirtyFrustum;
};