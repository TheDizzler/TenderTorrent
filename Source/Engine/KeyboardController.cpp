#include "KeyboardController.h"

KeyboardController::KeyboardController() {
}

KeyboardController::~KeyboardController() {
}

#include <sstream>
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
	bool otherKeyUp = rawKey->Message == WM_KEYUP;
	bool otherKeyDown = rawKey->Message == WM_KEYDOWN;
	bool keyUp = rawKey->Flags & RI_KEY_BREAK;
	//bool keyMake = rawKey->Flags & RI_KEY_MAKE; // this is always 0 
	USHORT keyChar = rawKey->VKey;

	switch (keyChar) {
		/*case VK_LEFT:
		case VK_NUMPAD4:*/
		case 'A':
		case 'a':
		{
			
			//keyDown[LEFT] = setIsDown(keyDown[LEFT], keyUp);
			keyDown[LEFT] = !keyUp;
			std::wostringstream ws;
			ws << "keyUp: " << keyUp << "\n";
			ws << "WM_KEYDOWN: " << (rawKey->Message == WM_KEYDOWN) << "\n";
			ws << "WM_KEYUP: " << (rawKey->Message == WM_KEYUP) << "\n";
			ws << "otherKeyUp: " << otherKeyUp << "\n";
			ws << "otherKeyDown: " << otherKeyDown << "\n\n";
			OutputDebugString(ws.str().c_str());
		}
		break;

	/*case VK_RIGHT:
	case VK_NUMPAD6:*/
		case 'D':
		case 'd':
			keyDown[RIGHT] = !keyUp; /*setIsDown(keyDown[RIGHT], keyUp);*/
			break;

		/*case VK_UP:
		case VK_NUMPAD8:*/
		case 'W':
		case 'w':
			keyDown[UP] = !keyUp; /*setIsDown(keyDown[UP], keyUp);*/
			break;

		/*case VK_DOWN:
		case VK_NUMPAD2:*/
		case 'S':
		case 's':
			keyDown[DOWN] = !keyUp; /*setIsDown(keyDown[DOWN], keyUp);*/
			break;

		case VK_SPACE:
			keyDown[FIRE] = !keyUp; /*setIsDown(keyDown[FIRE], keyUp);*/
			break;

		case VK_RETURN:
		case 'c':
		case 'C':
			keyDown[SELECT] = !keyUp; /*setIsDown(keyDown[SELECT], keyUp);*/
			break;

		case 'x':
		case 'X':
			keyDown[CANCEL] = !keyUp; /*setIsDown(keyDown[CANCEL], keyUp);*/
			break;

		case VK_ESCAPE:
		case 'p':
		case 'P':
			keyDown[ESC] = !keyUp; /*setIsDown(keyDown[ESC], keyUp);*/
			break;

	}
}


void KeyboardController::getLastInput() {

	for (int i = 0; i < KEYCOUNT; ++i)
		lastDown[i] = keyDown[i];
}
