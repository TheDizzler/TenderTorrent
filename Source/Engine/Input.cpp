#include "Input.h"


Input::Input() {
}


Input::~Input() {

	ReleaseCapture();
	ShowCursor(true);
}

#include "../Managers/GameManager.h"
bool Input::initRawInput(HWND hwnd) {

	keys.reset(new KeyboardController());
	mouse = make_unique<MouseController>(hwnd);
	if (guiFactory != NULL)
		mouse->loadMouseIcon(guiFactory.get(), "Mouse Icon");


	return true;
}
