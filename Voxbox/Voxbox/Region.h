#pragma once

#include "BaseIncludes.h"
#include "Chunk.h"
#include "Frustum.h"
#include "Graphics.h"

#define REGION_HEIGHT 16

struct ChunkIndex
{
	BlockIndex block;
	int chunk;
};

class Region
{
public:
	Region();
	~Region();

	void upload();
	void calculateFaces();
	void noise( int x, int z );

	void queueChunks( CoreData* coreData, const Frustum& frustum );
	float hitBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, ChunkIndex& chunkIndex );

	void setOffset( const glm::vec3& offset );

	const glm::vec3& getOffset() const;

private:
	Chunk* chunks;
	glm::vec3 offset;
};