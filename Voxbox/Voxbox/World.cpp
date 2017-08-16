#include "World.h"

World::World()
{
}

World::~World()
{
	unload();
}

void World::load()
{
	regions = new Region[WORLD_REGIONS];

	for( int x=0; x<WORLD_WIDTH; x++ )
	{
		for( int z=0; z<WORLD_DEPTH; z++ )
		{
			regions[x*WORLD_DEPTH+z].setOffset( glm::vec3( x, 0.0f, z ) );
			regions[x*WORLD_DEPTH+z].noise( x, z );
		}
	}
}

void World::unload()
{
	delete[] regions;
	regions = nullptr;
}

void World::upload()
{
	LOG_ASSERT( regions != nullptr, "Regions have not been allocated." );

	for( int i=0; i<WORLD_REGIONS; i++ )
	{
		regions[i].upload();
	}
}

void World::calculateFaces()
{
	LOG_ASSERT( regions != nullptr, "Regions have not been allocated." );

	for( int i=0; i<WORLD_REGIONS; i++ )
	{
		regions[i].calculateFaces();
	}
}

void World::queueChunks( CoreData* coreData, const Frustum& frustum )
{
	for( int i=0; i<WORLD_REGIONS; i++ )
	{
		regions[i].queueChunks( coreData, frustum );
	}
}

bool World::hitBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, glm::vec3& location )
{
	bool result = false;

	for( int i=0; i<WORLD_REGIONS && !result; i++ )
	{
		result = regions[i].hitBlock( rayStart, rayEnd, location );
	}

	return result;
}

Region& World::getRegion( int x, int z )
{
	LOG_ASSERT( x >= 0 && x < WORLD_WIDTH, "x index out of range." );
	LOG_ASSERT( z >= 0 && z < WORLD_DEPTH, "z index out of range." );

	return regions[x*WORLD_DEPTH+z];
}

Region* World::getRegions()
{
	return regions;
}