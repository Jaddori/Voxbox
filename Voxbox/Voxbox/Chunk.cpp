#include "Chunk.h"

Chunk::Chunk()
	: vao( 0 ), vbo( 0 ), ibo( 0 )
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
	if( vao == 0 )
	{
		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );

		glGenBuffers( 1, &vbo );
		glGenBuffers( 1, &ibo );

		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );

		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)) );

		glBindVertexArray( 0 );
	}

	//Vertex vertices[CHUNK_VOLUME*24];
	//GLuint indices[CHUNK_VOLUME*36];

	Vertex* vertices = new Vertex[CHUNK_VOLUME*24];
	GLuint* indices = new GLuint[CHUNK_VOLUME*36];

	int curVertex = 0;
	curIndex = 0;
	activeBlocks = 0;
	for( int y=0; y<CHUNK_SIZE; y++ )
	{
		for( int z=0; z<CHUNK_SIZE; z++ )
		{
			for( int x=0; x<CHUNK_SIZE; x++ )
			{
				uint8_t type = block( x, y, z );
				if( type > 0 )
				{
					// front
					if( z == 0 || block( x, y, z-1 ) == 0 )
					{
						vertices[curVertex].position = glm::vec3( x, y, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex].uv = glm::vec2( 0.1f, 0.05f );

						vertices[curVertex+1].position = glm::vec3( x, y+1.0f, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+1].uv = glm::vec2( 0.1f, 0.0f );

						vertices[curVertex+2].position = glm::vec3( x+1.0f, y, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+2].uv = glm::vec2( 0.15f, 0.05f );

						vertices[curVertex+3].position = glm::vec3( x+1.0f, y+1.0f, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+3].uv = glm::vec2( 0.15f, 0.0f );

						indices[curIndex] = curVertex;
						indices[curIndex+1] = curVertex+1;
						indices[curIndex+2] = curVertex+2;
						indices[curIndex+3] = curVertex+1;
						indices[curIndex+4] = curVertex+3;
						indices[curIndex+5] = curVertex+2;

						curVertex += 4;
						curIndex += 6;
					}

					// back
					if( z == CHUNK_SIZE-1 || block( x, y, z+1 ) == 0 )
					{
						vertices[curVertex].position = glm::vec3( x+1.0f, y, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex].uv = glm::vec2( 0.1f, 0.05f );

						vertices[curVertex+1].position = glm::vec3( x+1.0f, y+1.0f, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+1].uv = glm::vec2( 0.1f, 0.0f );

						vertices[curVertex+2].position = glm::vec3( x, y, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+2].uv = glm::vec2( 0.15f, 0.05f );

						vertices[curVertex+3].position = glm::vec3( x, y+1.0f, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+3].uv = glm::vec2( 0.15f, 0.0f );

						indices[curIndex] = curVertex;
						indices[curIndex+1] = curVertex+1;
						indices[curIndex+2] = curVertex+2;
						indices[curIndex+3] = curVertex+1;
						indices[curIndex+4] = curVertex+3;
						indices[curIndex+5] = curVertex+2;

						curVertex += 4;
						curIndex += 6;
					}

					// left
					if( x == 0 || block( x-1, y, z ) == 0 )
					{
						vertices[curVertex].position = glm::vec3( x, y, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex].uv = glm::vec2( 0.1f, 0.05f );

						vertices[curVertex+1].position = glm::vec3( x, y+1.0f, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+1].uv = glm::vec2( 0.1f, 0.0f );

						vertices[curVertex+2].position = glm::vec3( x, y, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+2].uv = glm::vec2( 0.15f, 0.05f );

						vertices[curVertex+3].position = glm::vec3( x, y+1.0f, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+3].uv = glm::vec2( 0.15f, 0.0f );

						indices[curIndex] = curVertex;
						indices[curIndex+1] = curVertex+1;
						indices[curIndex+2] = curVertex+2;
						indices[curIndex+3] = curVertex+1;
						indices[curIndex+4] = curVertex+3;
						indices[curIndex+5] = curVertex+2;

						curVertex += 4;
						curIndex += 6;
					}

					// right
					if( x == CHUNK_SIZE-1 || block( x+1, y, z ) == 0 )
					{
						vertices[curVertex].position = glm::vec3( x+1.0f, y, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex].uv = glm::vec2( 0.1f, 0.05f );

						vertices[curVertex+1].position = glm::vec3( x+1.0f, y+1.0f, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+1].uv = glm::vec2( 0.1f, 0.0f );

						vertices[curVertex+2].position = glm::vec3( x+1.0f, y, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+2].uv = glm::vec2( 0.15f, 0.05f );

						vertices[curVertex+3].position = glm::vec3( x+1.0f, y+1.0f, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+3].uv = glm::vec2( 0.15f, 0.0f );

						indices[curIndex] = curVertex;
						indices[curIndex+1] = curVertex+1;
						indices[curIndex+2] = curVertex+2;
						indices[curIndex+3] = curVertex+1;
						indices[curIndex+4] = curVertex+3;
						indices[curIndex+5] = curVertex+2;

						curVertex += 4;
						curIndex += 6;
					}

					// top
					if( y == CHUNK_SIZE-1 || block( x, y+1, z-1 ) == 0 )
					{
						vertices[curVertex].position = glm::vec3( x, y+1.0f, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex].uv = glm::vec2( 0.1f, 0.05f );

						vertices[curVertex+1].position = glm::vec3( x, y+1.0f, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+1].uv = glm::vec2( 0.1f, 0.0f );

						vertices[curVertex+2].position = glm::vec3( x+1.0f, y+1.0f, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+2].uv = glm::vec2( 0.15f, 0.05f );

						vertices[curVertex+3].position = glm::vec3( x+1.0f, y+1.0f, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+3].uv = glm::vec2( 0.15f, 0.0f );

						indices[curIndex] = curVertex;
						indices[curIndex+1] = curVertex+1;
						indices[curIndex+2] = curVertex+2;
						indices[curIndex+3] = curVertex+1;
						indices[curIndex+4] = curVertex+3;
						indices[curIndex+5] = curVertex+2;

						curVertex += 4;
						curIndex += 6;
					}

					// bottom
					if( y == 0 || block( x, y-1, z-1 ) == 0 )
					{
						vertices[curVertex].position = glm::vec3( x, y, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex].uv = glm::vec2( 0.1f, 0.05f );

						vertices[curVertex+1].position = glm::vec3( x, y, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+1].uv = glm::vec2( 0.1f, 0.0f );

						vertices[curVertex+2].position = glm::vec3( x+1.0f, y, z+1.0f ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+2].uv = glm::vec2( 0.15f, 0.05f );

						vertices[curVertex+3].position = glm::vec3( x+1.0f, y, z ) + offset * (float)CHUNK_SIZE;
						vertices[curVertex+3].uv = glm::vec2( 0.15f, 0.0f );

						indices[curIndex] = curVertex;
						indices[curIndex+1] = curVertex+1;
						indices[curIndex+2] = curVertex+2;
						indices[curIndex+3] = curVertex+1;
						indices[curIndex+4] = curVertex+3;
						indices[curIndex+5] = curVertex+2;

						curVertex += 4;
						curIndex += 6;
					}
				}
			}
		}
	}

	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(Vertex)*curVertex, vertices, GL_DYNAMIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*curIndex, indices, GL_DYNAMIC_DRAW );
	glBindVertexArray( 0 );

	delete[] vertices;
	delete[] indices;
}

void Chunk::unload()
{
	if( vao > 0 )
	{
		glDeleteVertexArrays( 1, &vao );
		glDeleteBuffers( 1, &vbo );
		glDeleteBuffers( 1, &ibo );
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

void Chunk::render()
{
	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLES, curIndex, GL_UNSIGNED_INT, 0 );
	glBindVertexArray( 0 );
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

/*GLuint Chunk::getUniformBuffer() const
{
	return uniformBuffer;
}*/