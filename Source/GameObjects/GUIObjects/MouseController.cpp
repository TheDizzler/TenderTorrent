#include "MouseController.h"

MouseController::MouseController() {
}

MouseController::~MouseController() {
}

DIMOUSESTATE MouseController::setCurrentState() {
	
	lastState = currentState;

	/*lastState.buttonDown[0] = GetKeyState(VK_LBUTTON) & 0x8000;
	lastState.buttonDown[1] = GetKeyState(VK_MBUTTON) & 0x8000;
	lastState.buttonDown[2] = GetKeyState(VK_RBUTTON) & 0x8000;*/

	return currentState;
}
