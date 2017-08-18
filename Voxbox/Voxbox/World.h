#pragma once

#include "Region.h"
#include "ThreadPool.h"

#define WORLD_WIDTH 2
#define WORLD_DEPTH 2
#define WORLD_REGIONS (WORLD_WIDTH*WORLD_DEPTH)

struct RegionIndex
{
	ChunkIndex chunkIndex;
	int region;
};

class World
{
public:
	World();
	~World();

	void load( ThreadPool* threadPool );
	void unload();
	void upload();

	void loadWorld( const char* path );
	void saveWorld( const char* path );

	//void calculateFaces();
	void queueChunks( CoreData* coreData, const Frustum& frustum );

	float hitBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, RegionIndex& regionIndex );

	void localToWorld( const RegionIndex& region, BlockIndex& block );
	void worldToLocal( int x, int y, int z, RegionIndex& index );
	inline void worldToLocal( const BlockIndex& block, RegionIndex& index )
	{
		worldToLocal( block.x, block.y, block.z, index );
	}

	void setBlock( const RegionIndex& index, uint8_t value );

	Region& getRegion( int x, int z );
	Region* getRegions();

private:
	Region* regions;
	ThreadPool* threadPool;
};