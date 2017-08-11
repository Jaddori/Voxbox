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
	
	void upload();
	void unload();

	void calculatePositions();
	void noise( int x, int z );

	void setOffset( const glm::vec3& offset );

	const uint8_t* getBlocks() const;
	const glm::vec4* getPositions() const;
	const glm::vec3& getOffset() const;
	int getActiveBlocks() const;

	GLuint getUniformBuffer() const;

private:
	inline int at( int x, int y, int z ) { return ( y*CHUNK_SIZE*CHUNK_SIZE + z*CHUNK_SIZE + x ); }

	uint8_t blocks[CHUNK_VOLUME];
	glm::vec4 positions[CHUNK_VOLUME];
	glm::vec3 offset;
	int activeBlocks;

	GLuint uniformBuffer;
};