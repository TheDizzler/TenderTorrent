#include "Input.h"


Input::Input() {
}


Input::~Input() {

	inputKB->Unacquire();
	inputMouse->Unacquire();
	//inputJoystick->Unacquire();
	directInput->Release();
}


bool Input::initDirectInput(HINSTANCE hInstance, HWND hwnd) {


	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**) &directInput, NULL)))

		return false;

	if (FAILED(directInput->CreateDevice(GUID_SysKeyboard, &inputKB, NULL)))

		return false;

	if (FAILED(directInput->CreateDevice(GUID_SysMouse, &inputMouse, NULL)))

		return false;

	/*if (FAILED(directInput->CreateDevice(GUID_Joystick, &inputJoystick, NULL)))
		return false;*/

		/*gamepad.reset(new GamePad);
		gamepad->Resume();
		buttons.Reset();*/

	if (FAILED(inputKB->SetDataFormat(&c_dfDIKeyboard)))

		return false;

	if (FAILED(inputKB->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))

		return false;

	if (FAILED(inputMouse->SetDataFormat(&c_dfDIMouse)))

		return false;

	if (FAILED(inputMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND)))
		return false;

	mouse.reset(new MouseController());

	return true;
}




