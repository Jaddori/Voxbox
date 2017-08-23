#include "Graphics.h"

Graphics::Graphics()
	: chunkViewMatrixLocation( 0 ), chunkProjectionMatrixLocation( 0 ), chunkOffsetLocation( 0 ),
	blockViewMatrixLocation( 0 ), blockProjectionMatrixLocation( 0 ),
	blockVAO( 0 ), blockVBO( 0 ), blockIBO( 0 ), blockUBO( 0 ),
	textProjectionMatrixLocation( 0 ), textVAO( 0 ), textVBO( 0 ),
	quadProjectionMatrixLocation( 0 ), quadVAO( 0 ), quadVBO( 0 ),
	billboardProjectionMatrixLocation( 0 ), billboardViewMatrixLocation( 0 ),
	billboardVAO( 0 ), billboardVBO( 0 ),
	writeIndex( 0 ), readIndex( 1 )
{
	LOG_INFO( "Constructing." );
}

Graphics::~Graphics()
{
	LOG_INFO( "Destructing." );
}

bool Graphics::load( Assets* assets )
{
	LOG_INFO( "Loading." );

	bool result = false;

	LOG_INFO( "Initializing cameras." );
	perspectiveCamera.updatePerspective( WINDOW_WIDTH, WINDOW_HEIGHT );
	orthographicCamera.updateOrthographic( WINDOW_WIDTH, WINDOW_HEIGHT );

	LOG_INFO( "Loading chunk shader." );
	if( chunkShader.load( "./assets/shaders/chunk.vs",
							nullptr,
							"./assets/shaders/chunk.fs" ) )
	{
		LOG_INFO( "Retrieving uniform locations from chunk shader." );
		chunkViewMatrixLocation = chunkShader.getLocation( "viewMatrix" );
		chunkProjectionMatrixLocation = chunkShader.getLocation( "projectionMatrix" );
	}
	else
	{
		LOG_ERROR( "Failed to load chunk shader." );
		result = false;
	}

	LOG_INFO( "Loading block atlas." );
	blockAtlas = assets->loadTexture( "./assets/textures/blocks.dds" );
	if( blockAtlas == nullptr )
	{
		LOG_ERROR( "Failed to load block atlas." );
		result = false;
	}

	LOG_INFO( "Loading block shader." );
	if( blockShader.load( "./assets/shaders/block.vs",
							nullptr,
							"./assets/shaders/block.fs" ) )
	{
		LOG_INFO( "Retrieving uniform locations from block shader." );
		blockViewMatrixLocation = blockShader.getLocation( "viewMatrix" );
		blockProjectionMatrixLocation = blockShader.getLocation( "projectionMatrix" );

		LOG_INFO( "Generating vertex data for block shader." );
		glGenVertexArrays( 1, &blockVAO );
		glBindVertexArray( blockVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		glGenBuffers( 1, &blockVBO );
		glGenBuffers( 1, &blockIBO );

		glBindBuffer( GL_ARRAY_BUFFER, blockVBO );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, blockIBO );
		
		const glm::vec3 frontBotLeft( -0.01f, -0.01f, -0.01f );
		const glm::vec3 frontTopLeft( -0.01f,  1.01f, -0.01f );
		const glm::vec3 frontBotRight( 1.01f, -0.01f, -0.01f );
		const glm::vec3 frontTopRight( 1.01f,  1.01f, -0.01f );

		const glm::vec3 backBotLeft( -0.01f, -0.01f, 1.01f );
		const glm::vec3 backTopLeft( -0.01f,  1.01f, 1.01f );
		const glm::vec3 backBotRight( 1.01f, -0.01f, 1.01f );
		const glm::vec3 backTopRight( 1.01f,  1.01f, 1.01f );

		glm::vec3 vdata[] =
		{
			// front
			frontBotLeft,
			frontTopLeft,
			frontBotRight,
			frontTopRight,

			// back
			backBotRight,
			backTopRight,
			backBotLeft,
			backTopLeft,

			// left
			backBotLeft,
			backTopLeft,
			frontBotLeft,
			frontTopLeft,

			// right
			frontBotRight,
			frontTopRight,
			backBotRight,
			backTopRight,

			// top
			frontTopLeft,
			backTopLeft,
			frontTopRight,
			backTopRight,

			// bottom
			backBotLeft,
			frontBotLeft,
			backBotRight,
			frontBotRight
		};

		GLuint idata[] =
		{
			// front
			0, 1, 2,
			1, 3, 2,

			// back
			4, 5, 6,
			5, 7, 6,

			// left
			8, 9, 10,
			9, 11, 10,

			// right
			12, 13, 14,
			13, 15, 14,

			// top
			16, 17, 18,
			17, 19, 18,

			// bottom
			20, 21, 22,
			21, 23, 22,
		};

		glBufferData( GL_ARRAY_BUFFER, sizeof(vdata), vdata, GL_STATIC_DRAW );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(idata), idata, GL_STATIC_DRAW );

		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0 );

		glGenBuffers( 1, &blockUBO );
		glBindBuffer( GL_ARRAY_BUFFER, blockUBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(Block)*GRAPHICS_MAX_BLOCKS, nullptr, GL_STREAM_DRAW );

		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Block), 0 );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof(Block), (void*)(sizeof(glm::vec3)) );

		glVertexAttribDivisor( 1, 1 );
		glVertexAttribDivisor( 2, 1 );

		glBindVertexArray( 0 );
	}
	else
	{
		LOG_ERROR( "Failed to load block shader." );
		result = false;
	}

	LOG_INFO( "Loading text shader." );
	if( textShader.load( "./assets/shaders/text.vs",
							"./assets/shaders/text.gs",
							"./assets/shaders/text.fs" ) )
	{
		LOG_INFO( "Retrieving uniform locations from text shader." );
		textProjectionMatrixLocation = textShader.getLocation( "projectionMatrix" );

		LOG_INFO( "Generating vertex data for text shader." );
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
		LOG_ERROR( "Failed to load text shader." );
		result = false;
	}

	LOG_INFO( "Loading quad shader." );
	if( quadShader.load( "./assets/shaders/quad.vs",
							"./assets/shaders/quad.gs",
							"./assets/shaders/quad.fs" ) )
	{
		LOG_INFO( "Retrieving uniform locations from text shader." );
		quadProjectionMatrixLocation = quadShader.getLocation( "projectionMatrix" );

		LOG_INFO( "Generating vertex data for quad shader." );
		glGenVertexArrays( 1, &quadVAO );
		glBindVertexArray( quadVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );
		glEnableVertexAttribArray( 3 );

		glGenBuffers( 1, &quadVBO );
		glBindBuffer( GL_ARRAY_BUFFER, quadVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(Quad)*GRAPHICS_MAX_QUADS, nullptr, GL_STREAM_DRAW );

		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof(Quad), 0 );
		glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof(Quad), (void*)(sizeof(GLfloat)*2) );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Quad), (void*)(sizeof(GLfloat)*6) );
		glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof(Quad), (void*)(sizeof(GLfloat)*8) );

		glBindVertexArray( 0 );
	}
	else
	{
		LOG_ERROR( "Failed to load quad shader." );
		result = false;
	}

	LOG_INFO( "Loading billboard shader." );
	if( billboardShader.load( "./assets/shaders/billboard.vs",
								"./assets/shaders/billboard.gs",
								"./assets/shaders/billboard.fs" ) )
	{
		LOG_INFO( "Retrieving uniform locations from billboard shader." );
		billboardProjectionMatrixLocation = billboardShader.getLocation( "projectionMatrix" );
		billboardViewMatrixLocation = billboardShader.getLocation( "viewMatrix" );

		LOG_INFO( "Generating vertex data for billboard shader." );
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
	else
	{
		LOG_ERROR( "Failed to load billboard shader." );
		result = false;
	}

	return result;
}

