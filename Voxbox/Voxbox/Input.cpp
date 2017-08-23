#include "Input.h"

Input::Input()
	: active( true )
{
	memset( keys, 0, INPUT_MAX_KEYS );
	memset( prevKeys, 0, INPUT_MAX_KEYS );
	memset( buttons, 0, INPUT_MAX_BUTTONS );
	memset( prevButtons, 0, INPUT_MAX_BUTTONS );

	LOG_INFO( "Constructing." );
}

Input::~Input()
{
	LOG_INFO( "Destructing." );
}

void Input::reset()
{
	memcpy( prevKeys, keys, INPUT_MAX_KEYS );
	memset( repeatedKeys, 0, INPUT_MAX_KEYS );
	memcpy( prevButtons, buttons, INPUT_MAX_BUTTONS );
	prevMousePosition = mousePosition;
	mouseWheel = 0;
	textInput[0] = 0;
}

bool Input::update( SDL_Event* e )
{
	bool handled = true;

	switch( e->type )
	{
		case SDL_KEYDOWN:
		{
			if( e->key.keysym.scancode < INPUT_MAX_KEYS )
			{
				keys[e->key.keysym.scancode] = true;
				repeatedKeys[e->key.keysym.scancode] = true;
			}
		} break;

		case SDL_KEYUP:
		{
			keys[e->key.keysym.scancode] = false;
		} break;

		case SDL_MOUSEBUTTONDOWN:
		{
			buttons[e->button.button] = true;
		} break;

		case SDL_MOUSEBUTTONUP:
		{
			buttons[e->button.button] = false;
		} break;

		case SDL_MOUSEMOTION:
		{
			mousePosition.x = e->motion.x;
			mousePosition.y = e->motion.y;
		} break;

		case SDL_MOUSEWHEEL:
		{
			mouseWheel = e->wheel.y;
		} break;

		case SDL_TEXTINPUT:
		{
			memcpy( textInput, e->text.text, sizeof(char)*INPUT_MAX_TEXT_INPUT );
		} break;

		case SDL_WINDOWEVENT:
		{
			if( e->window.event == SDL_WINDOWEVENT_ENTER )
			{
				active = true;
			}
			else if( e->window.event == SDL_WINDOWEVENT_LEAVE )
			{
				active = false;
			}
			else
				handled = false;
		} break;

		default:
			handled = false;
			break;
	}

	return handled;
}

bool Input::keyDown( int key )
{
	LOG_ASSERT( key >= 0 && key < INPUT_MAX_KEYS, "Bad key argument: %d", key );
	return ( active && keys[key] );
}

bool Input::keyPressed( int key )
{
	LOG_ASSERT( key >= 0 && key < INPUT_MAX_KEYS, "Bad key argument: %d", key );
	return ( active && keys[key] && !prevKeys[key] );
}

bool Input::keyReleased( int key )
{
	LOG_ASSERT( key >= 0 && key < INPUT_MAX_KEYS, "Bad key argument: %d", key );
	return ( active && !keys[key] && prevKeys[key] );
}

bool Input::keyRepeated( int key )
{
	LOG_ASSERT( key >= 0 && key < INPUT_MAX_KEYS, "Bad key argument: %d", key );
	return ( active && repeatedKeys[key] );
}

bool Input::buttonDown( int button )
{
	LOG_ASSERT( button >= 0 && button < INPUT_MAX_KEYS, "Bad button argument: %d", button );
	return ( active && buttons[button] );
}

bool Input::buttonPressed( int button )
{
	LOG_ASSERT( button >= 0 && button < INPUT_MAX_KEYS, "Bad button argument: %d", button );
	return ( active && buttons[button] && !prevButtons[button] );
}

bool Input::buttonReleased( int button )
{
	LOG_ASSERT( button >= 0 && button < INPUT_MAX_KEYS, "Bad button argument: %d", button );
	return ( active && !buttons[button] && prevButtons[button] );
}

Point Input::getMousePosition() const
{
	return mousePosition;
}

Point Input::getMouseDelta() const
{
	Point result =
	{
		prevMousePosition.x - mousePosition.x,
		prevMousePosition.y - mousePosition.y
	};

	return result;
}

int Input::getMouseWheel() const
{
	return mouseWheel;
}

const char* Input::getTextInput() const
{
	return textInput;
}

bool Input::getActive() const
{
	return active;
}