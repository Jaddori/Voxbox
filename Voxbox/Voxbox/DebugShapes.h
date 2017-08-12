#pragma once

#include "BaseIncludes.h"
#include "Shader.h"

#define DEBUG_SHAPES_MAX_LINES 128
#define DEBUG_SHAPES_MAX_SPHERES 128
#define DEBUG_SHAPES_MAX_AABB 64
#define DEBUG_SHAPES_MAX_OBB 32

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

	void render( const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix );
	void finalize();

	void addLine( const DebugLine& line );
	void addSphere( const DebugSphere& sphere );
	void addAABB( const DebugAABB& aabb );
	void addOBB( const DebugOBB& obb );

private:
	SwapArray<DebugLine> lines;
	SwapArray<DebugSphere> spheres;
	SwapArray<DebugAABB> AABBs;
	SwapArray<DebugOBB> OBBs;

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