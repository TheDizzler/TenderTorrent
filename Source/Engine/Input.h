#pragma once

#pragma comment (lib, "dinput8.lib")

#include <dinput.h>
#include <DirectXMath.h>
#include <GamePad.h>

#include "MouseController.h"
#include "KeyboardController.h"


using namespace DirectX;


class Input {
public:
	Input();
	~Input();

	bool initRawInput(HWND hwnd);

	void setRawInput(RAWINPUT* raw);

	/** Deprecated! */
	//bool initDirectInput(HINSTANCE hInstance, HWND hwnd);
	//virtual void detectInput(double time) = 0;


protected:
	//IDirectInputDevice8* inputKB;
	//IDirectInputDevice8* inputMouse;
	//IDirectInputDevice8* inputJoystick;

	/*std::unique_ptr<GamePad> gamepad;
	GamePad::ButtonStateTracker buttons;*/

	//DIMOUSESTATE mouseLastState;
	//LPDIRECTINPUT8 directInput;

	std::unique_ptr<KeyboardController> keys;
	std::unique_ptr<MouseController> mouse;

};

