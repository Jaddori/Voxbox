#pragma once

#include "BaseIncludes.h"
#include "Texture.h"

#define FONT_FIRST 32
#define FONT_LAST 128
#define FONT_RANGE (FONT_LAST-FONT_FIRST)

struct FontInfo
{
	uint16_t bitmapSize;
	uint16_t height;
	uint16_t widths[FONT_RANGE];
	uint16_t horizontalOffsets[FONT_RANGE];
	uint16_t verticalOffsets[FONT_RANGE];
};

class Font
{
public:
	Font();
	~Font();

	bool load( const char* infoPath, const char* texturePath );
	void upload();
	void unload();

	const FontInfo& getInfo() const;
	const Texture& getTexture() const;

private:
	FontInfo info;
	Texture texture;
};