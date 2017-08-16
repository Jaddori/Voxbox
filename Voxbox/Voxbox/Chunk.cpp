#include "Chunk.h"

Chunk::Chunk()
	: vao( 0 ), vbo( 0 ), ibo( 0 ),
	vertices( nullptr ), indices( nullptr ),
	valid( false ), uploaded( false )
{
	memset( blocks, 0, CHUNK_VOLUME );
#if 0
	for( int y=0; y<CHUNK_SIZE; y++ )
	{
		blocks[at( 0, y, 0 )] = 1;
		blocks[at( 3, y, 3 )] = 1;
		blocks[at( 5, y, 5 )] = y % 3;
	}
#endif
}

Chunk::~Chunk()
{
}

void Chunk::upload()
{
	LOG_ASSERT( vertices != nullptr && indices != nullptr, "Trying to upload chunk without generating faces first." );

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

	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertices, GL_DYNAMIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numIndices, indices, GL_DYNAMIC_DRAW );
	glBindVertexArray( 0 );

	delete[] vertices;
	delete[] indices;

	vertices = nullptr;
	indices = nullptr;

	uploaded = true;
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

void Chunk::calculateFaces()
{
	if( vertices == nullptr )
	{
		vertices = new Vertex[CHUNK_VOLUME*24];
		indices = new GLuint[CHUNK_VOLUME*36];
	}

	numVertices = 0;
	numIndices = 0;
	for( int y=0; y<CHUNK_SIZE; y++ )
	{
		for( int z=0; z<CHUNK_SIZE; z++ )
		{
			for( int x=0; x<CHUNK_SIZE; x++ )
			{
				uint8_t type = block( x, y, z );
				if( type > 0 )
				{
					glm::vec2 uvOrigin( (float)(type % 10)*0.1f, (float)(type / 10)*0.1f );

					// front
					if( z == 0 || block( x, y, z-1 ) == 0 )
					{
						addHorizontalFace( glm::vec3( x, y, z ), glm::vec3( 1.0f, 0.0f, 0.0f ), uvOrigin+CHUNK_FRONT_UV, false );
					}

					// back
					if( z == CHUNK_SIZE-1 || block( x, y, z+1 ) == 0 )
					{
						addHorizontalFace( glm::vec3( x, y, z+1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), uvOrigin+CHUNK_BACK_UV, true );
					}

					// left
					if( x == 0 || block( x-1, y, z ) == 0 )
					{
						addHorizontalFace( glm::vec3( x, y, z ), glm::vec3( 0.0f, 0.0f, 1.0f ), uvOrigin+CHUNK_LEFT_UV, true );
					}

					// right
					if( x == CHUNK_SIZE-1 || block( x+1, y, z ) == 0 )
					{
						addHorizontalFace( glm::vec3( x+1.0f, y, z ), glm::vec3( 0.0f, 0.0f, 1.0f ), uvOrigin+CHUNK_RIGHT_UV, false );
					}

					// top
					if( y == CHUNK_SIZE-1 || block( x, y+1, z ) == 0 )
					{
						addVerticalFace( glm::vec3( x, y+1.0f, z ), uvOrigin+CHUNK_TOP_UV, false );
					}

					// bottom
					if( y == 0 || block( x, y-1, z ) == 0 )
					{
						addVerticalFace( glm::vec3( x, y, z ), uvOrigin+CHUNK_BOTTOM_UV, true );
					}
				}
			}
		}
	}

	valid = true;
}

void Chunk::addHorizontalFace( const glm::vec3& position, const glm::vec3& direction, const glm::vec2& uvOffset, bool invert )
{
	glm::vec3 pos = position + offset * (float)CHUNK_SIZE;

	vertices[numVertices].position = pos;
	vertices[numVertices].uv = glm::vec2( 0.0f, 0.05f ) + uvOffset;

	vertices[numVertices+1].position = pos + glm::vec3( 0.0f, 1.0f, 0.0f );
	vertices[numVertices+1].uv = glm::vec2( 0.0f, 0.0f ) + uvOffset;

	vertices[numVertices+2].position = pos + direction;
	vertices[numVertices+2].uv = glm::vec2( 0.05f, 0.05f ) + uvOffset;

	vertices[numVertices+3].position = pos + direction + glm::vec3( 0.0f, 1.0f, 0.0f );
	vertices[numVertices+3].uv = glm::vec2( 0.05f, 0.0f ) + uvOffset;

	if( invert )
	{
		indices[numIndices] = numVertices;
		indices[numIndices+1] = numVertices+2;
		indices[numIndices+2] = numVertices+1;
		indices[numIndices+3] = numVertices+2;
		indices[numIndices+4] = numVertices+3;
		indices[numIndices+5] = numVertices+1;
	}
	else
	{
		indices[numIndices] = numVertices;
		indices[numIndices+1] = numVertices+1;
		indices[numIndices+2] = numVertices+2;
		indices[numIndices+3] = numVertices+1;
		indices[numIndices+4] = numVertices+3;
		indices[numIndices+5] = numVertices+2;
	}

	numVertices += 4;
	numIndices += 6;
}

