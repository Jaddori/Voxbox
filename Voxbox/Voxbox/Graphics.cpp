#include "Graphics.h"

Graphics::Graphics()
	: chunkViewMatrixLocation( 0 ), chunkProjectionMatrixLocation( 0 ), chunkOffsetLocation( 0 ),
	textProjectionMatrixLocation( 0 ), textVAO( 0 ), textVBO( 0 ),
	quadProjectionMatrixLocation( 0 ), quadVAO( 0 ), quadVBO( 0 ),
	billboardProjectionMatrixLocation( 0 ), billboardViewMatrixLocation( 0 ),
	billboardVAO( 0 ), billboardVBO( 0 )
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
}

void Graphics::end()
{
	// render billboards
	billboardShader.bind();
	billboardShader.setMat4( billboardProjectionMatrixLocation, chunkCamera.getProjectionMatrix() );
	billboardShader.setMat4( billboardViewMatrixLocation, chunkCamera.getViewMatrix() );

	glBindVertexArray( billboardVAO );
	glBindBuffer( GL_ARRAY_BUFFER, billboardVBO );

	const int BILLBOARD_COLLECTION_COUNT = billboardCollections.getSize();
	for( int curCollection = 0; curCollection < BILLBOARD_COLLECTION_COUNT; curCollection++ )
	{
		BillboardCollection& collection = billboardCollections[curCollection];

		if( collection.texture )
			collection.texture->bind();
		else
			glBindTexture( GL_TEXTURE_2D, 0 );

		const int BILLBOARD_COUNT = collection.billboards.getSize();
		int offset = 0;
		while( offset < BILLBOARD_COUNT )
		{
			int count = BILLBOARD_COUNT - offset;
			if( count > GRAPHICS_MAX_BILLBOARDS )
				count = GRAPHICS_MAX_BILLBOARDS;
			
			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Billboard)*count, collection.billboards.getData()+offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}

		collection.billboards.clear();
	}

	glBindVertexArray( 0 );

	// render quads
	quadShader.bind();
	quadShader.setMat4( quadProjectionMatrixLocation, textCamera.getProjectionMatrix() );

	glBindVertexArray( quadVAO );
	glBindBuffer( GL_ARRAY_BUFFER, quadVBO );

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	const int QUAD_COLLECTION_COUNT = quadCollections.getSize();
	for( int curCollection = 0; curCollection < QUAD_COLLECTION_COUNT; curCollection++ )
	{
		QuadCollection& collection = quadCollections[curCollection];

		if( collection.texture )
			collection.texture->bind();
		else
			glBindTexture( GL_TEXTURE_2D, 0 );

		const int QUAD_COUNT = collection.quads.getSize();
		int offset = 0;
		while( offset < QUAD_COUNT )
		{
			int count = QUAD_COUNT - offset;
			if( count > GRAPHICS_MAX_QUADS )
				count = GRAPHICS_MAX_QUADS;

			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Quad)*count, collection.quads.getData() + offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}

		collection.quads.clear();
	}

	// render text
	textShader.bind();
	textShader.setMat4( textProjectionMatrixLocation, textCamera.getProjectionMatrix() );

	glBindVertexArray( textVAO );
	glBindBuffer( GL_ARRAY_BUFFER, textVBO );

	const int GLYPH_COLLECTION_COUNT = glyphCollections.getSize();
	for( int curCollection = 0; curCollection < GLYPH_COLLECTION_COUNT; curCollection++ )
	{
		GlyphCollection& collection = glyphCollections[curCollection];

		if( collection.texture )
			collection.texture->bind();
		else
			glBindTexture( GL_TEXTURE_2D, 0 );

		const int GLYPH_COUNT = collection.glyphs.getSize();
		int offset = 0;
		while( offset < GLYPH_COUNT )
		{
			int count = GLYPH_COUNT - offset;
			if( count > GRAPHICS_MAX_GLYPHS )
				count = GRAPHICS_MAX_GLYPHS;

			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Glyph)*count, collection.glyphs.getData()+offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}

		collection.glyphs.clear();
	}

	glBindVertexArray( 0 );

	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
}

void Graphics::renderChunk( Chunk* chunk )
{
	chunkShader.bind();
	//chunkShader.setVec3( chunkOffsetLocation, chunk->getOffset() );

	chunk->render();
}

void Graphics::renderText( Font* font, const char* text, const glm::vec2& position, const glm::vec4& color )
{
	const int GLYPH_COLLECTION_COUNT = glyphCollections.getSize();
	int collectionIndex = -1;
	for( int i=0; i<GLYPH_COLLECTION_COUNT && collectionIndex < 0; i++ )
		if( glyphCollections[i].texture == font->getTexture() )
			collectionIndex = i;

	if( collectionIndex < 0 ) // this is a new texture
	{
		GlyphCollection& collection = glyphCollections.append();
		collection.texture = font->getTexture();

		collectionIndex = GLYPH_COLLECTION_COUNT;
	}

	GlyphCollection& collection = glyphCollections[collectionIndex];

	glm::vec2 offset;
	int index = 0;

	const char* cur = text;
	while( *cur )
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

			Glyph& glyph = collection.glyphs.append();

			glyph.position = position + offset;
			glyph.uv = font->getUV( c );
			glyph.size.x = font->getWidth( c );
			glyph.size.y = font->getHeight();
			glyph.color = color;

			offset.x += glyph.size.x;

			index++;
		}

		*cur++;
	}
}

void Graphics::renderQuad( const glm::vec2& position, const glm::vec4& uv, const glm::vec2& size, float opacity, Texture* texture  )
{
	/*quadShader.bind();

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
	glEnable( GL_DEPTH_TEST );*/

	const int QUAD_COUNT = quadCollections.getSize();

	int index = -1;
	for( int i=0; i<QUAD_COUNT && index < 0; i++ )
		if( quadCollections[i].texture == texture )
			index = i;

	if( index < 0 ) // this is a new texture
	{
		QuadCollection& collection = quadCollections.append();
		collection.texture = texture;
		collection.quads.expand( GRAPHICS_MAX_QUADS );

		index = QUAD_COUNT;
	}

	Quad& quad = quadCollections[index].quads.append();
	quad.position = position;
	quad.uv = uv;
	quad.size = size;
	quad.opacity = opacity;
}

void Graphics::renderBillboard( const glm::vec3& position, const glm::vec4& uv, const glm::vec2& size, bool spherical, Texture* texture )
{
	const int BILLBOARD_COUNT = billboardCollections.getSize();

	int index = -1;
	for( int i=0; i<BILLBOARD_COUNT && index < 0; i++ )
		if( billboardCollections[i].texture == texture )
			index = i;

	if( index < 0 ) // this is a new texture
	{
		BillboardCollection& collection = billboardCollections.append();
		collection.texture = texture;
		collection.billboards.expand( GRAPHICS_MAX_BILLBOARDS );

		index = BILLBOARD_COUNT;
	}

	Billboard& billboard = billboardCollections[index].billboards.append();
	billboard.position = position;
	billboard.uv = uv;
	billboard.size = size;
	billboard.spherical = ( spherical ? 1.0f : 0.0f );
}

Camera& Graphics::getChunkCamera()
{
	return chunkCamera;
}

Camera& Graphics::getTextCamera()
{
	return textCamera;
}