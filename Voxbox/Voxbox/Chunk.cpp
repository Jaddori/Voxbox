#include "Chunk.h"

Chunk::Chunk()
	: uniformBuffer( 0 )
{
#if 0
	memset( blocks, 0, CHUNK_VOLUME );
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

void Chunk::upload()
{
	if( uniformBuffer == 0 )
	{
		glGenBuffers( 1, &uniformBuffer );
		glBindBuffer( GL_UNIFORM_BUFFER, uniformBuffer );
		glBindBufferBase( GL_UNIFORM_BUFFER, 0, uniformBuffer );
		glBufferData( GL_UNIFORM_BUFFER, sizeof(glm::vec4)*CHUNK_VOLUME, nullptr, GL_DYNAMIC_DRAW );
	}

	glBindBuffer( GL_UNIFORM_BUFFER, uniformBuffer );
	GLvoid* p = glMapBuffer( GL_UNIFORM_BUFFER, GL_WRITE_ONLY );
	memcpy( p, positions, sizeof(glm::vec4)*activeBlocks );
	glUnmapBuffer( GL_UNIFORM_BUFFER );
}

void Chunk::unload()
{
	if( uniformBuffer > 0 )
	{
		glDeleteBuffers( 1, &uniformBuffer );
	}
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

void Chunk::noise( int x, int z )
{
	for( int blockX = 0; blockX < CHUNK_SIZE; blockX++ )
	{
		for( int blockZ = 0; blockZ < CHUNK_SIZE; blockZ++ )
		{
			float noiseValue = ( glm::perlin( glm::vec2( x+blockX, z+blockZ ) * 0.0137f ) + 1.0f ) * 0.5f;
			int height = (int)( noiseValue * CHUNK_SIZE ) + 1;
			if( height <= 0 )
				height = 1;
			else if( height > CHUNK_SIZE )
				height = CHUNK_SIZE;

			for( int blockY = 0; blockY < height; blockY++ )
			{
				blocks[at( blockX, blockY, blockZ )] = 1;
			}
		}
	}
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

GLuint Chunk::getUniformBuffer() const
{
	return uniformBuffer;
}