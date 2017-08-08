#pragma once

#include "BaseIncludes.h"

#define INPUT_MAX_KEYS 128
#define INPUT_MAX_BUTTONS 5

struct Point
{
	int x, y;
};

class Input
{
public:
	Input();
	~Input();

	bool update( SDL_Event* e );

	bool keyDown( int key );
	inline bool keyUp( int key ) { return !keyDown( key ); }
	bool keyPressed( int key );
	bool keyReleased( int key );

	bool buttonDown( int button );
	inline bool buttonUp( int button ) { return !buttonDown( button ); }
	bool buttonPressed( int button );
	bool buttonReleased( int button );

	Point getMousePosition() const;
	Point getMouseDelta() const;
	float getMouseWheel() const;
	float getMouseWheelDelta() const;

private:
	bool keys[INPUT_MAX_KEYS];
	bool prevKeys[INPUT_MAX_KEYS];
	bool buttons[INPUT_MAX_BUTTONS];
	bool prevButtons[INPUT_MAX_BUTTONS];

	Point mousePosition;
	Point prevMousePosition;
	float mouseWheel;
	float prevMouseWheel;
};