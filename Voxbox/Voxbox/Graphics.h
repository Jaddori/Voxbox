#pragma once

#include "BaseIncludes.h"
#include "Font.h"
#include "Texture.h"
#include "Chunk.h"
#include "Camera.h"

#define GRAPHICS_MAX_GLYPHS 128
#define GRAPHICS_MAX_QUADS 128

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool load();
	void unload();

	void begin();
	void end();

	void renderChunk( Chunk* chunk );
	void renderText( Font* font, const char* text, const glm::vec2& position );
	void renderQuad( const glm::vec2& position, const glm::vec2& size, Texture* texture, float opacity );

	Camera& getChunkCamera();

private:
	// rendering chunks
	Camera chunkCamera;
	Shader chunkShader;
	GLint chunkViewMatrixLocation;
	GLint chunkProjectionMatrixLocation;
	GLint chunkOffsetLocation;

	GLuint chunkVAO;
	GLuint chunkVBO;
	GLuint chunkIBO;
	GLuint chunkUBO;

	// rendering text
	Camera textCamera;
	Shader textShader;
	GLint textProjectionMatrixLocation;

	GLuint textVAO;
	GLuint textVBO;
	//GLuint textIBO;

	// rendering quads
	Shader quadShader;
	GLint quadProjectionMatrixLocation;

	GLuint quadVAO;
	GLuint quadVBO;
};

struct Glyph
{
	glm::vec2 position;
	glm::vec4 uv;
	glm::vec2 size;
};

struct Quad
{
	glm::vec2 position;
	glm::vec4 uv;
	glm::vec2 size;
	float opacity;
};