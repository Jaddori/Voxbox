#include "Input.h"

Input::Input()
{
	memset( keys, 0, INPUT_MAX_KEYS );
	memset( prevKeys, 0, INPUT_MAX_KEYS );
	memset( buttons, 0, INPUT_MAX_BUTTONS );
	memset( prevButtons, 0, INPUT_MAX_BUTTONS );

	LOG( VERBOSITY_INFORMATION, "Input.cpp - Constructing." );
}

Input::~Input()
{
	LOG( VERBOSITY_INFORMATION, "Input.cpp - Destructing." );
}

void Input::reset()
{
	memcpy( prevKeys, keys, INPUT_MAX_KEYS );
	memcpy( prevButtons, buttons, INPUT_MAX_BUTTONS );
	prevMousePosition = mousePosition;
	prevMouseWheel = mouseWheel;
}

bool Input::update( SDL_Event* e )
{
	bool handled = true;

	switch( e->type )
	{
		case SDL_KEYDOWN:
		{
			keys[e->key.keysym.scancode] = true;
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

		default:
			handled = false;
			break;
	}

	return handled;
}

bool Input::keyDown( int key )
{
	return keys[key];
}

bool Input::keyPressed( int key )
{
	return ( keys[key] && !prevKeys[key] );
}

bool Input::keyReleased( int key )
{
	return ( !keys[key] && prevKeys[key] );
}

bool Input::buttonDown( int button )
{
	return buttons[button];
}

bool Input::buttonPressed( int button )
{
	return ( buttons[button] && !prevButtons[button] );
}

bool Input::buttonReleased( int button )
{
	return ( !buttons[button] && prevButtons[button] );
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

float Input::getMouseWheel() const
{
	return mouseWheel;
}

float Input::getMouseWheelDelta() const
{
	return ( prevMouseWheel - mouseWheel );
}