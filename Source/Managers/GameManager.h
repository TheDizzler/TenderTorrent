#pragma once

#include "LevelManager.h"
#include "MenuManager.h"



enum GameState {
	menu, paused, playing
};


class GameEngine;

/** The lowest level of class where game code should be included.
	Everything below this (GameEngine downward) should generally go unmodified. */
class GameManager {
public:
	GameManager(GameEngine* gameEngine);
	~GameManager();


	bool initializeGame(ID3D11Device* device, MouseController* mouse);


	void update(double deltaTime, BYTE keyboardState[256], MouseController* mouse);
	void draw(SpriteBatch* batch);


	void loadLevel();
	void loadMainMenu();
	void exit();

private:

	//unique_ptr<Screen> currentScreen;
	Screen* currentScreen;
	Screen* lastScreen = 0;

	GameEngine* gameEngine;
	MouseController* mouse;
	ID3D11Device* device;
	


	
};