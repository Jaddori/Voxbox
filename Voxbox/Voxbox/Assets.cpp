#include "Assets.h"

Assets::Assets()
{
}

Assets::~Assets()
{
	unload();
}

void Assets::unload()
{
	// unload textures
	const int TEXTURE_COUNT = textures.getSize();
	for( int i=0; i<TEXTURE_COUNT; i++ )
		textures[i]->unload();

	textures.clear();

	// unload font
	const int FONT_COUNT = fonts.getSize();
	for( int i=0; i<FONT_COUNT; i++ )
		fonts[i]->unload();

	fonts.clear();
}

void Assets::upload()
{
	// upload textures
	const int UPLOAD_TEXTURE_COUNT = uploadTextures.getSize();
	for( int i=0; i<UPLOAD_TEXTURE_COUNT; i++ )
		uploadTextures[i]->upload();

	uploadTextures.clear();

	// upload fonts
	const int UPLOAD_FONT_COUNT = uploadFonts.getSize();
	for( int i=0; i<UPLOAD_FONT_COUNT; i++ )
		uploadFonts[i]->upload();

	uploadFonts.clear();
}

Texture* Assets::loadTexture( const char* path )
{
	LOG_ASSERT( path != nullptr, "Assets.cpp - Trying to load texture from invalid path." );

	Texture* result = nullptr;

	uint64_t hash = hashPath( path );

	LOG_INFO( "Assets.cpp - Loading texture from %s", path );
	LOG_INFO( "Assets.cpp - Hash = %ul", hash );

	int index = findAsset( textureHashes, hash );
	if( index < 0 ) // this is a new path
	{
		LOG_INFO( "Assets.cpp - Hash not found. Loading new." );

		result = new Texture();
		if( result->load( path ) )
		{
			textures.add( result );
			uploadTextures.add( result );
			textureHashes.add( hash );
		}
		else
		{
			LOG_WARNING( "Assets.cpp - Failed to load texture." );
			delete result;
		}
	}
	else
	{
		LOG_INFO( "Assets.cpp - Hash found. Returning existing texture." );
		result = textures[index];
	}

	return result;
}

void Assets::unloadTexture( Texture* texture )
{
	LOG_ASSERT( texture != nullptr, "Assets.cpp - Trying to unload nullptr texture." );

	int index = -1;
	const int TEXTURE_COUNT = textures.getSize();
	for( int i=0; i<TEXTURE_COUNT && index < 0; i++ )
		if( textures[i] == texture )
			index = i;

	if( index >= 0 )
	{
		LOG_INFO( "Assets.cpp - Unloading texture." );

		textures[index]->unload();
		textureHashes[index] = 0;

		index = -1;
		const int UPLOAD_TEXTURE_COUNT = uploadTextures.getSize();
		for( int i=0; i<UPLOAD_TEXTURE_COUNT && index < 0; i++ )
			if( uploadTextures[i] == texture )
				index = i;

		if( index >= 0 )
			uploadTextures[index] = nullptr;
	}
	else
		LOG_WARNING( "Assets.cpp - Trying to unload texture. But texture not found." );
}

Font* Assets::loadFont( const char* infoPath, const char* texturePath )
{
	LOG_ASSERT( infoPath != nullptr, "Assets.cpp - Trying to load font info from invalid path." );
	LOG_ASSERT( texturePath != nullptr, "Assets.cpp - Trying to load font texture from invalid path." );

	Font* result = nullptr;

	uint64_t hash = hashPath( infoPath );

	LOG_INFO( "Assets.cpp - Loading font from %s", infoPath );
	LOG_INFO( "Assets.cpp - Hash = %ul", hash );

	int index = findAsset( fontHashes, hash );
	if( index < 0 ) // this is a new path
	{
		LOG_INFO( "Assets.cpp - Hash not found. Loading new." );

		result = new Font();
		if( result->load( infoPath, texturePath ) )
		{
			fonts.add( result );
			uploadFonts.add( result );
			fontHashes.add( hash );
		}
		else
		{
			LOG_WARNING( "Assets.cpp - Failed to load font." );
			delete result;
		}
	}
	else
	{
		LOG_INFO( "Assets.cpp - Hash found. Returning existing font." );
		result = fonts[index];
	}

	return result;
}

void Assets::unloadFont( Font* font )
{
	LOG_ASSERT( font != nullptr, "Assets.cpp - Trying to unload nullptr font." );

	int index = -1;
	const int FONT_COUNT = fonts.getSize();
	for( int i=0; i<FONT_COUNT && index < 0; i++ )
		if( fonts[i] == font )
			index = i;

	if( index >= 0 )
	{
		LOG_INFO( "Assets.cpp - Unloading font." );

		fonts[index]->unload();
		fontHashes[index] = 0;

		index = -1;
		const int UPLOAD_FONT_COUNT = uploadFonts.getSize();
		for( int i=0; i<UPLOAD_FONT_COUNT && index < 0; i++ )
			if( uploadFonts[i] == font )
				index = i;

		if( index >= 0 )
			uploadFonts[index] = nullptr;
	}
	else
		LOG_WARNING( "Assets.cpp - Trying to unload font. But font not found." );
}