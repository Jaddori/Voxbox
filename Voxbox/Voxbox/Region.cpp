#include "Region.h"

Region::Region()
{
	chunks = new Chunk[REGION_HEIGHT];
}

Region::~Region()
{
	delete[] chunks;
}

void Region::load( ThreadPool* pool )
{
	threadPool = pool;
}

void Region::upload()
{
	for( int i=0; i<REGION_HEIGHT; i++ )
	{
		if( chunks[i].getValid() && !chunks[i].getUploaded() )
		{
			chunks[i].upload();
		}
	}
}

/*void Region::calculateFaces()
{
	for( int i=0; i<REGION_HEIGHT; i++ )
	{
		//if( !chunks[i].getValid() )
		{
			chunks[i].calculateFaces();
		}
	}
}*/

void Region::noise( int noiseX, int noiseZ )
{
	noiseX *= CHUNK_SIZE;
	noiseZ *= CHUNK_SIZE;

	for( int x=0; x<CHUNK_SIZE; x++ )
	{
		for( int z=0; z<CHUNK_SIZE; z++ )
		{
			// between -1 and 1
			float noiseValue = glm::perlin<float>( glm::vec2( noiseX+x, noiseZ+z ) * CHUNK_NOISE_MULTIPLIER );

			// between 0 and 1
			noiseValue = ( noiseValue + 1.0f ) * 0.5f;

			// between minHeight and maxHeight
			int height = (int)(noiseValue * REGION_HEIGHT * CHUNK_SIZE * 0.5f);

			for( int curChunk = 0; curChunk < REGION_HEIGHT; curChunk++ )
			{
				Chunk& chunk = chunks[curChunk];
				for( int y = 0; y < CHUNK_SIZE; y++ )
				{
					int blockHeight = (curChunk*CHUNK_SIZE+y);

					// calculate block type
					uint8_t blockType = 0;
					if( blockHeight < height )
					{
						if( blockHeight < CHUNK_STONE_THRESHOLD )
							blockType = 2;
						else
							blockType = 1;
					}

					chunk.setBlock( x, y, z, blockType );
				}
			}
		}
	}
}

void Region::queueWork( ThreadPool* threadPool )
{
	for( int i=0; i<REGION_HEIGHT; i++ )
	{
		Job job = { calculateChunk, &chunks[i] };
		threadPool->queueWork( job );
	}
}

void Region::loadRegion( FILE* file )
{
	for( int i=0; i<REGION_HEIGHT; i++ )
		chunks[i].loadChunk( file );
}

void Region::saveRegion( FILE* file )
{
	for( int i=0; i<REGION_HEIGHT; i++ )
		chunks[i].saveChunk( file );
}

void Region::queueChunks( CoreData* coreData, const Frustum& frustum )
{
	glm::vec3 minPosition = offset * (float)CHUNK_SIZE;
	glm::vec3 maxPosition = minPosition + glm::vec3( CHUNK_SIZE, CHUNK_SIZE*REGION_HEIGHT, CHUNK_SIZE );

	if( frustum.aabbCollision( minPosition, maxPosition ) )
	{
		maxPosition = minPosition + glm::vec3( CHUNK_SIZE );

		for( int i=0; i<REGION_HEIGHT; i++ )
		{
			coreData->debugShapes->addAABB( { minPosition, maxPosition, glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) } );

			if( chunks[i].getUploaded() && frustum.aabbCollision( minPosition, maxPosition ) )
			{
				coreData->graphics->queueChunk( &chunks[i] );
			}

			minPosition.y += CHUNK_SIZE;
			maxPosition.y += CHUNK_SIZE;
		}
	}
}

float Region::hitBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, ChunkIndex& chunkIndex )
{
	float result = -1.0f;

	glm::vec3 minPosition = offset * (float)CHUNK_SIZE;
	glm::vec3 maxPosition = minPosition + glm::vec3( CHUNK_SIZE, CHUNK_SIZE*REGION_HEIGHT, CHUNK_SIZE );

	float minDistance = CAMERA_FAR + 10.0f;

	if( inside( rayStart, minPosition, maxPosition ) || 
		rayCheck( rayStart, rayEnd, minPosition, maxPosition ) )
	{
		for( int i=0; i<REGION_HEIGHT; i++ )
		{
			BlockIndex tempIndex = { -1, -1, -1 };
			float distance = chunks[i].hitBlock( rayStart, rayEnd, tempIndex );
			if( distance > 0.0f && distance < minDistance )
			{
				minDistance = distance;
				result = distance;

				chunkIndex.block = tempIndex;
				chunkIndex.chunk = i;
			}
		}
	}

	return result;
}

void Region::setBlock( const ChunkIndex& index, uint8_t value )
{
	LOG_ASSERT( index.chunk >= 0 && index.chunk < REGION_HEIGHT, "Chunk index out of range." );

	chunks[index.chunk].setBlock( index.block, value );
	if( chunks[index.chunk].getDirty() )
	{
		Job job = { calculateChunk, &chunks[index.chunk] };
		threadPool->queueWork( job );
	}
}

void Region::setOffset( const glm::vec3& o )
{
	offset = o;

	for( int i=0; i<REGION_HEIGHT; i++ )
		chunks[i].setOffset( glm::vec3( offset.x, offset.y + i, offset.z ) );
}

int Region::getHeight( int x, int z ) const
{
	int result = -1;

	for( int chunkIndex=REGION_HEIGHT-1; chunkIndex>=0 && result < 0; chunkIndex-- )
	{
		for( int y = CHUNK_SIZE-1; y >= 0 && result < 0; y-- )
		{
			if( chunks[chunkIndex].getBlock( x, y, z ) > 0 )
				result = chunkIndex * CHUNK_SIZE + y;
		}
	}

	return result;
}

const glm::vec3& Region::getOffset() const
{
	return offset;
}

void Region::calculateChunk( void* args )
{
	Chunk* chunk = (Chunk*)args;

	chunk->calculateFaces();
}