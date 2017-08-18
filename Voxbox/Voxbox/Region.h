#pragma once

#include "BaseIncludes.h"
#include "Chunk.h"
#include "Frustum.h"
#include "Graphics.h"
#include "ThreadPool.h"

#define REGION_HEIGHT 6

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

	void load( ThreadPool* threadPool );
	void upload();
	//void calculateFaces();
	void noise( int x, int z );
	void queueWork( ThreadPool* threadPool );

	void loadRegion( FILE* file );
	void saveRegion( FILE* file );

	void queueChunks( CoreData* coreData, const Frustum& frustum );
	float hitBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, ChunkIndex& chunkIndex );

	void setBlock( const ChunkIndex& index, uint8_t value );
	void setOffset( const glm::vec3& offset );

	const glm::vec3& getOffset() const;

private:
	static void calculateChunk( void* args );

	Chunk* chunks;
	glm::vec3 offset;

	ThreadPool* threadPool;
};