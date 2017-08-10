#include "Camera.h"

Camera::Camera()
	: position( 0.0f ), direction( 0.0f, 0.0f, 1.0f ),
	dirtyViewMatrix( true ), dirtyFrustum( true ),
	horizontalAngle( 0.0f ), verticalAngle( 0.0f )
{
}

Camera::~Camera()
{
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
		frustum.extractPlanes( vp );

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

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::extractPlanes( const glm::mat4& vp )
{
	// Left clipping plane
	planes[0].x = vp[0][3] + vp[0][0];
	planes[0].y = vp[1][3] + vp[1][0];
	planes[0].z = vp[2][3] + vp[2][0];
	planes[0].w = vp[3][3] + vp[3][0];

	// Right clipping plane
	planes[1].x = vp[0][3] - vp[0][0];
	planes[1].y = vp[1][3] - vp[1][0];
	planes[1].z = vp[2][3] - vp[2][0];
	planes[1].w = vp[3][3] - vp[3][0];

	// Top clipping plane
	planes[2].x = vp[0][3] - vp[0][1];
	planes[2].y = vp[1][3] - vp[1][1];
	planes[2].z = vp[2][3] - vp[2][1];
	planes[2].w = vp[3][3] - vp[3][1];

	// Bottom clipping plane
	planes[3].x = vp[0][3] + vp[0][1];
	planes[3].y = vp[1][3] + vp[1][1];
	planes[3].z = vp[2][3] + vp[2][1];
	planes[3].w = vp[3][3] + vp[3][1];

	// Near clipping plane
	planes[4].x = vp[0][3] + vp[0][2];
	planes[4].y = vp[1][3] + vp[1][2];
	planes[4].z = vp[2][3] + vp[2][2];
	planes[4].w = vp[3][3] + vp[3][2];

	// Far clipping plane
	planes[5].x = vp[0][3] - vp[0][2];
	planes[5].y = vp[1][3] - vp[1][2];
	planes[5].z = vp[2][3] - vp[2][2];
	planes[5].w = vp[3][3] - vp[3][2];

	for( int i=0; i<6; i++ )
		planes[i] = glm::normalize( planes[i] );
}

int Frustum::intersectSphere( const glm::vec3& center, float radius ) const
{
	int result = INSIDE;

	for( int i=0; i<6 && result == INSIDE; i++ )
	{
		glm::vec3 planeNormal( planes[i].x, planes[i].y, planes[i].z );
		float dist = glm::dot( planeNormal, center ) + planes[i].w;

		if( dist < -radius )
			result = OUTSIDE;
	}

	return result;
}

int Frustum::intersectCube( const glm::vec3& position, float radius ) const
{
	glm::vec3 corners[8] =
	{
		position,
		position + glm::vec3( radius, 0.0f, 0.0f ),
		position + glm::vec3( radius, 0.0f, radius ),
		position + glm::vec3( 0.0f, 0.0f, radius ),

		position + glm::vec3( 0.0f, radius, 0.0f ),
		position + glm::vec3( radius, radius, 0.0f ),
		position + glm::vec3( radius, radius, radius ),
		position + glm::vec3( 0.0f, radius, radius ),
	};

	int totalInside = 0;

	for( int i=0; i<6; i++ )
	{
		int inCount = 8;
		int ptIn = 1;

		glm::vec3 planeNormal( planes[i].x, planes[i].y, planes[i].z );
		for( int j=0; j<8; j++ )
		{
			float dist = glm::dot( planeNormal, corners[j] ) + planes[i].w;
			if( dist < 0.0f )
			{
				ptIn = 0;
				inCount--;
			}
		}

		if( inCount == 0 )
			return OUTSIDE;

		totalInside += ptIn;
	}

	if(totalInside == 6 )
		return INTERSECT;
	return INSIDE;
}