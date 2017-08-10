#pragma once

#include "BaseIncludes.h"
#include "Camera.h"
#include "Shader.h"

struct DebugLine
{
	glm::vec3 start;
	glm::vec3 end;
	glm::vec4 color;
};

struct DebugSphere
{
	glm::vec3 position;
	float radius;
	glm::vec4 color;
};

struct DebugAABB
{
	glm::vec3 minPosition;
	glm::vec3 maxPosition;
	glm::vec4 color;
};

struct DebugOBB
{
	glm::vec3 position;
	glm::vec3 xAxis;
	glm::vec3 yAxis;
	glm::vec3 zAxis;
	glm::vec3 extents;
	glm::vec4 color;
};

class DebugShapes
{
public:
	DebugShapes();
	~DebugShapes();

	bool load();
	void upload();
	void unload();

	void render( Camera* camera );
	void finalize();

	void addLine( const DebugLine& line );
	void addSphere( const DebugSphere& sphere );
	void addAABB( const DebugAABB& aabb );
	void addOBB( const DebugOBB& obb );

private:
	Array<DebugLine> lines;
	Array<DebugSphere> spheres;
	Array<DebugAABB> AABBs;
	Array<DebugOBB> OBBs;

	Array<DebugLine> finalLines;
	Array<DebugSphere> finalSpheres;
	Array<DebugAABB> finalAABBs;
	Array<DebugOBB> finalOBBs;

	// line
	Shader lineShader;
	GLint lineProjectionMatrixLocation;
	GLint lineViewMatrixLocation;
	GLuint lineVAO, lineVBO;

	// sphere
	Shader sphereShader;
	GLint sphereProjectionMatrixLocation;
	GLint sphereViewMatrixLocation;
	GLuint sphereVAO, sphereVBO;

	// aabb
	Shader aabbShader;
	GLint aabbProjectionMatrixLocation;
	GLint aabbViewMatrixLocation;
	GLuint aabbVAO, aabbVBO;

	// obb
	Shader obbShader;
	GLint obbProjectionMatrixLocation;
	GLint obbViewMatrixLocation;
	GLuint obbVAO, obbVBO;
};