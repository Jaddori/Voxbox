#include "Camera.h"

Camera::Camera()
	: position( 0.0f ), direction( 0.0f, 0.0f, 1.0f ), up( 0.0f, 1.0f, 0.0f ),
	dirtyViewMatrix( true ), dirtyFrustum( true ),
	horizontalAngle( 0.0f ), verticalAngle( 0.0f )
{
	frustum.setCameraParameters( CAMERA_FOV, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR );
}

Camera::~Camera()
{
}

void Camera::finalize()
{
	finalFrustum = getFrustum();

	finalPosition = position;
	finalDirection = direction;
	finalUp = up;

	finalViewMatrix = getViewMatrix();
	finalProjectionMatrix = getProjectionMatrix();
}

void Camera::updatePosition( const glm::vec3& localMovement )
{
	// move backwards and forwards
	if( fabs( localMovement.z ) > EPSILON )
	{
		glm::vec3 forward = glm::normalize( direction );
		position += forward * localMovement.z;
	}

	// move left and right
	if( fabs( localMovement.x ) > EPSILON )
	{
		glm::vec3 right( glm::sin( horizontalAngle - PI*0.5f ),
							0.0f,
							glm::cos( horizontalAngle - PI*0.5f ) );
		position += right * localMovement.x;
	}

	// move up and down
	if( fabs( localMovement.y > EPSILON ) )
	{
		glm::vec3 up( 0.0f, 1.0f, 0.0f );
		position += up * localMovement.y;
	}

	dirtyViewMatrix = true;
	dirtyFrustum = true;
}

void Camera::updateDirection( int deltaX, int deltaY )
{
	direction = glm::normalize( direction );

	horizontalAngle += (float)deltaX * CAMERA_HORIZONTAL_SENSITIVITY;
	verticalAngle += (float)deltaY * CAMERA_VERTICAL_SENSITIVITY;

	// clamp angles
	if( horizontalAngle > 2*PI )
		horizontalAngle -= 2*PI;
	else if( horizontalAngle < -2*PI )
		horizontalAngle += 2*PI;

	if( fabs( verticalAngle ) > PI*0.5f )
		verticalAngle = PI*0.5f;

	// calculate new direction
	direction = glm::vec3(
		glm::cos( verticalAngle ) * glm::sin( horizontalAngle ),
		glm::sin( verticalAngle ),
		glm::cos( verticalAngle ) * glm::cos( horizontalAngle )
	);

	// calculate up vector
	glm::vec3 right = glm::vec3(
		glm::sin( horizontalAngle - 3.14f * 0.5f ),
		0,
		glm::cos( horizontalAngle - 3.14f * 0.5f )
	);

	up = glm::cross( right, direction );

	dirtyViewMatrix = true;
	dirtyFrustum = true;
}

void Camera::updatePerspective( float width, float height )
{
	projectionMatrix = glm::perspectiveFov( CAMERA_FOV, width, height, CAMERA_NEAR, CAMERA_FAR );
}

void Camera::updateOrthographic( float width, float height )
{
	projectionMatrix = glm::ortho( 0.0f, width, height, 0.0f );
}

void Camera::setPosition( const glm::vec3& p )
{
	position = p;
	dirtyViewMatrix = true;
	dirtyFrustum = true;
}

void Camera::setDirection( const glm::vec3& d )
{
	direction = d;
	dirtyViewMatrix = true;
	dirtyFrustum = true;
}

const Frustum& Camera::getFrustum()
{
	if( dirtyFrustum )
	{
		glm::mat4 vp = getViewMatrix() * getProjectionMatrix();
		//frustum.extractPlanes( vp );
		//frustum.extractCorners( position, direction, up );
		frustum.updateFrustum( position, direction, up );

		dirtyFrustum = false;
	}

	return frustum;
}

const glm::mat4& Camera::getViewMatrix()
{
	if( dirtyViewMatrix )
	{
		viewMatrix = glm::lookAt( position, position + direction, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		dirtyViewMatrix = false;
	}

	return viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}

const glm::vec3& Camera::getPosition() const
{
	return position;
}

const glm::vec3& Camera::getDirection() const
{
	return direction;
}

const Frustum& Camera::getFinalFrustum() const
{
	return finalFrustum;
}

const glm::vec3& Camera::getFinalPosition() const
{
	return finalPosition;
}

const glm::vec3& Camera::getFinalDirection() const
{
	return finalDirection;
}

const glm::mat4& Camera::getFinalViewMatrix() const
{
	return finalViewMatrix;
}

const glm::mat4& Camera::getFinalProjectionMatrix() const
{
	return finalProjectionMatrix;
}