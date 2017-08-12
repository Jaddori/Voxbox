#pragma once

#include "BaseIncludes.h"
#include "Shader.h"

#define CHUNK_SIZE 16
#define CHUNK_VOLUME (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE)
#define CHUNK_NOISE_MULTIPLIER 0.0137f

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
};

class Chunk
{
public:
	Chunk();
	~Chunk();
	
	void upload();
	void unload();

	void calculateFaces();
	void noise( int x, int z );
	void render();

	void setOffset( const glm::vec3& offset );

	const uint8_t* getBlocks() const;
	const glm::vec3& getOffset() const;
	bool getValid() const;
	bool getUploaded() const;

private:
	inline int at( int x, int y, int z ) { return ( y*CHUNK_SIZE*CHUNK_SIZE + z*CHUNK_SIZE + x ); }
	inline uint8_t block( int x, int y, int z ) { return blocks[at( x, y, z )]; }
	void addHorizontalFace( const glm::vec3& position, const glm::vec3& direction, bool invert );
	void addVerticalFace( const glm::vec3& position, bool invert );

	uint8_t blocks[CHUNK_VOLUME];
	glm::vec3 offset;

	Vertex* vertices;
	GLuint* indices;
	int numVertices;
	int numIndices;

	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	bool valid;
	bool uploaded;
};