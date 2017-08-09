#include "Graphics.h"

Graphics::Graphics()
	: chunkViewMatrixLocation( 0 ), chunkProjectionMatrixLocation( 0 ), chunkOffsetLocation( 0 ),
	chunkVAO( 0 ), chunkVBO( 0 ), chunkIBO( 0 ), chunkUBO( 0 )
{
}

Graphics::~Graphics()
{
}

bool Graphics::load()
{
	bool result = false;

	chunkCamera.updatePerspective( WINDOW_WIDTH, WINDOW_HEIGHT );
	textCamera.updateOrthographic( WINDOW_WIDTH, WINDOW_HEIGHT );

	if( chunkShader.load( "./assets/shaders/chunk.vs",
							nullptr,
							"./assets/shaders/chunk.fs" ) )
	{
		chunkViewMatrixLocation = chunkShader.getLocation( "viewMatrix" );
		chunkProjectionMatrixLocation = chunkShader.getLocation( "projectionMatrix" );
		chunkOffsetLocation = chunkShader.getLocation( "offset" );

		glGenVertexArrays( 1, &chunkVAO );
		glBindVertexArray( chunkVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );

		glGenBuffers( 1, &chunkVBO );
		glGenBuffers( 1, &chunkIBO );

		glBindBuffer( GL_ARRAY_BUFFER, chunkVBO );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, chunkIBO );

		GLfloat vdata[] =
		{
			// front
			0.0f, 0.0f, 0.0f,		0.0f, 0.1f,
			0.0f, 1.0f, 0.0f,		0.0f, 0.05f,
			1.0f, 0.0f, 0.0f,		0.05f, 0.1f,
			1.0f, 1.0f, 0.0f,		0.05f, 0.05f,

			// right
			1.0f, 0.0f, 0.0f,		0.05f, 0.1f,
			1.0f, 1.0f, 0.0f,		0.05f, 0.05f,
			1.0f, 0.0f, 1.0f,		0.1f, 0.1f,
			1.0f, 1.0f, 1.0f,		0.1f, 0.05f,

			// back
			1.0f, 0.0f, 1.0f,		0.0f, 0.1f,
			1.0f, 1.0f, 1.0f,		0.0f, 0.05f,
			0.0f, 0.0f, 1.0f,		0.05f, 0.1f,
			0.0f, 1.0f, 1.0f,		0.05f, 0.05f,

			// left
			0.0f, 0.0f, 1.0f,		0.05f, 0.1f,
			0.0f, 1.0f, 1.0f,		0.05f, 0.05f,
			0.0f, 0.0f, 0.0f,		0.1f, 0.1f,
			0.0f, 1.0f, 0.0f,		0.1f, 0.05f,

			// top
			0.0f, 1.0f, 0.0f,		0.0f, 0.05f,
			0.0f, 1.0f, 1.0f,		0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,		0.05f, 0.05f,
			1.0f, 1.0f, 1.0f,		0.05f, 0.0f,

			// bottom
			0.0f, 0.0f, 1.0f,		0.05f, 0.05f,
			0.0f, 0.0f, 0.0f,		0.05f, 0.0f,
			1.0f, 0.0f, 1.0f,		0.1f, 0.05f,
			1.0f, 0.0f, 0.0f,		0.1f, 0.0f,
		};

		GLuint idata[] =
		{ 
			0, 1, 2, 1, 3, 2,			// front
			4, 5, 6, 5, 7, 6,			// right
			8, 9, 10, 9, 11, 10,		// back
			12, 13, 14, 13, 15, 14,		// left
			16, 17, 18, 17, 19, 18,		// top
			20, 21, 22, 21, 23, 22,		// bottom
		};

		glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*20*6, vdata, GL_STATIC_DRAW );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6*6, idata, GL_STATIC_DRAW );

		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, 0 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (void*)(sizeof(GLfloat)*3 ) );

		GLuint bindingPoint = 0;
		GLuint blockIndex = glGetUniformBlockIndex( chunkShader.getProgram(), "blockPositions" );
		glUniformBlockBinding( chunkShader.getProgram(), blockIndex, bindingPoint );

		glGenBuffers( 1, &chunkUBO );
		glBindBuffer( GL_UNIFORM_BUFFER, chunkUBO );
		glBindBufferBase( GL_UNIFORM_BUFFER, bindingPoint, chunkUBO );
		glBufferData( GL_UNIFORM_BUFFER, sizeof(glm::vec4)*CHUNK_VOLUME, nullptr, GL_DYNAMIC_DRAW );

		glBindVertexArray( 0 );
	}
	else
	{
		printf( "Failed to load chunk shader.\n" );
		result = false;
	}

	if( textShader.load( "./assets/shaders/text.vs",
							nullptr,
							"./assets/shaders/text.fs" ) )
	{
		textProjectionMatrixLocation = textShader.getLocation( "projectionMatrix" );
		textWorldMatrixLocation = textShader.getLocation( "worldMatrix" );
		textUVOffsetLocation = textShader.getLocation( "UVOffset" );
		textUVLengthLocation = textShader.getLocation( "UVLength" );

		glGenVertexArrays( 1, &textVAO );
		glBindVertexArray( textVAO );

		glEnableVertexAttribArray( 0 );

		glGenBuffers( 1, &textVBO );
		glGenBuffers( 1, &textIBO );

		GLfloat vdata[] =
		{
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};

		GLuint idata[] = { 0, 1, 2, 1, 3, 2 };

		glBindBuffer( GL_ARRAY_BUFFER, textVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*8, vdata, GL_STATIC_DRAW );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, textIBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, idata, GL_STATIC_DRAW );

		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, 0 );

		glBindVertexArray( 0 );
	}
	else
	{
		printf( "Failed to load text shader.\n" );
		result = false;
	}

	return result;
}

