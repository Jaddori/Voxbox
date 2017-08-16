#pragma once

#include "Region.h"

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

	void load();
	void unload();
	void upload();

	void calculateFaces();
	void queueChunks( CoreData* coreData, const Frustum& frustum );

	bool hitBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, RegionIndex& regionIndex );

	Region& getRegion( int x, int z );
	Region* getRegions();

private:
	Region* regions;
};