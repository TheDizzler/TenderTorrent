#pragma once

#pragma comment (lib, "dinput8.lib")

#include <dinput.h>
#include <DirectXMath.h>
#include <GamePad.h>

#include "../GameObjects/GUIObjects/MouseController.h"


using namespace DirectX;


class Input {
public:
	Input();
	~Input();

	bool initDirectInput(HINSTANCE hInstance, HWND hwnd);
	virtual void detectInput(double time) = 0;


protected:
	IDirectInputDevice8* inputKB;
	IDirectInputDevice8* inputMouse;
	//IDirectInputDevice8* inputJoystick;

	/*std::unique_ptr<GamePad> gamepad;
	GamePad::ButtonStateTracker buttons;*/

	//DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 directInput;


	std::unique_ptr<MouseController> mouse;

};

