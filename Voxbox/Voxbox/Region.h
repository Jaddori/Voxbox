#pragma once

#include "BaseIncludes.h"
#include "Chunk.h"
#include "Frustum.h"
#include "Graphics.h"

#define REGION_HEIGHT 16

class Region
{
public:
	Region();
	~Region();

	void upload();
	void calculateFaces();
	void noise( int x, int z );

	void queueChunks( Graphics& graphics, const Frustum& frustum );

	void setOffset( const glm::vec3& offset );

	const glm::vec3& getOffset() const;
	bool getValid() const;
	bool getUploaded() const;

private:
	Chunk* chunks;
	glm::vec3 offset;
	bool valid;
	bool uploaded;
};