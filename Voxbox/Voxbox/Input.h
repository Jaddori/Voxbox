#pragma once

#include "BaseIncludes.h"

#define INPUT_MAX_KEYS 128
#define INPUT_MAX_BUTTONS 5
#define INPUT_MAX_TEXT_INPUT 32

class Input
{
public:
	Input();
	~Input();

	void reset();
	bool update( SDL_Event* e );

	bool keyDown( int key );
	inline bool keyUp( int key ) { return !keyDown( key ); }
	bool keyPressed( int key );
	bool keyReleased( int key );
	bool keyRepeated( int key );

	bool buttonDown( int button );
	inline bool buttonUp( int button ) { return !buttonDown( button ); }
	bool buttonPressed( int button );
	bool buttonReleased( int button );

	Point getMousePosition() const;
	Point getMouseDelta() const;
	int getMouseWheel() const;
	int getMouseWheelDelta() const;
	const char* getTextInput() const;

private:
	bool keys[INPUT_MAX_KEYS];
	bool prevKeys[INPUT_MAX_KEYS];
	bool buttons[INPUT_MAX_BUTTONS];
	bool prevButtons[INPUT_MAX_BUTTONS];
	bool repeatedKeys[INPUT_MAX_KEYS];

	Point mousePosition;
	Point prevMousePosition;
	int mouseWheel;
	int prevMouseWheel;

	char textInput[INPUT_MAX_TEXT_INPUT];
};