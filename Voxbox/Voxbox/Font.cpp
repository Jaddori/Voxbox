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

		result = texture.load( texturePath );
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

const FontInfo& Font::getInfo() const
{
	return info;
}

const Texture& Font::getTexture() const
{
	return texture;
}