void Graphics::unload()
{
	LOG_INFO( "Unloading." );

	chunkShader.unload();
	blockShader.unload();
	textShader.unload();
	quadShader.unload();
	billboardShader.unload();

	if( blockVAO )
	{
		glDeleteVertexArrays( 1, &blockVAO );
		glDeleteBuffers( 1, &blockVBO );
		glDeleteBuffers( 1, &blockIBO );
		glDeleteBuffers( 1, &blockUBO );
	}

	blockVAO = blockVBO = blockIBO = blockUBO = 0;

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

void Graphics::finalize()
{
	// finalize cameras
	perspectiveCamera.finalize();
	orthographicCamera.finalize();

	// swap index
	//writeIndex = ( writeIndex + 1 ) % 2;
	//readIndex = ( readIndex + 1 ) % 2;
	swap( writeIndex, readIndex );

	// swap chunks
	chunks[writeIndex].clear();

	// swap blocks
	const int BLOCK_COLLECTION_COUNT = blockCollections.getSize();
	for( int i=0; i<BLOCK_COLLECTION_COUNT; i++ )
		blockCollections[i].blocks[writeIndex].clear();

	// swap glyphs
	const int GLYPH_COLLECTION_COUNT = glyphCollections.getSize();
	for( int i=0; i<GLYPH_COLLECTION_COUNT; i++ )
		glyphCollections[i].glyphs[writeIndex].clear();

	// swap quads
	const int QUAD_COLLECTION_COUNT = quadCollections.getSize();
	for( int i=0; i<QUAD_COLLECTION_COUNT; i++ )
		quadCollections[i].quads[writeIndex].clear();

	// swap billboards
	const int BILLBOARD_COLLECTION_COUNT = billboardCollections.getSize();
	for( int i=0; i<BILLBOARD_COLLECTION_COUNT; i++ )
		billboardCollections[i].billboards[writeIndex].clear();
}

void Graphics::render()
{
	// render chunks
	chunkShader.bind();
	chunkShader.setMat4( chunkProjectionMatrixLocation, perspectiveCamera.getProjectionMatrix() );
	chunkShader.setMat4( chunkViewMatrixLocation, perspectiveCamera.getViewMatrix() );

	blockAtlas->bind();

	const int CHUNK_COUNT = chunks[readIndex].getSize();
	for( int i=0; i<CHUNK_COUNT; i++ )
		chunks[readIndex][i]->render();

	glBindTexture( GL_TEXTURE_2D, 0 ); // prevent binding leaks

	// render blocks
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	blockShader.bind();
	blockShader.setMat4( blockProjectionMatrixLocation, perspectiveCamera.getProjectionMatrix() );
	blockShader.setMat4( blockViewMatrixLocation, perspectiveCamera.getViewMatrix() );

	glBindVertexArray( blockVAO );
	glBindBuffer( GL_ARRAY_BUFFER, blockUBO );
	
	const int BLOCK_COLLECTION_COUNT = blockCollections.getSize();
	for( int curCollection = 0; curCollection < BLOCK_COLLECTION_COUNT; curCollection++ )
	{
		BlockCollection& collection = blockCollections[curCollection];

		const int BLOCK_COUNT = collection.blocks[readIndex].getSize();
		int offset = 0;
		while( offset < BLOCK_COUNT )
		{
			int count = BLOCK_COUNT - offset;
			if( count > GRAPHICS_MAX_BLOCKS )
				count = GRAPHICS_MAX_BLOCKS;
			
			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Block)*count, collection.blocks[readIndex].getData() + offset );
			glDrawElementsInstanced( GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, nullptr, count );

			offset += count;
		}
	}

	glDisable( GL_BLEND );

	glBindVertexArray( 0 );

	// render billboards
	billboardShader.bind();
	billboardShader.setMat4( billboardProjectionMatrixLocation, perspectiveCamera.getProjectionMatrix() );
	billboardShader.setMat4( billboardViewMatrixLocation, perspectiveCamera.getViewMatrix() );
	
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

		const int BILLBOARD_COUNT = collection.billboards[readIndex].getSize();
		int offset = 0;
		while( offset < BILLBOARD_COUNT )
		{
			int count = BILLBOARD_COUNT - offset;
			if( count > GRAPHICS_MAX_BILLBOARDS )
				count = GRAPHICS_MAX_BILLBOARDS;
			
			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Billboard)*count, collection.billboards[readIndex].getData()+offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}

		//collection.billboards.clear();
	}

	glBindVertexArray( 0 );

	// render quads
	quadShader.bind();
	quadShader.setMat4( quadProjectionMatrixLocation, orthographicCamera.getProjectionMatrix() );

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

		const int QUAD_COUNT = collection.quads[readIndex].getSize();
		int offset = 0;
		while( offset < QUAD_COUNT )
		{
			int count = QUAD_COUNT - offset;
			if( count > GRAPHICS_MAX_QUADS )
				count = GRAPHICS_MAX_QUADS;

			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Quad)*count, collection.quads[readIndex].getData() + offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}

		//collection.quads.clear();
	}

	glBindVertexArray( 0 );

	// render text
	textShader.bind();
	textShader.setMat4( textProjectionMatrixLocation, orthographicCamera.getProjectionMatrix() );

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

		const int GLYPH_COUNT = collection.glyphs[readIndex].getSize();
		int offset = 0;
		while( offset < GLYPH_COUNT )
		{
			int count = GLYPH_COUNT - offset;
			if( count > GRAPHICS_MAX_GLYPHS )
				count = GRAPHICS_MAX_GLYPHS;

			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Glyph)*count, collection.glyphs[readIndex].getData()+offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}
	}

	glBindVertexArray( 0 );

	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
}

