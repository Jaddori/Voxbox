#pragma once

#include "BaseIncludes.h"
#include "Font.h"
#include "Texture.h"
#include "Chunk.h"
#include "Camera.h"

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
	void renderText( Font* font, const char* text, const glm::vec3& position );

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
	GLint textWorldMatrixLocation;
	GLint textUVOffsetLocation;
	GLint textUVLengthLocation;

	GLuint textVAO;
	GLuint textVBO;
	GLuint textIBO;
};