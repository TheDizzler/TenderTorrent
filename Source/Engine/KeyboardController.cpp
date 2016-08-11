#include "KeyboardController.h"

KeyboardController::KeyboardController() {
}

KeyboardController::~KeyboardController() {
}

void KeyboardController::getInput(RAWKEYBOARD* rawKey) {

	getLastInput();

	//rawKey = rwky;

	bool down;

	if (rawKey->Message == WM_KEYDOWN) {
		down = true;
	} else if (rawKey->Message == WM_KEYUP) {
		down = false;
	} else {
		// something else that we don't care about??
		return;
	}

	UINT keyChar = rawKey->VKey;

	switch (keyChar) {
		/*case VK_LEFT:
		case VK_NUMPAD4:*/
		case 'A':
		case 'a':
			keyDown[LEFT] = down;
			break;

		/*case VK_RIGHT:
		case VK_NUMPAD6:*/
		case 'D':
		case 'd':
			keyDown[RIGHT] = down;
			break;

		/*case VK_UP:
		case VK_NUMPAD8:*/
		case 'W':
		case 'w':
			keyDown[UP] = down;
			break;

		/*case VK_DOWN:
		case VK_NUMPAD2:*/
		case 'S':
		case 's':
			keyDown[DOWN] = down;
			break;

		case VK_SPACE:
			keyDown[FIRE] = down;
			break;

		case VK_RETURN:
		case 'c':
		case 'C':
			keyDown[SELECT] = down;
			break;

		case 'x':
		case 'X':
			keyDown[CANCEL] = down;
			break;

		case VK_ESCAPE:
		case 'p':
		case 'P':
			keyDown[ESC] = down;
			break;

	}
}

void KeyboardController::getLastInput() {

	for (int i = 0; i < KEYCOUNT; ++i)
		lastDown[i] = keyDown[i];
}
