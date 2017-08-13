#pragma once

#include "BaseIncludes.h"
#include "Font.h"
#include "Texture.h"
#include "Chunk.h"
#include "Camera.h"

#define GRAPHICS_MAX_GLYPHS 128
#define GRAPHICS_MAX_QUADS 128
#define GRAPHICS_MAX_BILLBOARDS 128

struct Glyph
{
	glm::vec2 position;
	glm::vec4 uv;
	glm::vec2 size;
	glm::vec4 color;
};

struct Quad
{
	glm::vec2 position;
	glm::vec4 uv;
	glm::vec2 size;
	float opacity;
};

struct Billboard
{
	glm::vec3 position;
	glm::vec4 uv;
	glm::vec2 size;
	float spherical;
};

struct BillboardCollection
{
	Texture* texture;
	Array<Billboard> billboards;
};

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
	void renderText( Font* font, const char* text, const glm::vec2& position, const glm::vec4& color );
	void renderQuad( const glm::vec2& position, const glm::vec2& size, Texture* texture, float opacity );
	void renderBillboard( const glm::vec3& position, const glm::vec4& uv, const glm::vec2& size, bool spherical, Texture* texture );

	Camera& getChunkCamera();
	Camera& getTextCamera();

private:
	// rendering chunks
	Camera chunkCamera;
	Shader chunkShader;
	GLint chunkViewMatrixLocation;
	GLint chunkProjectionMatrixLocation;
	GLint chunkOffsetLocation;

	// rendering text
	Camera textCamera;
	Shader textShader;
	GLint textProjectionMatrixLocation;

	GLuint textVAO;
	GLuint textVBO;

	// rendering quads
	Shader quadShader;
	GLint quadProjectionMatrixLocation;

	GLuint quadVAO;
	GLuint quadVBO;

	// rendering billboards
	Shader billboardShader;
	GLint billboardProjectionMatrixLocation;
	GLint billboardViewMatrixLocation;

	GLuint billboardVAO;
	GLuint billboardVBO;

	Array<BillboardCollection> billboards;
};