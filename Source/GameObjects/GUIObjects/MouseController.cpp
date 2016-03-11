#include "MouseController.h"

MouseController::MouseController() {
}

MouseController::~MouseController() {
}

DIMOUSESTATE MouseController::setCurrentState() {
	
	lastState = currentState;
	return currentState;
}
