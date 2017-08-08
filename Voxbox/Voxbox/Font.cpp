#include "Font.h"

Font::Font()
{
}

Font::~Font()
{
}

bool Font::load( const char* path )
{
	bool result = false;

	FILE* file = fopen( path, "rb" );
	if( file )
	{
		fread( &info, sizeof(info), 1, file );
		fclose( file );

		result = true;
	}

	return result;
}

const FontInfo& Font::getInfo() const
{
	return info;
}