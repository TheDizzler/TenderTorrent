#include "../pch.h"
#pragma once


#include "../DXTKGui/Controllers/MouseController.h"
#include "../DXTKGui/Controllers/KeyboardController.h"



class Input {
public:
	Input();
	virtual ~Input();

	bool initRawInput(HWND hwnd);


protected:

	std::unique_ptr<KeyboardController> keys;
	std::unique_ptr<MouseController> mouse;

};