void Graphics::unload()
{
	chunkShader.unload();
	textShader.unload();

	if( chunkVAO )
	{
		glDeleteVertexArrays( 1, &chunkVAO );
		glDeleteBuffers( 1, &chunkVBO );
		glDeleteBuffers( 1, &chunkIBO );
		glDeleteBuffers( 1, &chunkUBO );
	}

	chunkVAO = chunkVBO = chunkIBO = chunkUBO = 0;

	if( textVAO )
	{
		glDeleteVertexArrays( 1, &textVAO );
		glDeleteBuffers( 1, &textVBO );
		glDeleteBuffers( 1, &textIBO );
	}

	textVAO = textVBO = textIBO = 0;
}

void Graphics::begin()
{
	chunkShader.bind();
	chunkShader.setMat4( chunkViewMatrixLocation, chunkCamera.getViewMatrix() );
	chunkShader.setMat4( chunkProjectionMatrixLocation, chunkCamera.getProjectionMatrix() );

	textShader.bind();
	textShader.setMat4( textProjectionMatrixLocation, textCamera.getProjectionMatrix() );
}

void Graphics::end()
{
}

void Graphics::renderChunk( Chunk* chunk )
{
	chunkShader.bind();
	chunkShader.setVec3( chunkOffsetLocation, chunk->getOffset() );

	glBindVertexArray( chunkVAO );

	glBindBuffer( GL_UNIFORM_BUFFER, chunkUBO );
	GLvoid* p = glMapBuffer( GL_UNIFORM_BUFFER, GL_WRITE_ONLY );
	memcpy( p, chunk->getPositions(), sizeof(glm::vec4)*chunk->getActiveBlocks() );
	glUnmapBuffer( GL_UNIFORM_BUFFER );

	glDrawElementsInstanced( GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, nullptr, chunk->getActiveBlocks() );

	glBindVertexArray( 0 );
}

void Graphics::renderText( Font* font, const char* text, const glm::vec3& position )
{
	glm::vec3 offset;

	textShader.bind();
	font->getTexture().bind();

	glBindVertexArray( textVAO );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	const char* cur = text;
	while( *cur )
	{
		//glm::mat4 worldMatrix = glm::translate( glm::mat4(), position + offset );
		glm::mat4 worldMatrix = glm::scale( glm::translate( glm::mat4(), position + offset ), glm::vec3( 200.0f, 200.0f, 0.0f ) );

		textShader.setMat4( textWorldMatrixLocation, worldMatrix );
		textShader.setVec2( textUVOffsetLocation, glm::vec2( 0.0f, 0.0f ) );
		textShader.setVec2( textUVLengthLocation, glm::vec2( 1.0f, 1.0f ) );

		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

		cur++;
	}

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );

	glBindVertexArray( 0 );
}

Camera& Graphics::getChunkCamera()
{
	return chunkCamera;
}