#include "World.h"

bool compareBlockDistance( const BlockNode& a, const BlockNode& b )
{
	return ( b.distance + b.hueristic < a.distance + a.hueristic );
}

bool compareBlockEquality( const BlockNode&a, const BlockNode& b )
{
	return ( a.x == b.x && a.y == b.y && a.z == b.z );
}

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
			regions[index].load( threadPool );
			regions[index].setOffset( glm::vec3( x, 0.0f, z ) );
			regions[index].noise( x, z );
			regions[index].queueWork( threadPool );
		}
	}

	nodes = new BlockNode[WORLD_WIDTH*CHUNK_SIZE*WORLD_DEPTH*CHUNK_SIZE];
}

void World::unload()
{
	delete[] regions;
	regions = nullptr;

	delete[] nodes;
	nodes = nullptr;
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

bool World::findPath( const BlockIndex& worldStart, const BlockIndex& worldEnd, Array<BlockIndex>& path )
{
	glm::vec3 worldPos( worldEnd.x, worldEnd.y, worldEnd.z );

	for( int regionX = 0; regionX<WORLD_WIDTH; regionX++ )
	{
		for( int regionZ=0; regionZ<WORLD_DEPTH; regionZ++ )
		{
			int regionIndex = regionX * WORLD_DEPTH + regionZ;

			for( int blockX = 0; blockX < CHUNK_SIZE; blockX++ )
			{
				for( int blockZ = 0; blockZ < CHUNK_SIZE; blockZ++ )
				{
					int xoffset = regionX * CHUNK_SIZE;
					int zoffset = regionZ * CHUNK_SIZE;
					int mapIndex = blockZ * WORLD_WIDTH * CHUNK_SIZE +
									xoffset + blockX +
									zoffset * WORLD_WIDTH * CHUNK_SIZE;

					nodes[mapIndex].x = xoffset + blockX;
					nodes[mapIndex].y = regions[regionIndex].getHeight( blockX, blockZ );
					nodes[mapIndex].z = zoffset + blockZ;

					glm::vec3 blockPos( nodes[mapIndex].x, nodes[mapIndex].y, nodes[mapIndex].z );

					nodes[mapIndex].hueristic = glm::distance( blockPos, worldPos );
					nodes[mapIndex].distance = -1.0f;
					nodes[mapIndex].px = -1;
					nodes[mapIndex].pz = -1;
				}
			}
		}
	}

	Array<BlockNode> openList;
	Array<BlockNode> closedList;

	BlockNode startNode = node( worldStart.x, worldStart.z );
	startNode.distance = 0.0f;
	startNode.px = -1;
	startNode.pz = -1;

	openList.add( startNode );

	BlockNode endNode = node( worldEnd.x, worldEnd.z );
	
	BlockNode curNode;
	bool foundPath = false;
	while( openList.getSize() > 0 && !foundPath )
	{
		curNode = openList[0];
		openList.removeAt( 0 );
		closedList.add( curNode );

		foundPath = ( curNode.x == endNode.x && curNode.z == endNode.z );

		/*if( curNode.x != endNode.x ||
			curNode.z != endNode.z )*/
		if( !foundPath )
		{
			// check left
			if( curNode.x > 0 )
			{
				BlockNode& leftNode = node( curNode.x-1, curNode.z );

				bool isClosed = closedList.contains( leftNode, compareBlockEquality );
				bool isAlreadyOpen = openList.contains( leftNode, compareBlockEquality );
				bool isLowEnough = ( leftNode.y <= curNode.y +1 );

				if( !isClosed && !isAlreadyOpen && isLowEnough )
				{
					float distance = curNode.distance + 1.0f;

					leftNode.distance = distance;
					leftNode.px = curNode.x;
					leftNode.pz = curNode.z;

					openList.add( leftNode );
				}
			}

			// check right
			if( curNode.x < WORLD_WIDTH*CHUNK_SIZE )
			{
				BlockNode& rightNode = node( curNode.x+1, curNode.z );
				if( !closedList.contains( rightNode, compareBlockEquality ) &&
					!openList.contains( rightNode, compareBlockEquality ) &&
					rightNode.y <= curNode.y+1 )
				{
					float distance = curNode.distance + 1.0f;

					rightNode.distance = distance;
					rightNode.px = curNode.x;
					rightNode.pz = curNode.z;

					openList.add( rightNode );
				}
			}

			// check backward
			if( curNode.z > 0 )
			{
				BlockNode& backNode = node( curNode.x, curNode.z-1 );
				if( !closedList.contains( backNode, compareBlockEquality ) &&
					!openList.contains( backNode, compareBlockEquality ) &&
					backNode.y <= curNode.y+1 )
				{
					float distance = curNode.distance + 1.0f;

					backNode.distance = distance;
					backNode.px = curNode.x;
					backNode.pz = curNode.z;

					openList.add( backNode );
				}
			}

			// check forward
			if( curNode.z < WORLD_DEPTH*CHUNK_SIZE )
			{
				BlockNode& forwardNode = node( curNode.x, curNode.z+1 );
				if( !closedList.contains( forwardNode, compareBlockEquality ) &&
					!openList.contains( forwardNode, compareBlockEquality ) &&
					forwardNode.y <= curNode.y+1 )
				{
					float distance = curNode.distance + 1.0f;

					forwardNode.distance = distance;
					forwardNode.px = curNode.x;
					forwardNode.pz = curNode.z;

					openList.add( forwardNode );
				}
			}

			openList.sort( compareBlockDistance );
		}
	}

	// reconstruct path
	if( foundPath )
	{
		while( curNode.px >= 0 && curNode.pz >= 0 )
		{
			path.add( { curNode.x, curNode.y, curNode.z } );
			curNode = node( curNode.px, curNode.pz );
		}
	}

	return foundPath;
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

void World::setBlock( const RegionIndex& index, uint8_t value )
{
	LOG_ASSERT( index.region >= 0 && index.region < WORLD_REGIONS, "Region index out of range." );

	regions[index.region].setBlock( index.chunkIndex, value );
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