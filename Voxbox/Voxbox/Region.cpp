#include "Region.h"

Region::Region()
{
	chunks = new Chunk[REGION_HEIGHT];
}

Region::~Region()
{
	delete[] chunks;
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

void Region::calculateFaces()
{
	for( int i=0; i<REGION_HEIGHT; i++ )
	{
		//if( !chunks[i].getValid() )
		{
			chunks[i].calculateFaces();
		}
	}
}

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

void Region::queueChunks( CoreData* coreData, const Frustum& frustum )
{
	glm::vec3 minPosition = offset * (float)CHUNK_SIZE;
	glm::vec3 maxPosition = minPosition + glm::vec3( CHUNK_SIZE, CHUNK_SIZE*REGION_HEIGHT, CHUNK_SIZE );

	coreData->debugShapes->addAABB( { minPosition, maxPosition, glm::vec4( 1.0f, 0.0f, 1.0f, 1.0f ) } );

	if( frustum.aabbCollision( minPosition, maxPosition ) )
	{
		maxPosition = minPosition + glm::vec3( CHUNK_SIZE );

		for( int i=0; i<REGION_HEIGHT; i++ )
		{
			coreData->debugShapes->addAABB( { minPosition, maxPosition, glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) } );

			if( frustum.aabbCollision( minPosition, maxPosition ) )
			{
				coreData->graphics->queueChunk( &chunks[i] );
				coreData->debugShapes->addSphere( chunks[i].dbg );
			}

			minPosition.y += CHUNK_SIZE;
			maxPosition.y += CHUNK_SIZE;
		}
	}
}

bool Region::hitBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, glm::vec3& location )
{
	bool result = false;

	glm::vec3 minPosition = offset * (float)CHUNK_SIZE;
	glm::vec3 maxPosition = minPosition + glm::vec3( CHUNK_SIZE, CHUNK_SIZE*REGION_HEIGHT, CHUNK_SIZE );

	float minDistance = CAMERA_FAR + 10.0f;

	if( inside( rayStart, minPosition, maxPosition ) ||
		rayCheck( rayStart, rayEnd, minPosition, maxPosition ) )
	{
		maxPosition = minPosition + glm::vec3( CHUNK_SIZE );

		for( int i=0; i<REGION_HEIGHT; i++ )
		{
			if( inside( rayStart, minPosition, maxPosition ) ||
				rayCheck( rayStart, rayEnd, minPosition, maxPosition ) )
			{
				glm::vec3 tempLocation;
				if( chunks[i].hitBlock( rayStart, rayEnd, tempLocation ) )
				{
					float distance = glm::distance( rayStart, tempLocation );
					if( distance < minDistance )
					{
						minDistance = distance;
						location = tempLocation;
					}
					result = true;
				}
			}

			minPosition.y += CHUNK_SIZE;
			maxPosition.y += CHUNK_SIZE;
		}
	}

	return result;
}

bool Region::marchBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, glm::vec3& location )
{
	bool result = false;

	glm::vec3 minPosition = offset * (float)CHUNK_SIZE;
	glm::vec3 maxPosition = minPosition + glm::vec3( CHUNK_SIZE, CHUNK_SIZE*REGION_HEIGHT, CHUNK_SIZE );

	float minDistance = CAMERA_FAR + 10.0f;

	if( inside( rayStart, minPosition, maxPosition ) || 
		rayCheck( rayStart, rayEnd, minPosition, maxPosition ) )
	{
		maxPosition = minPosition + glm::vec3( CHUNK_SIZE );

		for( int i=0; i<REGION_HEIGHT; i++ )
		{
			glm::vec3 tempLocation;
			if( chunks[i].marchBlock( rayStart, rayEnd, tempLocation ) )
			{
				float distance = glm::distance( rayStart, tempLocation );
				if( distance < minDistance )
				{
					distance = minDistance;
					location = tempLocation;
				}

				result = true;
			}

			minPosition.y += CHUNK_SIZE;
			maxPosition.y += CHUNK_SIZE;
		}
	}

	return result;
}

void Region::setOffset( const glm::vec3& o )
{
	offset = o;

	for( int i=0; i<REGION_HEIGHT; i++ )
		chunks[i].setOffset( glm::vec3( offset.x, offset.y + i, offset.z ) );
}

const glm::vec3& Region::getOffset() const
{
	return offset;
}