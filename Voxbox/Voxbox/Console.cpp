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

void Console::toggle()
{
	visible = !visible;
}

void Console::render( Graphics* graphics )
{
	if( visible )
	{
		// draw background
		graphics->renderQuad( glm::vec2( 0.0f, 0.0f ), glm::vec2( 640.0f, 256.0f ), nullptr, 0.45f );

		// draw messages
		const Array<LogMessage>& messages = Log::instance().getMessages();
		const int threshold = Log::instance().getThreshold();

		float yoffset = 256.0f;
		for( int i=messages.getSize()-1; i>=0 && yoffset > 0.0f; i-- )
		{
			const LogMessage& message = messages[i];
			if( message.verbosity >= threshold )
			{
				glm::vec2 textBounds = font.measureText( message.message );

				yoffset -= textBounds.y;
				if( yoffset >= 0.0f )
				{
					graphics->renderText( &font, message.message, glm::vec2( 8.0f, yoffset ) );
				}
			}
		}
	}
}

void Console::setVisible( bool v )
{
	visible = v;
}