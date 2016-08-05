#pragma once

#include <dinput.h>
#include "../Graphics/Sprite.h"

class MouseButton {
public:
	bool leftButtonDown = false;
	bool midButtonDown = false;
	bool rightButtonDown = false;

};


class MouseController : public Sprite {
public:

	MouseButton currentButtons;
	MouseButton lastButtons;


	MouseController();
	~MouseController();

	DIMOUSESTATE setCurrentState();
	DIMOUSESTATE currentState;
	DIMOUSESTATE lastState;

	bool leftButtonDown();
	bool midButtonDown();
	bool rightButtonDown();

	bool leftButtonLastDown();
	bool midButtonLastDown();
	bool rightButtonLastDown();


	void leftButtonHandled();

private:

	

};