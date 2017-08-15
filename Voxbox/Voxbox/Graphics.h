#pragma once

#include "BaseIncludes.h"
#include "Font.h"
#include "Texture.h"
#include "Chunk.h"
#include "Camera.h"
#include "Assets.h"

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

struct GlyphCollection
{
	Texture* texture;
	Array<Glyph> glyphs[2];
};

struct Quad
{
	glm::vec2 position;
	glm::vec4 uv;
	glm::vec2 size;
	float opacity;
};

struct QuadCollection
{
	Texture* texture;
	Array<Quad> quads[2];
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
	Array<Billboard> billboards[2];
};

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool load( Assets* assets );
	void unload();
	void finalize();

	void render();

	void queueChunk( Chunk* chunk );
	void queueText( Font* font, const char* text, const glm::vec2& position, const glm::vec4& color );
	void queueQuad( const glm::vec2& position, const glm::vec2& size, const glm::vec4& uv, float opacity, Texture* texture );
	void queueBillboard( const glm::vec3& position, const glm::vec2& size, const glm::vec4& uv, bool spherical, Texture* texture );

	Camera& getPerspectiveCamera();
	Camera& getOrthographicCamera();

private:
	Camera perspectiveCamera;
	Camera orthographicCamera;
	int writeIndex;
	int readIndex;

	// rendering chunks
	Shader chunkShader;
	Texture* blockAtlas;
	GLint chunkProjectionMatrixLocation;
	GLint chunkViewMatrixLocation;
	GLint chunkOffsetLocation;

	Array<Chunk*> chunks[2];
	
	// rendering text
	Shader textShader;
	GLint textProjectionMatrixLocation;

	GLuint textVAO;
	GLuint textVBO;

	Array<GlyphCollection> glyphCollections;

	// rendering quads
	Shader quadShader;
	GLint quadProjectionMatrixLocation;

	GLuint quadVAO;
	GLuint quadVBO;

	Array<QuadCollection> quadCollections;

	// rendering billboards
	Shader billboardShader;
	GLint billboardProjectionMatrixLocation;
	GLint billboardViewMatrixLocation;

	GLuint billboardVAO;
	GLuint billboardVBO;

	Array<BillboardCollection> billboardCollections;
};