void Graphics::queueChunk( Chunk* chunk )
{
	chunks[writeIndex].add( chunk );
}

void Graphics::queueBlock( const glm::vec3& position, const glm::vec4& color )
{
	if( blockCollections.getSize() <= 0 )
	{
		BlockCollection& collection = blockCollections.append();
		collection.blocks[writeIndex].expand( GRAPHICS_MAX_BLOCKS );
		collection.blocks[readIndex].expand( GRAPHICS_MAX_BLOCKS );
	}

	BlockCollection& collection = blockCollections[0];

	Block& block = collection.blocks[writeIndex].append();
	block.position = position;
	block.color = color;
}

void Graphics::queueText( Font* font, const char* text, const glm::vec2& position, const glm::vec4& color )
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
		collection.glyphs[writeIndex].expand( GRAPHICS_MAX_GLYPHS );
		collection.glyphs[readIndex].expand( GRAPHICS_MAX_GLYPHS );

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

			Glyph& glyph = collection.glyphs[writeIndex].append();

			glyph.position = position + offset;

			// avoid sub-pixel positions
			glyph.position.x = (int)( glyph.position.x + 0.5f );
			glyph.position.y = (int)( glyph.position.y + 0.5f );

			glyph.uv = font->getUV( c );
			glyph.size.x = (float)font->getWidth( c );
			glyph.size.y = (float)font->getHeight();
			glyph.color = color;

			offset.x += glyph.size.x;

			index++;
		}

		cur++;
	}
}

