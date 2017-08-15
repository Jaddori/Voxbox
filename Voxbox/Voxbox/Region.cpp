#include "Region.h"

Region::Region()
	: valid( false ), uploaded( false )
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
		chunks[i].upload();
	}

	uploaded = true;
}

void Region::calculateFaces()
{
	for( int i=0; i<REGION_HEIGHT; i++ )
	{
		chunks[i].calculateFaces();
	}

	valid = true;
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

void Region::queueChunks( Graphics& graphics, const Frustum& frustum )
{
	glm::vec3 minPosition = offset;
	glm::vec3 maxPosition = minPosition + glm::vec3( CHUNK_SIZE, CHUNK_SIZE*REGION_HEIGHT, CHUNK_SIZE );

	if( frustum.aabbCollision( minPosition, maxPosition ) )
	{
		maxPosition = minPosition + glm::vec3( CHUNK_SIZE );

		for( int i=0; i<REGION_HEIGHT; i++ )
		{
			if( frustum.aabbCollision( minPosition, maxPosition ) )
			{
				graphics.queueChunk( &chunks[i] );
			}

			minPosition.y += CHUNK_SIZE;
			maxPosition.y += CHUNK_SIZE;
		}
	}
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

bool Region::getValid() const
{
	return valid;
}

bool Region::getUploaded() const
{
	return uploaded;
}