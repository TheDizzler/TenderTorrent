#include "Input.h"


Input::Input() {
}


Input::~Input() {

	//inputKB->Unacquire();
	//inputMouse->Unacquire();
	//inputJoystick->Unacquire();
	//directInput->Release();

	ReleaseCapture();
	ShowCursor(true);
}

bool Input::initRawInput(HWND hwnd) {

	RAWINPUTDEVICE rid[2];
	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x06; // keyboard
	rid[0].dwFlags = RIDEV_NOLEGACY; // do not generate legacy messages such as WM_KEYDOWN
	rid[0].hwndTarget = hwnd; // 0??

	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x02; // mouse
	rid[1].dwFlags = /*RIDEV_CAPTUREMOUSE|*/ RIDEV_INPUTSINK;
	rid[1].hwndTarget = hwnd;
	
	if (!RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE)))
		return false;

	keys.reset(new KeyboardController());
	mouse.reset(new MouseController(hwnd));
	

	return true;
}

void Input::setRawInput(RAWINPUT* raw) {

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
		keys->getInput(&raw->data.keyboard);

	if (raw->header.dwType == RIM_TYPEMOUSE)
		mouse->getRawInput(&raw->data.mouse);
}

/* This is DirectInput junk! Deprecated!! 
bool Input::initDirectInput(HINSTANCE hInstance, HWND hwnd) {


	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**) &directInput, NULL)))

		return false;

	if (FAILED(directInput->CreateDevice(GUID_SysKeyboard, &inputKB, NULL)))

		return false;

	if (FAILED(directInput->CreateDevice(GUID_SysMouse, &inputMouse, NULL)))

		return false;

	//if (FAILED(directInput->CreateDevice(GUID_Joystick, &inputJoystick, NULL)))
	//	return false;

	//	gamepad.reset(new GamePad);
	//	gamepad->Resume();
	//	buttons.Reset();

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
}*/




