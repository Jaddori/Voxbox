#include "World.h"

World::World()
{
}

World::~World()
{
	unload();
}

void World::load( ThreadPool* pool )
{
	threadPool = pool;

	regions = new Region[WORLD_REGIONS];

	for( int x=0; x<WORLD_WIDTH; x++ )
	{
		for( int z=0; z<WORLD_DEPTH; z++ )
		{
			int index = x * WORLD_DEPTH + z;
			regions[index].setOffset( glm::vec3( x, 0.0f, z ) );
			regions[index].noise( x, z );
			regions[index].queueWork( threadPool );
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

void World::loadWorld( const char* path )
{
	LOG_ASSERT( regions != nullptr, "Regions have not been allocated." );

	FILE* file = fopen( path, "rb" );
	LOG_ASSERT( file != NULL, "Failed to open \"%s\".", path );

	LOG_INFO( "Saving world to %s.", path );

	for( int i=0; i<WORLD_REGIONS; i++ )
	{
		regions[i].loadRegion( file );
	}

	fclose( file );
}

void World::saveWorld( const char* path )
{
	LOG_ASSERT( regions != nullptr, "Regions have not been allocated." );

	FILE* file = fopen( path, "wb" );
	LOG_ASSERT( file != NULL, "Failed to open \"%s\".", path );

	LOG_INFO( "Loading world from %s.", path );

	for( int i=0; i<WORLD_REGIONS; i++ )
	{
		regions[i].saveRegion( file );
	}

	fclose( file );
}

void World::queueChunks( CoreData* coreData, const Frustum& frustum )
{
	for( int i=0; i<WORLD_REGIONS; i++ )
	{
		regions[i].queueChunks( coreData, frustum );
	}
}

float World::hitBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, RegionIndex& regionIndex )
{
	float result = -1.0f;

	float minDistance = CAMERA_FAR + 10.0f;
	for( int i=0; i<WORLD_REGIONS; i++ )
	{
		ChunkIndex tempIndex = { -1, -1, -1, -1 };
		float distance = regions[i].hitBlock( rayStart, rayEnd, tempIndex );
		if( distance > 0.0f && distance < minDistance )
		{
			result = distance;
			minDistance = distance;

			regionIndex.chunkIndex = tempIndex;
			regionIndex.region = i;
		}
	}

	return result;
}

void World::localToWorld( const RegionIndex& region, BlockIndex& block )
{
	int regionX = region.region / WORLD_WIDTH;
	int regionZ = region.region % WORLD_DEPTH;

	block.x = region.chunkIndex.block.x + regionX * CHUNK_SIZE;
	block.z = region.chunkIndex.block.z + regionZ * CHUNK_SIZE;

	block.y = region.chunkIndex.block.y + region.chunkIndex.chunk * CHUNK_SIZE;
}

void World::worldToLocal( int x, int y, int z, RegionIndex& index )
{
	LOG_ASSERT( x >= 0 && x < CHUNK_SIZE * WORLD_WIDTH, "X index out of world range." );
	LOG_ASSERT( y >= 0 && y < CHUNK_SIZE * REGION_HEIGHT, "Y index out of world range." );
	LOG_ASSERT( z >= 0 && z < CHUNK_SIZE * WORLD_DEPTH, "Z index out of world range." );

	// transform to chunk index
	int chunkX = x / CHUNK_SIZE;
	int chunkY = y / CHUNK_SIZE;
	int chunkZ = z / CHUNK_SIZE;

	index.region = chunkX*WORLD_DEPTH+chunkZ;
	index.chunkIndex.chunk = chunkY;

	// transform to block index
	index.chunkIndex.block.x = x % CHUNK_SIZE;
	index.chunkIndex.block.y = y % CHUNK_SIZE;
	index.chunkIndex.block.z = z % CHUNK_SIZE;
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