#pragma once

#include "BaseIncludes.h"
#include "Shader.h"

#define CHUNK_SIZE 16
#define CHUNK_VOLUME (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE)

class Chunk
{
public:
	Chunk();
	~Chunk();

	// TEMP
	void load( Shader* shader );

	void render( Shader* shader, GLint chunkSizeLocation, GLint offsetLocation, GLint positionLocation );
	void calculatePositions();

	void setOffset( const glm::vec3& offset );

private:
	inline int at( int x, int y, int z ) { return ( y*CHUNK_SIZE*CHUNK_SIZE + z*CHUNK_SIZE + x ); }

	uint8_t blocks[CHUNK_VOLUME];
	glm::vec4 positions[CHUNK_VOLUME];
	glm::vec3 offset;
	int activeBlocks = 0;

	// TEMP:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint ubo;
};