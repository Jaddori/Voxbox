#pragma once

#include "BaseIncludes.h"
#include "Graphics.h"

#define CONSOLE_COLOR_INFORMATION glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f )
#define CONSOLE_COLOR_WARNING glm::vec4( 1.0f, 0.45f, 0.0f, 1.0f )
#define CONSOLE_COLOR_ERROR glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f )
#define CONSOLE_COLOR_DEBUG glm::vec4( 0.65f, 0.65f, 1.0f, 1.0f )

class Console
{
public:
	Console();
	~Console();

	bool load();
	void unload();
	void finalize();

	void toggle();
	void render( Graphics* graphics );

	void setVisible( bool visible );

private:
	Font font;
	bool visible;

	Array<LogMessage> finalMessages;
	int finalThreshold;
};