void Graphics::queueQuad( const glm::vec2& position, const glm::vec2& size, const glm::vec4& uv, float opacity, Texture* texture  )
{
	const int QUAD_COLLECTION_COUNT = quadCollections.getSize();

	int index = -1;
	for( int i=0; i<QUAD_COLLECTION_COUNT && index < 0; i++ )
		if( quadCollections[i].texture == texture )
			index = i;

	if( index < 0 ) // this is a new texture
	{
		QuadCollection& collection = quadCollections.append();
		collection.texture = texture;
		collection.quads[writeIndex].expand( GRAPHICS_MAX_QUADS );
		collection.quads[readIndex].expand( GRAPHICS_MAX_QUADS );

		index = QUAD_COLLECTION_COUNT;
	}

	Quad& quad = quadCollections[index].quads[writeIndex].append();
	quad.position = position;
	quad.size = size;
	quad.uv = uv;
	quad.opacity = opacity;
}

void Graphics::queueBillboard( const glm::vec3& position, const glm::vec2& size, const glm::vec4& uv, bool spherical, Texture* texture )
{
	const int BILLBOARD_COLLECTION_COUNT = billboardCollections.getSize();

	int index = -1;
	for( int i=0; i<BILLBOARD_COLLECTION_COUNT && index < 0; i++ )
		if( billboardCollections[i].texture == texture )
			index = i;

	if( index < 0 ) // this is a new texture
	{
		BillboardCollection& collection = billboardCollections.append();
		collection.texture = texture;
		collection.billboards[writeIndex].expand( GRAPHICS_MAX_BILLBOARDS );
		collection.billboards[readIndex].expand( GRAPHICS_MAX_BILLBOARDS );

		index = BILLBOARD_COLLECTION_COUNT;
	}

	Billboard& billboard = billboardCollections[index].billboards[writeIndex].append();
	billboard.position = position;
	billboard.uv = uv;
	billboard.size = size;
	billboard.spherical = ( spherical ? 1.0f : 0.0f );
}

Camera& Graphics::getPerspectiveCamera()
{
	return perspectiveCamera;
}

Camera& Graphics::getOrthographicCamera()
{
	return orthographicCamera;
}
