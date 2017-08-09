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
		const Array<LogMessage>& messages = Log::instance().getMessages();

		int numMessages = messages.getSize();
		if( numMessages > 10 )
			numMessages = 10;

		for( int i=0; i<numMessages; i++ )
		{
			int index = messages.getSize() - numMessages + i;
			glm::vec2 position( 8.0f, i*font.getHeight() );
			graphics->renderText( &font, messages[index].message, position );
		}
	}
}

void Console::setVisible( bool v )
{
	visible = v;
}