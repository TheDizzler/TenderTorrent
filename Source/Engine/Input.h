#pragma once

#pragma comment (lib, "dinput8.lib")

#include <dinput.h>
#include <DirectXMath.h>
#include <GamePad.h>

#include "../DXTKGui/Controllers/MouseController.h"
#include "../DXTKGui/Controllers/KeyboardController.h"


using namespace DirectX;


class Input {
public:
	Input();
	virtual ~Input();

	bool initRawInput(HWND hwnd);


protected:

	std::unique_ptr<KeyboardController> keys;
	std::unique_ptr<MouseController> mouse;

};

