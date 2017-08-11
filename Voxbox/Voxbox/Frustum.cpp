#include "Frustum.h"

FrustumPlane::FrustumPlane()
{
}

FrustumPlane::FrustumPlane( const FrustumPlane& ref )
	: normal( ref.normal ), point( ref.point ), d( ref.d )
{
}

FrustumPlane::~FrustumPlane()
{
}

FrustumPlane& FrustumPlane::operator=( const FrustumPlane& ref )
{
	normal = ref.normal;
	point = ref.point;
	d = ref.d;

	return *this;
}

void FrustumPlane::setPlane3Points(const glm::vec3& first, const glm::vec3& second, const glm::vec3& third)
{
	glm::vec3 helper1, helper2;
	helper1 = first - second; // doing vectors from (2->1) and (2->3)
	helper2 = third - second;

	normal = glm::normalize( glm::cross( helper2, helper1 ) );

	point = second;

	d = -( glm::dot( normal, point ) );
}

void FrustumPlane::setNormalAndPoint( const glm::vec3& n, const glm::vec3& p )
{
	normal = glm::normalize( n );
	point = p;
	d = -( glm::dot( n, p ) );
}

float FrustumPlane::distance( const glm::vec3& point ) const
{
	return (d + glm::dot(normal, point));
}

const glm::vec3& FrustumPlane::getNormal() const
{
	return normal;
}

Frustum::Frustum()
{
}

Frustum::Frustum( const Frustum& ref )
	: nearTopLeft( ref.nearTopLeft ), nearTopRight( ref.nearTopRight ),
	nearBottomRight( ref.nearBottomRight ), nearBottomLeft( ref.nearBottomLeft ),
	farTopLeft( ref.farTopLeft ), farTopRight( ref.farTopRight ),
	farBottomRight( ref.farBottomRight ), farBottomLeft( ref.farBottomLeft ),
	nearDistance( ref.nearDistance ), farDistance( ref.farDistance ),
	aspectRatio( ref.aspectRatio ), fov( ref.fov ), tang( ref.tang ),
	nearWidth( ref.nearWidth ), nearHeight( ref.nearHeight ),
	farWidth( ref.farWidth ), farHeight( ref.farHeight )
{
	for( int i=0; i<6; i++ )
	{
		planes[i] = ref.planes[i];
	}
}

Frustum::~Frustum()
{
}

Frustum& Frustum::operator=( const Frustum& ref )
{
	nearTopLeft = ref.nearTopLeft;
	nearTopRight = ref.nearTopRight;
	nearBottomRight = ref.nearBottomRight;
	nearBottomLeft = ref.nearBottomLeft;

	farTopLeft = ref.farTopLeft;
	farTopRight = ref.farTopRight;
	farBottomRight = ref.farBottomRight;
	farBottomLeft = ref.farBottomLeft;

	nearDistance = ref.nearDistance;
	farDistance = ref.farDistance;
	aspectRatio = ref.aspectRatio;
	fov = ref.fov;
	tang = ref.tang;

	nearWidth = ref.nearWidth;
	nearHeight = ref.nearHeight;

	farWidth = ref.farWidth;
	farHeight = ref.farHeight;

	for( int i=0; i<6; i++ )
	{
		planes[i] = ref.planes[i];
	}

	return *this;
}

void Frustum::setCameraParameters( float fv, float ar, float nd, float fd )
{
	fov = fv;
	aspectRatio = ar;
	nearDistance = nd;
	farDistance = fd;

	tang = glm::tan( fov*glm::radians<float>( 1.0f ) * 0.5f );
	nearHeight = nd * tang;
	nearWidth = nearHeight * aspectRatio;
	farHeight = fd * tang;
	farWidth = farHeight * aspectRatio;
}

