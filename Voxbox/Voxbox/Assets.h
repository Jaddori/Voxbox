#pragma once

#include "BaseIncludes.h"
#include "Font.h"
#include "Texture.h"

class Assets
{
public:
	Assets();
	~Assets();

	void unload();
	void upload();

	Texture* loadTexture( const char* path );
	void unloadTexture( Texture* texture );

	Font* loadFont( const char* infoPath, const char* texturePath );
	void unloadFont( Font* font );

private:
	template<typename T>
	int findVacant( const Array<T*>& container )
	{
		int index = -1;
		const int COUNT = container.getSize();
		for( int i=0; i<COUNT && index < 0; i++ )
			if( container[i] == nullptr )
				index = i;
		return index;
	}

	int findAsset( const Array<uint64_t>& container, uint64_t hash )
	{
		int index = -1;
		const int COUNT = container.getSize();
		for( int i=0; i<COUNT && index < 0; i++ )
			if( container[i] == hash )
				index = i;
		return index;
	}

	inline uint64_t hashPath( const char* path )
	{
		uint64_t hash = 5381;
		int c;

		while (c = *path++)
			hash = ((hash << 5) + hash) + c;

		return hash;
	}

	Array<Texture*> textures;
	Array<Texture*> uploadTextures;
	Array<uint64_t> textureHashes;

	Array<Font*> fonts;
	Array<Font*> uploadFonts;
	Array<uint64_t> fontHashes;
};