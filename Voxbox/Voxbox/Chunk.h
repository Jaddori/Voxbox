#pragma once

#include "BaseIncludes.h"
#include "Shader.h"

#define CHUNK_SIZE 16
#define CHUNK_VOLUME (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE)
#define CHUNK_NOISE_MULTIPLIER 0.0137f

#define CHUNK_FRONT_UV glm::vec2( 0.0f, 0.05f )
#define CHUNK_BACK_UV CHUNK_FRONT_UV
#define CHUNK_LEFT_UV glm::vec2( 0.05f, 0.05f )
#define CHUNK_RIGHT_UV CHUNK_LEFT_UV
#define CHUNK_TOP_UV glm::vec2( 0.0f, 0.0f )
#define CHUNK_BOTTOM_UV glm::vec2( 0.05f, 0.0f )

#define CHUNK_STONE_THRESHOLD 4

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
	void setUploaded( bool uploaded );

	const uint8_t* getBlocks() const;
	const glm::vec3& getOffset() const;
	bool getValid() const;
	bool getUploaded() const;

private:
	inline int at( int x, int y, int z ) { return ( y*CHUNK_SIZE*CHUNK_SIZE + z*CHUNK_SIZE + x ); }
	inline uint8_t block( int x, int y, int z ) { return blocks[at( x, y, z )]; }
	void addHorizontalFace( const glm::vec3& position, const glm::vec3& direction, const glm::vec2& uvOffset, bool invert );
	void addVerticalFace( const glm::vec3& position, const glm::vec2& uvOffset, bool invert );

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