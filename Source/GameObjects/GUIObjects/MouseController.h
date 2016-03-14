#pragma once

#include <dinput.h>
#include "../Graphics/Sprite.h"


class MouseController : public Sprite {
public:

	/*typedef struct MouseState {

		bool buttonDown[3];

	};*/

	MouseController();
	~MouseController();

	DIMOUSESTATE setCurrentState();
	DIMOUSESTATE currentState;
	DIMOUSESTATE lastState;

	//MouseState lastState;
private:

	

};