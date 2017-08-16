#include "Utils.h"

bool rayCheck( const glm::vec3& rayStart, const glm::vec3& rayEnd, const glm::vec3& minPosition, const glm::vec3& maxPosition )
{
	float epsilon = glm::epsilon<float>();
	float tmin = 0.0f;
	float tmax = std::numeric_limits<float>().max();
	glm::vec3 rayDirection = glm::normalize( rayEnd - rayStart );
	glm::vec3 rayPosition = rayStart;
	glm::vec3 aabbMin = minPosition;
	glm::vec3 aabbMax = maxPosition;

	unsigned int threeSlabs = 3;

	for (unsigned int i = 0; i < threeSlabs; i++)
	{
		if (glm::abs(rayDirection[i]) < epsilon) // Ray is parallell to slab
		{
			if (rayPosition[i] < aabbMin[i] || rayPosition[i] > aabbMax[i]) // No hit if origin not inside slab
				return false;
		}
		else
		{
			// compute intersection t value of ray with near and far plane of slab
			float ood = 1.0f / rayDirection[i];
			float t1 = (aabbMin[i] - rayPosition[i]) * ood;
			float t2 = (aabbMax[i] - rayPosition[i]) * ood;

			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			if (t1 > tmin)
				tmin = t1;

			if (t2 < tmax)
				tmax = t2;

			if (tmin > tmax)
				return false;
		}

	}

	/*float hitdistance = tmin;
	if (tmin < 0)
	hitdistance = tmax;
	glm::vec3 intersectionPoint = rayPosition + (rayDirection * hitdistance);
	ray->hit(intersectionPoint, hitdistance);*/

	return true;
}