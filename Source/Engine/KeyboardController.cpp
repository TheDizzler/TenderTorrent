#include "KeyboardController.h"

KeyboardController::KeyboardController() {
}

KeyboardController::~KeyboardController() {
}

void KeyboardController::getInput(RAWKEYBOARD* rawKey) {

	getLastInput();

	//rawKey = rwky;


	//bool down;
	//if (rawKey->Message == WM_KEYDOWN) {
	//	down = true;
	//} else if (rawKey->Message == WM_KEYUP) {
	//	down = false;
	//} else {
	//	// something else that we don't care about??
	//	return;
	//}

	bool keyUp = rawKey->Flags & RI_KEY_BREAK;
	USHORT keyChar = rawKey->VKey;

	switch (keyChar) {
		/*case VK_LEFT:
		case VK_NUMPAD4:*/
		case 'A':
		case 'a':
			keyDown[LEFT] = setIsDown(keyDown[LEFT], keyUp);
			break;

		/*case VK_RIGHT:
		case VK_NUMPAD6:*/
		case 'D':
		case 'd':
			keyDown[RIGHT] = setIsDown(keyDown[RIGHT], keyUp);
			break;

		/*case VK_UP:
		case VK_NUMPAD8:*/
		case 'W':
		case 'w':
			keyDown[UP] = setIsDown(keyDown[UP], keyUp);
			break;

		/*case VK_DOWN:
		case VK_NUMPAD2:*/
		case 'S':
		case 's':
			keyDown[DOWN] = setIsDown(keyDown[DOWN], keyUp);
			break;

		case VK_SPACE:
			keyDown[FIRE] = setIsDown(keyDown[FIRE], keyUp);
			break;

		case VK_RETURN:
		case 'c':
		case 'C':
			keyDown[SELECT] = setIsDown(keyDown[SELECT], keyUp);
			break;

		case 'x':
		case 'X':
			keyDown[CANCEL] = setIsDown(keyDown[CANCEL], keyUp);
			break;

		case VK_ESCAPE:
		case 'p':
		case 'P':
			keyDown[ESC] = setIsDown(keyDown[ESC], keyUp);
			break;

	}
}

bool KeyboardController::setIsDown(bool keyLastDown, bool keyNewStateUp) {

	if (!keyNewStateUp)
		if (keyLastDown)
			return false;
		else
			return true;
	else
		if (keyLastDown)
			return true;
		else
			return false;

}


void KeyboardController::getLastInput() {

	for (int i = 0; i < KEYCOUNT; ++i)
		lastDown[i] = keyDown[i];
}
