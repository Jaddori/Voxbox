#include "Chunk.h"

Chunk::Chunk()
{
#if 0
	memset( blocks, 1, CHUNK_VOLUME );
#else
	for( int i=0; i<CHUNK_VOLUME; i++ )
	{
		blocks[i] = rand() % 2 + 1;
	}
#endif
}

Chunk::~Chunk()
{
}

void Chunk::calculatePositions()
{
	activeBlocks = 0;
	for( int y=0; y<CHUNK_SIZE; y++ )
	{
		for( int z=0; z<CHUNK_SIZE; z++ )
		{
			for( int x=0; x<CHUNK_SIZE; x++ )
			{
				uint8_t type = blocks[at( x, y, z )];
				if( type > 0 )
				{
					bool vacantNeighbours = false;
					if( x == 0 || blocks[at( x-1, y, z )] == 0 )
						vacantNeighbours = true;
					else if( x == CHUNK_SIZE-1 || blocks[at( x+1, y, z )] == 0 )
						vacantNeighbours = true;
					else if( y == 0 || blocks[at( x, y-1, z )] == 0 )
						vacantNeighbours = true;
					else if( y == CHUNK_SIZE-1 || blocks[at( x, y+1, z )] == 0 )
						vacantNeighbours = true;
					else if( z == 0 || blocks[at( x, y, z-1 )] == 0 )
						vacantNeighbours = true;
					else if( z == CHUNK_SIZE-1 || blocks[at( x, y, z+1 )] == 0 )
						vacantNeighbours = true;

					if( vacantNeighbours )
					{
						positions[activeBlocks++] = glm::vec4( x, y, z, type-1 );
					}
				}
			}
		}
	}

	LOG( VERBOSITY_INFORMATION, "Chunk.cpp - Calculating positions. Active blocks: %d.", activeBlocks );
}

void Chunk::setOffset( const glm::vec3& o )
{
	offset = o;
}

const uint8_t* Chunk::getBlocks() const
{
	return blocks;
}

const glm::vec4* Chunk::getPositions() const
{
	return positions;
}

const glm::vec3& Chunk::getOffset() const
{
	return offset;
}

int Chunk::getActiveBlocks() const
{
	return activeBlocks;
}