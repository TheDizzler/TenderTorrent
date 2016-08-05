#include "MouseController.h"

MouseController::MouseController() {
}

MouseController::~MouseController() {
}

DIMOUSESTATE MouseController::setCurrentState() {

	lastState = currentState;

	lastButtons = currentButtons;
	/*lastButtons.leftButtonDown = currentButtons.leftButtonDown;
	lastButtons.midButtonDown = currentButtons.midButtonDown;
	lastButtons.rightButtonDown = currentButtons.rightButtonDown;*/


	currentButtons.leftButtonDown = GetKeyState(VK_LBUTTON) & 0x8000;
	currentButtons.midButtonDown = GetKeyState(VK_MBUTTON) & 0x8000;
	currentButtons.rightButtonDown = GetKeyState(VK_RBUTTON) & 0x8000;

	return currentState;
}

bool MouseController::leftButtonDown() {
	return currentButtons.leftButtonDown;
}

bool MouseController::midButtonDown() {
	return currentButtons.midButtonDown;
}

bool MouseController::rightButtonDown() {
	return currentButtons.rightButtonDown;
}

bool MouseController::leftButtonLastDown() {
	return lastButtons.leftButtonDown;
}

bool MouseController::midButtonLastDown() {
	return lastButtons.midButtonDown;
}

bool MouseController::rightButtonLastDown() {
	return lastButtons.rightButtonDown;
}

void MouseController::leftButtonHandled() {

	currentButtons.leftButtonDown = false;
}

