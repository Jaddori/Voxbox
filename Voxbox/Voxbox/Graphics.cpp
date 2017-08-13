#include "Graphics.h"

Graphics::Graphics()
	: chunkViewMatrixLocation( 0 ), chunkProjectionMatrixLocation( 0 ), chunkOffsetLocation( 0 )
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
		glEnableVertexAttribArray( 3 );

		glGenBuffers( 1, &textVBO );
		glBindBuffer( GL_ARRAY_BUFFER, textVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(Glyph)*GRAPHICS_MAX_GLYPHS, nullptr, GL_STREAM_DRAW );

		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof(Glyph), 0 );
		glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)(sizeof(GLfloat)*2) );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)(sizeof(GLfloat)*6) );
		glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)(sizeof(GLfloat)*8) );

		glBindVertexArray( 0 );
	}
	else
	{
		LOG( VERBOSITY_ERROR, "Graphics.cpp - Failed to load text shader." );
		result = false;
	}

	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Loading quad shader." );
	if( quadShader.load( "./assets/shaders/quad.vs",
							"./assets/shaders/quad.gs",
							"./assets/shaders/quad.fs" ) )
	{
		LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Retrieving uniform locations from text shader." );
		quadProjectionMatrixLocation = quadShader.getLocation( "projectionMatrix" );

		LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Generating vertex data for quad shader." );
		glGenVertexArrays( 1, &quadVAO );
		glBindVertexArray( quadVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );
		glEnableVertexAttribArray( 3 );

		glGenBuffers( 1, &quadVBO );
		glBindBuffer( GL_ARRAY_BUFFER, quadVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(Quad)*GRAPHICS_MAX_QUADS, nullptr, GL_STREAM_DRAW );

		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof(Glyph), 0 );
		glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)(sizeof(GLfloat)*2) );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)(sizeof(GLfloat)*6) );
		glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)(sizeof(GLfloat)*8) );

		glBindVertexArray( 0 );
	}

	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Loading billboard shader." );
	if( billboardShader.load( "./assets/shaders/billboard.vs",
								"./assets/shaders/billboard.gs",
								"./assets/shaders/billboard.fs" ) )
	{
		LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Retrieving uniform locations from billboard shader." );
		billboardProjectionMatrixLocation = billboardShader.getLocation( "projectionMatrix" );
		billboardViewMatrixLocation = billboardShader.getLocation( "viewMatrix" );

		LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Generating vertex data for billboard shader." );
		glGenVertexArrays( 1, &billboardVAO );
		glBindVertexArray( billboardVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );
		glEnableVertexAttribArray( 3 );

		glGenBuffers( 1, &billboardVBO );
		glBindBuffer( GL_ARRAY_BUFFER, billboardVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(Billboard)*GRAPHICS_MAX_BILLBOARDS, nullptr, GL_STREAM_DRAW );

		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Billboard), 0 );
		glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof(Billboard), (void*)(sizeof(GLfloat)*3) );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Billboard), (void*)(sizeof(GLfloat)*7 ) );
		glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof(Billboard), (void*)( sizeof(GLfloat)*9) );

		glBindVertexArray( 0 );
	}

	return result;
}

void Graphics::unload()
{
	LOG( VERBOSITY_INFORMATION, "Graphics.cpp - Unloading." );

	chunkShader.unload();
	textShader.unload();

	if( textVAO )
	{
		glDeleteVertexArrays( 1, &textVAO );
		glDeleteBuffers( 1, &textVBO );
	}

	textVAO = textVBO = 0;

	if( quadVAO )
	{
		glDeleteVertexArrays( 1, &quadVAO );
		glDeleteBuffers( 1, &quadVBO );
	}

	quadVAO = quadVBO = 0;

	if( billboardVAO )
	{
		glDeleteVertexArrays( 1, &billboardVAO );
		glDeleteBuffers( 1, &billboardVBO );
	}

	billboardVAO = billboardVBO = 0;
}

void Graphics::begin()
{
	chunkShader.bind();
	chunkShader.setMat4( chunkViewMatrixLocation, chunkCamera.getViewMatrix() );
	chunkShader.setMat4( chunkProjectionMatrixLocation, chunkCamera.getProjectionMatrix() );

	textShader.bind();
	textShader.setMat4( textProjectionMatrixLocation, textCamera.getProjectionMatrix() );

	quadShader.bind();
	quadShader.setMat4( quadProjectionMatrixLocation, textCamera.getProjectionMatrix() );

	billboardShader.bind();
	billboardShader.setMat4( billboardProjectionMatrixLocation, chunkCamera.getProjectionMatrix() );
	billboardShader.setMat4( billboardViewMatrixLocation, chunkCamera.getViewMatrix() );
}

void Graphics::end()
{
}

void Graphics::renderChunk( Chunk* chunk )
{
	chunkShader.bind();
	//chunkShader.setVec3( chunkOffsetLocation, chunk->getOffset() );

	chunk->render();
}

void Graphics::renderText( Font* font, const char* text, const glm::vec2& position, const glm::vec4& color )
{
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
		else if( *cur == '\t' )
		{
			offset.x += font->getWidth(0) * FONT_TAB_WIDTH;
		}
		else if( *cur >= FONT_FIRST && *cur <= FONT_LAST )
		{
			char c = *cur - FONT_FIRST;

			glyphs[index].position = position + offset;
			glyphs[index].uv = font->getUV( c );
			glyphs[index].size.x = font->getWidth( c );
			glyphs[index].size.y = font->getHeight();
			glyphs[index].color = color;

			offset.x += glyphs[index].size.x;

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

void Graphics::renderQuad( const glm::vec2& position, const glm::vec2& size, Texture* texture, float opacity )
{
	quadShader.bind();

	if( texture )
		texture->bind();
	else
		glBindTexture( GL_TEXTURE_2D, 0 );

	Quad quad = { position, glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ), size, opacity };

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glBindVertexArray( quadVAO );
	glBindBuffer( GL_ARRAY_BUFFER, quadVBO );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Quad), &quad );
	glDrawArrays( GL_POINTS, 0, 1 );
	glBindVertexArray( 0 );

	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
}

void Graphics::renderBillboard( const glm::vec3& position, const glm::vec4& uv, const glm::vec2& size, Texture* texture )
{
	billboardShader.bind();

	if( texture )
		texture->bind();
	else
		glBindTexture( GL_TEXTURE_2D, 0 );

	Billboard billboard = { position, uv, size, 1.0f };

	glBindVertexArray( billboardVAO );
	glBindBuffer( GL_ARRAY_BUFFER, billboardVBO );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Billboard), &billboard );
	glDrawArrays( GL_POINTS, 0, 1 );
	glBindVertexArray( 0 );
}

Camera& Graphics::getChunkCamera()
{
	return chunkCamera;
}

Camera& Graphics::getTextCamera()
{
	return textCamera;
}