void Frustum::updateFrustum( const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up )
{
	glm::vec3 nearCenter, farCenter, x, y, z;

	//compute z axis of camera, opposite direction from the looking direction
	z = -direction;
	//z = glm::normalize(glm::vec3(-direction.x, 0, -direction.z));

	// x axis of camera with given up vector and z axis
	x = glm::normalize(glm::cross(up, z));

	//y the real "up vector" is the cross product of Z and X
	y = glm::normalize(glm::cross(z, x));

	// compute near and far planes
	nearCenter = position - (z * nearDistance);
	farCenter = position - (z * farDistance);

	//compute 4 corners of near plane
	nearTopLeft = nearCenter + (y * nearHeight) - (x * nearWidth);
	nearTopRight = nearCenter + (y * nearHeight) + (x * nearWidth);
	nearBottomLeft = nearCenter - (y * nearHeight) - (x * nearWidth);
	nearBottomRight = nearCenter - (y * nearHeight) + (x * nearWidth);

	// compute 4 corners of far plane
	farTopLeft = farCenter + (y * farHeight) - (x * farWidth);
	farTopRight = farCenter + (y * farHeight) + (x * farWidth);
	farBottomLeft = farCenter - (y * farHeight) - (x * farWidth);
	farBottomRight = farCenter - (y * farHeight) + (x * farWidth);

	// compute all planes
	planes[FRUSTUM_TOP].setPlane3Points(nearTopRight, nearTopLeft, farTopLeft);
	planes[FRUSTUM_BOTTOM].setPlane3Points(nearBottomLeft, nearBottomRight, farBottomRight);
	planes[FRUSTUM_LEFT].setPlane3Points(nearTopLeft, nearBottomLeft, farBottomLeft);
	planes[FRUSTUM_RIGHT].setPlane3Points(nearBottomRight, nearTopRight, farBottomRight);
	planes[FRUSTUM_NEAR].setPlane3Points(nearTopLeft, nearTopRight, nearBottomRight);
	planes[FRUSTUM_FAR].setPlane3Points(farTopRight, farTopLeft, farBottomLeft);
}

bool Frustum::pointCollision( glm::vec3& point ) const
{
	bool collision = true;

	for( int i = 0; i < FRUSTUM_PLANE_AMOUNT && collision; i++ )
	{
		float distance = planes[i].distance( point );
		collision = ( planes[i].distance(point) >= 0.0f );
	}

	return collision;
}

bool Frustum::aabbCollision( const glm::vec3& minPosition, const glm::vec3& maxPosition ) const
{
	bool collision = true; // box inside frustum

	for( int i = 0; i < FRUSTUM_PLANE_AMOUNT && collision; i++ )
	{
		if( planes[i].distance( getPositiveVertex( minPosition, maxPosition, planes[i].getNormal() ) ) < 0 )
			collision = false; // outside
	}
	return collision;
}

void Frustum::addDebugLines( DebugShapes& debugShapes ) const
{
	DebugLine line;
	line.color = glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f );

	// near
	line.start = nearTopLeft;
	line.end = nearTopRight;
	debugShapes.addLine( line );

	line.start = nearBottomRight;
	debugShapes.addLine( line );

	line.end = nearBottomLeft;
	debugShapes.addLine( line );

	line.start = nearTopLeft;
	debugShapes.addLine( line );

	// far
	line.start = farTopLeft;
	line.end = farTopRight;
	debugShapes.addLine( line );

	line.start = farBottomRight;
	debugShapes.addLine( line );

	line.end = farBottomLeft;
	debugShapes.addLine( line );

	line.start = farTopLeft;
	debugShapes.addLine( line );

	// connectors
	line.start = nearTopLeft;
	line.end = farTopLeft;
	debugShapes.addLine( line );

	line.start = nearTopRight;
	line.end = farTopRight;
	debugShapes.addLine( line );

	line.start = nearBottomLeft;
	line.end = farBottomLeft;
	debugShapes.addLine( line );

	line.start = nearBottomRight;
	line.end = farBottomRight;
	debugShapes.addLine( line );
}

bool Frustum::pointPlaneCollision( int plane, const glm::vec3& point ) const
{
	bool collision = true;
	float distance = planes[plane].distance(point);
	if( planes[plane].distance(point) < 0 )
	{
		collision = false;
	}
	return collision;
}

glm::vec3 Frustum::getPositiveVertex( const glm::vec3& minPosition, const glm::vec3& maxPosition,const glm::vec3& normal ) const
{
	glm::vec3 positiveVertex = minPosition;

	if (normal.x >= 0)
		positiveVertex.x = maxPosition.x;
	if (normal.y >= 0)
		positiveVertex.y = maxPosition.y;
	if (normal.z >= 0)
		positiveVertex.z = maxPosition.z;

	return positiveVertex;
}

glm::vec3 Frustum::getNegativeVertex( const glm::vec3& minPosition, const glm::vec3& maxPosition, const glm::vec3& normal ) const
{
	glm::vec3 negativeVertex = maxPosition;

	if (normal.x >= 0)
		negativeVertex.x = minPosition.x;
	if (normal.y >= 0)
		negativeVertex.y = minPosition.y;
	if (normal.z >= 0)
		negativeVertex.z = minPosition.z;

	return negativeVertex;
}