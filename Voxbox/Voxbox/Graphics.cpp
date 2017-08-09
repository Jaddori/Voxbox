#include "Graphics.h"

Graphics::Graphics()
	: chunkViewMatrixLocation( 0 ), chunkProjectionMatrixLocation( 0 ), chunkOffsetLocation( 0 ),
	chunkVAO( 0 ), chunkVBO( 0 ), chunkIBO( 0 ), chunkUBO( 0 )
{
	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Constructing." );
}

Graphics::~Graphics()
{
	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Destructing." );
}

bool Graphics::load()
{
	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Loading." );

	bool result = false;

	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Initializing cameras." );
	chunkCamera.updatePerspective( WINDOW_WIDTH, WINDOW_HEIGHT );
	textCamera.updateOrthographic( WINDOW_WIDTH, WINDOW_HEIGHT );

	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Loading chunk shader." );
	if( chunkShader.load( "./assets/shaders/chunk.vs",
							nullptr,
							"./assets/shaders/chunk.fs" ) )
	{
		LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Retrieving uniform locations from chunk shader." );
		chunkViewMatrixLocation = chunkShader.getLocation( "viewMatrix" );
		chunkProjectionMatrixLocation = chunkShader.getLocation( "projectionMatrix" );
		chunkOffsetLocation = chunkShader.getLocation( "offset" );

		LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Generating vertex data for chunk shader." );
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
		LOG( VERBOSITY_ERROR, "Graphics.cpp - Failed to load chunk shader." );
		result = false;
	}

	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Loading text shader." );
	if( textShader.load( "./assets/shaders/text.vs",
							"./assets/shaders/text.gs",
							"./assets/shaders/text.fs" ) )
	{
		LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Retrieving uniform locations from text shader." );
		textProjectionMatrixLocation = textShader.getLocation( "projectionMatrix" );

		LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Generating vertex data for text shader." );
		glGenVertexArrays( 1, &textVAO );
		glBindVertexArray( textVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		glGenBuffers( 1, &textVBO );
		glBindBuffer( GL_ARRAY_BUFFER, textVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(Glyph)*GRAPHICS_MAX_GLYPHS, nullptr, GL_DYNAMIC_DRAW );

		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof(Glyph), 0 );
		glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)(sizeof(GLfloat)*2) );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)(sizeof(GLfloat)*6) );

		glBindVertexArray( 0 );
	}
	else
	{
		LOG( VERBOSITY_ERROR, "Graphics.cpp - Failed to load text shader." );
		result = false;
	}

	return result;
}

void Graphics::unload()
{
	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Unloading." );

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
		//glDeleteBuffers( 1, &textIBO );
	}

	textVAO = textVBO = 0; //textIBO = 0;
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

void Graphics::renderText( Font* font, const char* text, const glm::vec2& position )
{
	const float NEGATIVE_PADDING = -4.0f;

	glm::vec2 offset;

	Glyph glyphs[GRAPHICS_MAX_GLYPHS];

	int index = 0;

	const char* cur = text;
	while( *cur && index < GRAPHICS_MAX_GLYPHS )
	{
		if( *cur == '\n' )
		{
			offset.x = 0;
			offset.y += font->getHeight();
		}
		else if( *cur >= FONT_FIRST && *cur <= FONT_LAST )
		{
			char c = *cur - FONT_FIRST;

			glyphs[index].position = position + offset;
			glyphs[index].uv = font->getUV( c );
			glyphs[index].size.x = font->getWidth( c );
			glyphs[index].size.y = font->getHeight();

			offset.x += glyphs[index].size.x;
			if( c > FONT_FIRST )
				offset.x += NEGATIVE_PADDING;

			index++;
		}

		*cur++;
	}

	textShader.bind();
	font->getTexture().bind();

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glBindVertexArray( textVAO );
	glBindBuffer( GL_ARRAY_BUFFER, textVBO );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Glyph)*index, glyphs );
	glDrawArrays( GL_POINTS, 0, index );
	glBindVertexArray( 0 );

	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
}

Camera& Graphics::getChunkCamera()
{
	return chunkCamera;
}