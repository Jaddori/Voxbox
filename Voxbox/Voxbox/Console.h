#pragma once

#include "BaseIncludes.h"
#include "Graphics.h"

class Console
{
public:
	Console();
	~Console();

	bool load();
	void unload();

	void toggle();
	void render( Graphics* graphics );

	void setVisible( bool visible );

private:
	Font font;
	bool visible;
};