#if !defined(USERINPUT_H)
/*****************************************************************************
*	$Note:	$
*	$Author: TDGillon $
*	$Notice: (C) Copyright 2023 by AtomosZ Industries. All rights reserved. $
*****************************************************************************/

struct ButtonState
{
	enum State
	{
		UP = 0,         // Button is up
		HELD = 1,       // Button is held down
		RELEASED = 2,   // Button was just released
		PRESSED = 3,    // Buton was just pressed
	};
};


struct MouseInput
{
	struct State
	{
		bool32	leftButton;
		bool32	middleButton;
		bool32	rightButton;
		bool32	xButton1;
		bool32	xButton2;

		float	x;
		float	y;

		int32	scrollWheelValue; // this should be zeroed every frame?
	};

	State state;

	ButtonState::State leftButton;
	ButtonState::State middleButton;
	ButtonState::State rightButton;
	ButtonState::State xButton1;
	ButtonState::State xButton2;

	bool32 hasMoved;
};

/// <summary>
/// Should be able to index using letter or VK_ key. Ex: keys['D']
/// </summary>
struct KeyboardInput
{
	ButtonState::State keys[256];
};

struct GameInput
{
	MouseInput mouse;
	KeyboardInput keys;

	float deltaTime;
	//ControllerInput controllers[5];
};

#define USERINPUT_H
#endif