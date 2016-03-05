#pragma once

#include "GraphicsEngine.h"
#include "Input.h"

#include "../LevelController/LevelManager.h"




class GameEngine : public GraphicsEngine, public Input {
public:

	GameEngine();
	~GameEngine();

	bool initEngine(HWND hwnd, HINSTANCE hInstance);

	void run(double time, int fps);
	virtual void render(double time);
private:

	std::unique_ptr<LevelManager> levelManager;


	POINT cursorPos;

	bool waitingForInput = true;

	BYTE keyboardState[256];
	DIMOUSESTATE mouseCurrentState;
	virtual void detectInput(double time);

	void update(double time);

	bool initStage();



	HWND hwnd;
};