void Chunk::addVerticalFace( const glm::vec3& position, const glm::vec2& uvOffset, bool invert )
{
	glm::vec3 pos = position + offset * (float)CHUNK_SIZE;

	vertices[numVertices].position = pos;
	vertices[numVertices].uv = glm::vec2( 0.0f, 0.05f ) + uvOffset;

	vertices[numVertices+1].position = pos + glm::vec3( 0.0f, 0.0f, 1.0f );
	vertices[numVertices+1].uv = glm::vec2( 0.0f, 0.0f ) + uvOffset;

	vertices[numVertices+2].position = pos + glm::vec3( 1.0f, 0.0f, 0.0f );
	vertices[numVertices+2].uv = glm::vec2( 0.05f, 0.05f ) + uvOffset;

	vertices[numVertices+3].position = pos + glm::vec3( 1.0f, 0.0f, 1.0f );
	vertices[numVertices+3].uv = glm::vec2( 0.05f, 0.0f ) + uvOffset;

	if( invert )
	{
		indices[numIndices] = numVertices;
		indices[numIndices+1] = numVertices+2;
		indices[numIndices+2] = numVertices+1;
		indices[numIndices+3] = numVertices+2;
		indices[numIndices+4] = numVertices+3;
		indices[numIndices+5] = numVertices+1;
	}
	else
	{
		indices[numIndices] = numVertices;
		indices[numIndices+1] = numVertices+1;
		indices[numIndices+2] = numVertices+2;
		indices[numIndices+3] = numVertices+1;
		indices[numIndices+4] = numVertices+3;
		indices[numIndices+5] = numVertices+2;
	}

	numVertices += 4;
	numIndices += 6;
}

void Chunk::noise( int x, int z )
{
	for( int blockX = 0; blockX < CHUNK_SIZE; blockX++ )
	{
		for( int blockZ = 0; blockZ < CHUNK_SIZE; blockZ++ )
		{
			float noiseValue = ( glm::perlin( glm::vec2( x+blockX, z+blockZ ) * CHUNK_NOISE_MULTIPLIER ) + 1.0f ) * 0.5f;
			int height = (int)( noiseValue * CHUNK_SIZE ) + 1;
			if( height <= 0 )
				height = 1;
			else if( height > CHUNK_SIZE )
				height = CHUNK_SIZE;

			for( int blockY = 0; blockY < height; blockY++ )
			{
				if( blockY < CHUNK_STONE_THRESHOLD )
					blocks[at( blockX, blockY, blockZ )] = 2;
				else
					blocks[at( blockX, blockY, blockZ )] = 1;
			}
		}
	}
}

void Chunk::render()
{
	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0 );
	glBindVertexArray( 0 );
}

bool Chunk::hitBlock( const glm::vec3& rayStart, const glm::vec3& rayEnd, glm::vec3& location )
{
	bool result = false;

	float minDistance = CAMERA_FAR+10.0f;

	for( int y=0; y<CHUNK_SIZE; y++ )
	{
		for( int x=0; x<CHUNK_SIZE; x++ )
		{
			for( int z=0; z<CHUNK_SIZE; z++ )
			{
				if( block( x, y, z ) > 0 )
				{
					glm::vec3 minPosition = offset * (float)CHUNK_SIZE + glm::vec3( x, y, z );
					glm::vec3 maxPosition = minPosition + glm::vec3( 1.0f );

					if( rayCheck( rayStart, rayEnd, minPosition, maxPosition ) )
					{
						float distance = glm::distance( rayStart, minPosition );
						if( distance < minDistance )
						{
							minDistance = distance;
							location = minPosition;
						}
						result = true;
					}
				}
			}
		}
	}

	return result;
}

void Chunk::setBlock( int x, int y, int z, uint8_t value )
{
	block( x, y, z ) = value;
}

void Chunk::setOffset( const glm::vec3& o )
{
	offset = o;
}

void Chunk::setUploaded( bool u )
{
	uploaded = u;
}

const uint8_t* Chunk::getBlocks() const
{
	return blocks;
}

const glm::vec3& Chunk::getOffset() const
{
	return offset;
}

bool Chunk::getValid() const
{
	return valid;
}

bool Chunk::getUploaded() const
{
	return uploaded;
}