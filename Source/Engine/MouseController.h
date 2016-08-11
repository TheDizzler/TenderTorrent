#pragma once

//#include <dinput.h>
#include "../GameObjects/Graphics/Sprite.h"

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

	/** RawInput should be used for moving cameras, etc. 
		If only using mouse as a cursor, use GetCursorPos(). */
	void getRawInput(RAWMOUSE* rawMouse);
	void getLastRawInput();


	bool leftButtonDown();
	bool midButtonDown();
	bool rightButtonDown();

	bool leftButtonLastDown();
	bool midButtonLastDown();
	bool rightButtonLastDown();


	void leftButtonHandled();

private:

	

};