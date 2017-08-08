#include "Chunk.h"

Chunk::Chunk()
{
#if 1
	memset( blocks, 1, CHUNK_VOLUME );
#else
	for( int i=0; i<CHUNK_VOLUME; i++ )
	{
		blocks[i] = rand() % 2;
	}
#endif
}

Chunk::~Chunk()
{
}

void Chunk::load( Shader* shader )
{
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glGenBuffers( 1, &vbo );
	glGenBuffers( 1, &ibo );

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );

	GLfloat vdata[] =
	{
		// front
		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 0.0f,

		// right
		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,		0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,

		// back
		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,		1.0f, 0.0f,

		// left
		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,

		// top
		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,

		// bottom
		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	};

	GLuint idata[] =
	{ 
		0, 1, 2, 1, 3, 2,
		4, 5, 6, 5, 7, 6,
		8, 9, 10, 9, 11, 10,
		12, 13, 14, 13, 15, 14,
		16, 17, 18, 17, 19, 18,
		20, 21, 22, 21, 23, 22,
	};

	glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*20*6, vdata, GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6*6, idata, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, 0 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (void*)(sizeof(GLfloat)*3 ) );

	GLuint bindingPoint = 0;
	GLuint blockIndex = glGetUniformBlockIndex( shader->getProgram(), "blockPositions" );
	glUniformBlockBinding( shader->getProgram(), blockIndex, bindingPoint );

	glGenBuffers( 1, &ubo );
	glBindBuffer( GL_UNIFORM_BUFFER, ubo );
	glBindBufferBase( GL_UNIFORM_BUFFER, bindingPoint, ubo );
	glBufferData( GL_UNIFORM_BUFFER, sizeof(glm::vec4)*CHUNK_VOLUME, positions, GL_DYNAMIC_DRAW );

	glBindVertexArray( 0 );

	calculatePositions();
}

void Chunk::render( Shader* shader, GLint chunkSizeLocation, GLint offsetLocation, GLint positionLocation )
{
	shader->setInt( chunkSizeLocation, CHUNK_SIZE );
	shader->setVec3( offsetLocation, offset );
	shader->setVec4( positionLocation, positions, activeBlocks );

	glBindVertexArray( vao );
	glDrawElementsInstanced( GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0, activeBlocks );
	glBindVertexArray( 0 );
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
				if( blocks[at( x, y, z )] > 0 )
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
						positions[activeBlocks++] = glm::vec4( x, y, z, 0.0f );
					}
				}
			}
		}
	}

	glBindBuffer( GL_UNIFORM_BUFFER, ubo );
	GLvoid* p = glMapBuffer( GL_UNIFORM_BUFFER, GL_WRITE_ONLY );
	memcpy( p, positions, sizeof(glm::vec4)*activeBlocks );
	glUnmapBuffer( GL_UNIFORM_BUFFER );
}

void Chunk::setOffset( const glm::vec3& o )
{
	offset = o;
}