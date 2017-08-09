#include "Font.h"

Font::Font()
{
}

Font::~Font()
{
}

bool Font::load( const char* infoPath, const char* texturePath )
{
	bool result = false;

	FILE* file = fopen( infoPath, "rb" );
	if( file )
	{
		fread( &info, sizeof(info), 1, file );
		fclose( file );

		LOG( VERBOSITY_INFORMATION, "Font.cpp - Loaded font from \"%s\".", infoPath );

		result = texture.load( texturePath );
	}
	else
	{
		LOG( VERBOSITY_WARNING, "Font.cpp - Failed to load font from \"%s\".", infoPath );
	}

	return result;
}

void Font::upload()
{
	texture.upload();
}

void Font::unload()
{
	texture.unload();
}

int Font::getBitmapSize() const
{
	return info.bitmapSize;
}

int Font::getHeight() const
{
	return info.height;
}

int Font::getWidth( char c ) const
{
	assert( c >= 0 && c <= FONT_RANGE );
	return info.widths[c];
}

int Font::getHorizontalOffset( char c ) const
{
	assert( c >= 0 && c <= FONT_RANGE );
	return info.horizontalOffsets[c];
}

int Font::getVerticalOffset( char c ) const
{
	assert( c >= 0 && c <= FONT_RANGE );
	return info.verticalOffsets[c];
}

glm::vec4 Font::getUV( char c ) const
{
	assert( c >= 0 && c <= FONT_RANGE );
	float s = (float)info.horizontalOffsets[c];
	float t = (float)info.verticalOffsets[c];
	float u = s + info.widths[c];
	float v = t + info.height;
	/*float x = 0.0f;
	float y = 0.0f;
	float z = 1.0f;
	float w = 1.0f;*/
	return glm::vec4( s, t, u, v ) / (float)info.bitmapSize;
}

const FontInfo& Font::getInfo() const
{
	return info;
}

const Texture& Font::getTexture() const
{
	return texture;
}