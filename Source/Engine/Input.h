#pragma once

#pragma comment (lib, "dinput8.lib")
//#pragma comment (lib, "dxguid.lib")

#include <dinput.h>
#include <DirectXMath.h>
#include <GamePad.h>

using namespace DirectX;

//struct Movement {
//
//	float leftRight = 0.0f;
//	float backForward = 0.0f;
//	float upDown = 0.0f;
//	float pitch = 0.0f;
//	float yaw = 0.0f;
//};


class Input {
public:
	Input();
	~Input();

	float rotX = 0;
	float rotY = 0;
	float rotZ = 0;
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	//void update(double time);

	bool initDirectInput(HINSTANCE hInstance, HWND hwnd);
	virtual void detectInput(double time) = 0;
	
	//Movement movement;
	//Movement getMovement();

protected:
	IDirectInputDevice8* inputKB;
	IDirectInputDevice8* inputMouse;
	//IDirectInputDevice8* inputJoystick;

	/*std::unique_ptr<GamePad> gamepad;
	GamePad::ButtonStateTracker buttons;*/

	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 directInput;

};

