#include "Console.h"

Console::Console()
	: visible( false )
{
	LOG( VERBOSITY_INFORMATION, "Console.cpp - Constructing." );
}

Console::~Console()
{
	LOG( VERBOSITY_INFORMATION, "Console.cpp - Destructing." );
}

bool Console::load()
{
	bool result = false;

	LOG( VERBOSITY_INFORMATION, "Console.cpp - Loading font." );
	if( font.load( "./assets/fonts/modern12.bin", "./assets/fonts/modern12.dds" ) )
	{
		font.upload();

		result = true;
	}
	else
		LOG( VERBOSITY_WARNING, "Console.cpp - Failed to load font." );

	return result;
}

void Console::unload()
{
	font.unload();
}

void Console::finalize()
{
	finalMessages.fastCopy( Log::instance().getMessages() );
	finalThreshold = Log::instance().getThreshold();
}

void Console::toggle()
{
	visible = !visible;
}

void Console::render( Graphics* graphics )
{
	if( visible )
	{
		// setup colors
		const glm::vec4 TEXT_COLORS[] =
		{
			CONSOLE_COLOR_INFORMATION,
			CONSOLE_COLOR_WARNING,
			CONSOLE_COLOR_ERROR,
			CONSOLE_COLOR_DEBUG
		};

		// draw background
		graphics->renderQuad( glm::vec2( 0.0f, 0.0f ), glm::vec2( 640.0f, 256.0f ), nullptr, 0.45f );

		// draw messages
		float yoffset = 256.0f;
		for( int i=finalMessages.getSize()-1; i>=0 && yoffset > 0.0f; i-- )
		{
			const LogMessage& message = finalMessages[i];
			if( message.verbosity >= finalThreshold )
			{
				glm::vec2 textBounds = font.measureText( message.message );

				yoffset -= textBounds.y;
				if( yoffset >= 0.0f )
				{
					graphics->renderText( &font, message.message, glm::vec2( 8.0f, yoffset ), TEXT_COLORS[message.verbosity] );
				}
			}
		}
	}
}

void Console::setVisible( bool v )
{
	visible = v;
}